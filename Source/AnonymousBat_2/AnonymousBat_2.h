// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(AnonymousBat_2, Log, All);

// �Լ���, �ش������ġ
#define AB2LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// UE_LOG�� ��� ���� ��� �Ʒ�ó�� ������ ����� �� �ִ�. 
// Verbosity�� Error, Warning, Log���� �α� ���� ���� ����
#define AB2LOG_S(Verbosity) UE_LOG(AnonymousBat_2, Verbosity, TEXT("%s"), *AB2LOG_CALLINFO)

// �߰��� �� ����ϰ� ���� ���� ������ �Ʒ�ó�� ���(���� ����)
#define AB2LOG(Verbosity, Format, ...) UE_LOG(AnonymousBat_2, Verbosity, TEXT("%s%s"), *AB2LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// Expr ������ ���� �ƴϸ� Error �αװ� �߻��� �Ӱ� �߰� �ȴ�.
#define AB2CHECK(Expr, ...)	{ if(!(Expr)) {AB2LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

