// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Math/SolidAngleMathUtility.h"
#include "Math/Plane.h"
#include "Math/Matrix.h"

/** Inverse Rotation matrix */
class FInverseRotationMatrix
	: public YMatrix
{
public:
	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 */
	FInverseRotationMatrix(const YRotator& Rot);
};


FORCEINLINE FInverseRotationMatrix::FInverseRotationMatrix(const YRotator& Rot)
	: YMatrix(
		YMatrix( // Yaw
		YPlane(+YMath::Cos(Rot.Yaw * PI / 180.f), -YMath::Sin(Rot.Yaw * PI / 180.f), 0.0f, 0.0f),
		YPlane(+YMath::Sin(Rot.Yaw * PI / 180.f), +YMath::Cos(Rot.Yaw * PI / 180.f), 0.0f, 0.0f),
		YPlane(0.0f, 0.0f, 1.0f, 0.0f),
		YPlane(0.0f, 0.0f, 0.0f, 1.0f)) *
		YMatrix( // Pitch
		YPlane(+YMath::Cos(Rot.Pitch * PI / 180.f), 0.0f, -YMath::Sin(Rot.Pitch * PI / 180.f), 0.0f),
		YPlane(0.0f, 1.0f, 0.0f, 0.0f),
		YPlane(+YMath::Sin(Rot.Pitch * PI / 180.f), 0.0f, +YMath::Cos(Rot.Pitch * PI / 180.f), 0.0f),
		YPlane(0.0f, 0.0f, 0.0f, 1.0f)) *
		YMatrix( // Roll
		YPlane(1.0f, 0.0f, 0.0f, 0.0f),
		YPlane(0.0f, +YMath::Cos(Rot.Roll * PI / 180.f), +YMath::Sin(Rot.Roll * PI / 180.f), 0.0f),
		YPlane(0.0f, -YMath::Sin(Rot.Roll * PI / 180.f), +YMath::Cos(Rot.Roll * PI / 180.f), 0.0f),
		YPlane(0.0f, 0.0f, 0.0f, 1.0f))
	)
{ }
