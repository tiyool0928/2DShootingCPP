// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget != nullptr)
	{
		//mainWidget 블루프린트 파일을 메모리에 로드
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		//뷰포트에 출력
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}

void AShootingGameModeBase::PrintScore()
{
	if (mainUI != nullptr)
	{
		mainUI->scoreData->SetText(FText::AsNumber(CurrentScore));
	}
}

void AShootingGameModeBase::AddScore(int32 point)
{
	CurrentScore += point;

	PrintScore();
}

void AShootingGameModeBase::ShowMenu()
{
	if (menuWidget != nullptr)
	{
		menuUI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

		if (menuUI != nullptr)
		{
			menuUI->AddToViewport();
		}
		//일시정지상태
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//커서 활성화
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}

