
#pragma once

#include "CoreMinimal.h"


#define PrintLine(){ CLog::Log(__FUNCTION__,__LINE__); }

/**
 * 
 */
	//int 32 ��� ȯ�濡���� 4����Ʈ�� ����� 32��Ʈ
class UNREALC_ACTION_API CLog
{
public:

	//InPlayȭ�鿡 ����
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);

	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);

};
