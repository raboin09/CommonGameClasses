#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Performance/CommonPerformanceTestActor.h"

#if WITH_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FPerformanceTestSpec, "CommonGame.Performance",
                  EAutomationTestFlags::EditorContext |
                  EAutomationTestFlags::ProductFilter |
                  EAutomationTestFlags::HighPriority)
	END_DEFINE_SPEC(FPerformanceTestSpec)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForFunctionalTest, ACommonPerformanceTestActor*, TestActor);
inline bool FWaitForFunctionalTest::Update()
{
	return TestActor ? TestActor->bFinishedTest : false;
}

inline void FPerformanceTestSpec::Define()
{
	Describe("Performance", [this]()
	{
		It("Should collect and report performance metrics", [this]()
		{
			UWorld* World = GEngine->GetWorldFromContextObject(
				GEngine->GameViewport, EGetWorldErrorMode::ReturnNull);
			TestNotNull("World exists", World);

			// Spawn performance test actor
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = 
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ACommonPerformanceTestActor* TestActor = 
				World->SpawnActor<ACommonPerformanceTestActor>(SpawnParams);
			TestNotNull("Test actor spawned", TestActor);
			TestActor->TestDuration = 30.0f;
			TestActor->WarmupDuration = 5.0f;
			TestActor->StartTest();
			ADD_LATENT_AUTOMATION_COMMAND(FWaitForFunctionalTest(TestActor));
		});
	});
}
#endif
