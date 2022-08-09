// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//박스 콜라이더 컴포넌트를 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	//박스 콜라이더 컴포넌트를 루트 컴포넌트로 설정
	SetRootComponent(boxComp);
	//스태틱 메시 컴포넌트를 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));
	//메시 컴포넌트를 박스 콜라이더 컴포넌트의 자식 컴포넌트로 설정
	meshComp->SetupAttachment(boxComp);

	//박스 콜라이더 컴포넌트의 크기를 50 x 50 x 50으로 설정
	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	//총구 표시 컴포넌트 생성 및 루트의 자식으로 설정
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);

	//오버랩 이벤트 활성
	boxComp->SetGenerateOverlapEvents(true);
	//충돌 응답을 query & physics로 설정
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Object Type을 player로 설정
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	//모든 채널과의 충돌을 응답 없음으로 설정
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//적과는 충돌 이벤트 체크
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	//WorldStatic는 서로 Block
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//1.상하,좌우 입력 값을 이용한 방향 벡터
	FVector dir = FVector(0, h, v);
	//2.방향 벡터의 길이가 1이 되도록 정규화(nomalize)
	dir.Normalize();
	//3.이동할 위치 좌표
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	//4.액터의 위치좌표 갱신
	SetActorLocation(newLocation, true);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Axis 바인딩 된 값을 함수에 연결
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawn::MoveVertical);

	//Action 바인딩 된 값을 함수에 연결
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPawn::Fire);
}
//좌우축 입력 함수
void APlayerPawn::MoveHorizontal(float value)
{
	h = value;
}
//상하축 입력 함수
void APlayerPawn::MoveVertical(float value)
{
	v = value;
}
//총알 발사 함수
void APlayerPawn::Fire()
{
	//총알 스폰
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, 
		firePosition->GetComponentLocation(), firePosition->GetComponentRotation());
	//음원파일 실행
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}

