// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Math/SolidAngleMathUtility.h"
#include "Math/Matrix.h"

/** Combined Scale rotation and translation matrix */
class YScaleRotationTranslationMatrix
	: public FMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Scale scale to apply to matrix
	 * @param Rot rotation
	 * @param Origin translation to apply
	 */
	YScaleRotationTranslationMatrix(const FVector& Scale, const FRotator& Rot, const FVector& Origin);
};


FORCEINLINE YScaleRotationTranslationMatrix::YScaleRotationTranslationMatrix(const FVector& Scale, const FRotator& Rot, const FVector& Origin)
{
	float SP, SY, SR;
	float CP, CY, CR;
	YMath::SinCos(&SP, &CP, YMath::DegreesToRadians(Rot.Pitch));
	YMath::SinCos(&SY, &CY, YMath::DegreesToRadians(Rot.Yaw));
	YMath::SinCos(&SR, &CR, YMath::DegreesToRadians(Rot.Roll));

	M[0][0]	= (CP * CY) * Scale.X;
	M[0][1]	= (CP * SY) * Scale.X;
	M[0][2]	= (SP) * Scale.X;
	M[0][3]	= 0.f;

	M[1][0]	= (SR * SP * CY - CR * SY) * Scale.Y;
	M[1][1]	= (SR * SP * SY + CR * CY) * Scale.Y;
	M[1][2]	= (- SR * CP) * Scale.Y;
	M[1][3]	= 0.f;

	M[2][0]	= ( -( CR * SP * CY + SR * SY ) ) * Scale.Z;
	M[2][1]	= (CY * SR - CR * SP * SY) * Scale.Z;
	M[2][2]	= (CR * CP) * Scale.Z;
	M[2][3]	= 0.f;

	M[3][0]	= Origin.X;
	M[3][1]	= Origin.Y;
	M[3][2]	= Origin.Z;
	M[3][3]	= 1.f;
}
