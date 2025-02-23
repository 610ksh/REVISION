// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Components/ActorComponent.h"
#include "GSCharacterStatComponent.generated.h"

/*
	캐릭터 레벨 정보에서 HP 정보까지 모든 스탯을 스탯 컴포넌트 (해당 클래스)에서 관리함.
	- struct FGSCharacterData* 변수 필요.
	- 데이터를 관리하는 변수들은 private로 한정해 선언한다.
	- 레벨은 SetNewLevel 함수를 통해서만 변경할 수 있도록 설계한다.
	- 게임 인스턴스에서 데이터를 가져와 초기화하고 레벨이 변경되면 해당 스탯이 바뀌도록 제작한다.
	
*/

// 멀리캐스트 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate); // Dead 판정을 위한 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate); // UI 위젯을 위한 델리게이트(->GSCharacterWidget)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTDARK_API UGSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGSCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// 미리 만들어진 이니셜라이즈 컴포넌트 오버라이드
	virtual void InitializeComponent() override;

public:	
	// Called every frame (사용안함)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 새로운 레벨 설정
	void SetNewLevel(int32 NewLevel);
	// 캐릭터의 TakeDamage 함수에서 이를 호출해서 데미지 상호작용을 처리
	void SetDamage(float NewDamage);
	// HP 설정
	void SetHP(float NewHP);
	// 데미지 크기 반환
	float GetAttack();
	// 현재 HP 비율 반환
	float GetHPRatio();
	// 경험치 드랍
	UFUNCTION(BlueprintCallable, Category = Stat)
	int32 GetDropExp() const;

	UFUNCTION(BlueprintCallable, Category = Stat)
		float MaxHP() const;

	// 델리게이트 변수
	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangeDelegate OnHPChanged;

private:
	// 캐릭터 데이터 테이블 구조체 참조 변수. 정확히 말하자면, 하나의 행(Row) 구조체 정보를 가져오는 변수. ex) Level 4의 데이터 정보들
	struct FGSCharacterData* CurrentStatData = nullptr;

	// 레벨 변수
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	// 현재 HP / Transient는 UPROPERTY 속성을 저장하지 않는다는걸 의미한다. 수시로 변할수 있는 데이터이기에 보관하는것이 의미 없음.
	UPROPERTY(Transient, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		bool IsInitialHP = false;
};