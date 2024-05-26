// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NYOTA_API Debug
{
public:

	//** Print on screen **// 
	//** 在世界中打印 **//
	static void SLOG(const FString& DebugMessage)  {

		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, DebugMessage);
	};

	//** Draw point in the world **// 
	//** 在世界重画点 **//
	void POINT(UObject* Target, FVector const& Position, FColor const& PointColor) {

		DrawDebugPoint(Target->GetWorld(), Position, 10.f, PointColor, 1);
	}


	//** Draw Line in the world **// 
	//** 在世界重画线 **//
	static void LINE(UObject* Target, FVector const& LineStart, FVector const& LineEnd, FColor const& PointColor) {

		DrawDebugLine(Target->GetWorld(), LineStart, LineEnd, PointColor, 1);
	}

	//** Draw Line in the world **// 
	//** 在世界重画胶囊体 **//
	static void CAPSULE(UObject* Target, FVector const& Center, float HalfHeight, float Radius, FColor const& PointColor) {

		DrawDebugCapsule(Target->GetWorld(), Center, HalfHeight, Radius, FQuat::Identity, PointColor, 1);
	}
};
