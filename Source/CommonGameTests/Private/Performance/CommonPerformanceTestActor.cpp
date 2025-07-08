#include "Performance/CommonPerformanceTestActor.h"

#include "RHI.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformProcess.h"

ACommonPerformanceTestActor::ACommonPerformanceTestActor()
{
    PrimaryActorTick.bCanEverTick = true;
    TestDuration = 30.0f;
    WarmupDuration = 5.0f;
    ElapsedTime = 0.0f;
    bIsWarming = true;
    Report = FPerformanceReport();   
}

void ACommonPerformanceTestActor::PrepareTest()
{
    Super::PrepareTest();

    // Initialize report
    Report = FPerformanceReport();
    Report.BuildId = FGenericPlatformMisc::GetEnvironmentVariable(TEXT("BUILD_NUMBER"));
    Report.MapName = GetWorld()->GetMapName();
    Report.TestTime = FDateTime::Now();
}

void ACommonPerformanceTestActor::StartTest()
{
    Super::StartTest();
    UE_LOG(LogTemp, Log, TEXT("Starting performance test with %f seconds warmup"), WarmupDuration);
}

void ACommonPerformanceTestActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsWarming)
    {
        ElapsedTime += DeltaSeconds;
        if (ElapsedTime >= WarmupDuration)
        {
            bIsWarming = false;
            ElapsedTime = 0.0f;
            UE_LOG(LogTemp, Log, TEXT("Warmup complete, starting measurements"));
        }
        return;
    }

    if (ElapsedTime < TestDuration)
    {
        ElapsedTime += DeltaSeconds;
        CollectFrameData();
    }
    else
    {
        bFinishedTest = true;
        ProcessResults();
        ExportResults();
        ReportToTeamCity();
        FinishTest(EFunctionalTestResult::Succeeded, TEXT("Performance test completed"));
    }
}

void ACommonPerformanceTestActor::CollectFrameData()
{
    FFrameData FrameData;
    FrameData.TimeStamp = ElapsedTime;
    FrameData.FrameTime = FApp::GetDeltaTime();
    Report.FrameHistory.Add(FrameData);
}

void ACommonPerformanceTestActor::ProcessResults()
{
    if (Report.FrameHistory.Num() == 0)
        return;

    // Calculate FPS metrics
    TArray<float> FPSValues;
    float TotalFPS = 0.0f;
    float MinFrameTime = TNumericLimits<float>::Max();
    float MaxFrameTime = 0.0f;

    for (const FFrameData& Frame : Report.FrameHistory)
    {
        float FPS = 1.0f / Frame.FrameTime;
        FPSValues.Add(FPS);
        TotalFPS += FPS;
        
        MinFrameTime = FMath::Min(MinFrameTime, Frame.FrameTime);
        MaxFrameTime = FMath::Max(MaxFrameTime, Frame.FrameTime);
    }

    // Calculate averages
    Report.AverageFPS = TotalFPS / Report.FrameHistory.Num();
    Report.MinFPS = 1.0f / MaxFrameTime;
    Report.MaxFPS = 1.0f / MinFrameTime;

    // Calculate 99th percentile FPS
    FPSValues.Sort();
    int32 P99Index = FMath::FloorToInt(FPSValues.Num() * 0.01f);
    Report.P99FPS = FPSValues[P99Index];
}

void ACommonPerformanceTestActor::ExportResults() const
{
    // Export to JSON
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(Report, JsonString);

    FString FilePath = FPaths::Combine(
        FPaths::ProjectSavedDir(), 
        TEXT("PerformanceTests"),
        FString::Printf(TEXT("PerfTest_%s_%s.json"),
            *Report.MapName,
            *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")))
    );

    FString DirectoryPath = FPaths::GetPath(FilePath);
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*DirectoryPath))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*DirectoryPath);
    }

    if (FFileHelper::SaveStringToFile(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Performance report saved to: %s"), *FilePath);
    }
}

void ACommonPerformanceTestActor::ReportToTeamCity() const
{
    TArray<FString> Messages = {
        FString::Printf(TEXT("##teamcity[buildStatisticValue key='AverageFPS' value='%.2f']"), 
            Report.AverageFPS),
        FString::Printf(TEXT("##teamcity[buildStatisticValue key='MinFPS' value='%.2f']"), 
            Report.MinFPS),
        FString::Printf(TEXT("##teamcity[buildStatisticValue key='MaxFPS' value='%.2f']"), 
            Report.MaxFPS),
        FString::Printf(TEXT("##teamcity[buildStatisticValue key='P99FPS' value='%.2f']"), 
            Report.P99FPS)
    };

    for (const FString& Message : Messages)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
}