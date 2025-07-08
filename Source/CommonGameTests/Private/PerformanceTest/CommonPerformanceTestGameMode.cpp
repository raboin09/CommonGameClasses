#include "PerformanceTest/CommonPerformanceTestGameMode.h"

#include "Kismet/GameplayStatics.h"

ACommonPerformanceTestGameMode::ACommonPerformanceTestGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsWarming = true;
    bIsCapturing = false;
    WarmupTimer = 0.0f;
    CapturedFrames = 0;
}

void ACommonPerformanceTestGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Starting session"));
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Warming up for %.1f seconds..."), WarmupTime);
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Will capture %d frames"), FramesToCapture);
}

void ACommonPerformanceTestGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bIsWarming)
    {
        WarmupTimer += DeltaSeconds;
        if (WarmupTimer >= WarmupTime)
        {
            bIsWarming = false;
            StartCapture();
        }
    }
    else if (bIsCapturing)
    {
        CapturedFrames++;
        
        // Log progress every 10 frames
        if (CapturedFrames % 10 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("CSV Capture: Captured %d/%d frames"), 
                CapturedFrames, FramesToCapture);
        }

        if (CapturedFrames >= FramesToCapture)
        {
            StopCapture();
        }
    } else if (bIsDoneCapturing)
    {
        CapturedFrames++;
        // Give 20 frames to write csv
        if(CapturedFrames >= FramesToCapture + 20)
        {
            UE_LOG(LogTemp, Log, TEXT("CSV Capture: Requesting application exit"));
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
            FPlatformMisc::RequestExit(true);
        }
    }

}

void ACommonPerformanceTestGameMode::StartCapture()
{
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Starting capture"));
    FCsvProfiler::Get()->BeginCapture();
    CaptureStartTime = FDateTime::Now();
    bIsCapturing = true;
}

void ACommonPerformanceTestGameMode::StopCapture()
{
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Stopping capture"));
    
    CSV_METADATA(TEXT("CSVCapture"), TEXT("End"));
    FCsvProfiler::Get()->EndCapture();
    bIsCapturing = false;

    float CaptureTime = (FDateTime::Now() - CaptureStartTime).GetTotalSeconds();
    
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Capture completed"));
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Total frames: %d"), CapturedFrames);
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Capture time: %.2f seconds"), CaptureTime);
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Average FPS: %.2f"), 
        CapturedFrames / CaptureTime);
    
    // Log CSV file location
    FString CSVDirectory = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("CSVProfiles"));
    UE_LOG(LogTemp, Log, TEXT("CSV Capture: Output directory: %s"), *CSVDirectory);
    bIsDoneCapturing = true;
}

void ACommonPerformanceTestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    if (bIsCapturing)
    {
        StopCapture();
    }
}
