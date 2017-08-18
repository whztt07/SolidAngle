// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GenericPlatform/GenericApplication.h"
#include "HAL/IConsoleManager.h"

const FGamepadKeFNames::Type FGamepadKeFNames::Invalid(NAME_None);

// Ensure that the GamepadKeFNames match those in InputCoreTypes.cpp
const FGamepadKeFNames::Type FGamepadKeFNames::LeftAnalogX("Gamepad_LeftX");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftAnalogY("Gamepad_LeftY");
const FGamepadKeFNames::Type FGamepadKeFNames::RightAnalogX("Gamepad_RightX");
const FGamepadKeFNames::Type FGamepadKeFNames::RightAnalogY("Gamepad_RightY");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftTriggerAnalog("Gamepad_LeftTriggerAxis");
const FGamepadKeFNames::Type FGamepadKeFNames::RightTriggerAnalog("Gamepad_RightTriggerAxis");

const FGamepadKeFNames::Type FGamepadKeFNames::LeftThumb("Gamepad_LeftThumbstick");
const FGamepadKeFNames::Type FGamepadKeFNames::RightThumb("Gamepad_RightThumbstick");
const FGamepadKeFNames::Type FGamepadKeFNames::SpecialLeft("Gamepad_Special_Left");
const FGamepadKeFNames::Type FGamepadKeFNames::SpecialLeft_X("Gamepad_Special_Left_X");
const FGamepadKeFNames::Type FGamepadKeFNames::SpecialLeft_Y("Gamepad_Special_Left_Y");
const FGamepadKeFNames::Type FGamepadKeFNames::SpecialRight("Gamepad_Special_Right");
const FGamepadKeFNames::Type FGamepadKeFNames::FaceButtonBottom("Gamepad_FaceButton_Bottom");
const FGamepadKeFNames::Type FGamepadKeFNames::FaceButtonRight("Gamepad_FaceButton_Right");
const FGamepadKeFNames::Type FGamepadKeFNames::FaceButtonLeft("Gamepad_FaceButton_Left");
const FGamepadKeFNames::Type FGamepadKeFNames::FaceButtonTop("Gamepad_FaceButton_Top");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftShoulder("Gamepad_LeftShoulder");
const FGamepadKeFNames::Type FGamepadKeFNames::RightShoulder("Gamepad_RightShoulder");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftTriggerThreshold("Gamepad_LeftTrigger");
const FGamepadKeFNames::Type FGamepadKeFNames::RightTriggerThreshold("Gamepad_RightTrigger");
const FGamepadKeFNames::Type FGamepadKeFNames::DPadUp("Gamepad_DPad_Up");
const FGamepadKeFNames::Type FGamepadKeFNames::DPadDown("Gamepad_DPad_Down");
const FGamepadKeFNames::Type FGamepadKeFNames::DPadRight("Gamepad_DPad_Right");
const FGamepadKeFNames::Type FGamepadKeFNames::DPadLeft("Gamepad_DPad_Left");

const FGamepadKeFNames::Type FGamepadKeFNames::LeftStickUp("Gamepad_LeftStick_Up");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftStickDown("Gamepad_LeftStick_Down");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftStickRight("Gamepad_LeftStick_Right");
const FGamepadKeFNames::Type FGamepadKeFNames::LeftStickLeft("Gamepad_LeftStick_Left");

const FGamepadKeFNames::Type FGamepadKeFNames::RightStickUp("Gamepad_RightStick_Up");
const FGamepadKeFNames::Type FGamepadKeFNames::RightStickDown("Gamepad_RightStick_Down");
const FGamepadKeFNames::Type FGamepadKeFNames::RightStickRight("Gamepad_RightStick_Right");
const FGamepadKeFNames::Type FGamepadKeFNames::RightStickLeft("Gamepad_RightStick_Left");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton1("MotionController_Left_FaceButton1");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton2("MotionController_Left_FaceButton2");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton3("MotionController_Left_FaceButton3");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton4("MotionController_Left_FaceButton4");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton5("MotionController_Left_FaceButton5");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton6("MotionController_Left_FaceButton6");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton7("MotionController_Left_FaceButton7");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_FaceButton8("MotionController_Left_FaceButton8");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Shoulder("MotionController_Left_Shoulder");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Trigger("MotionController_Left_Trigger");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Grip1("MotionController_Left_Grip1");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Grip2("MotionController_Left_Grip2");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick("MotionController_Left_Thumbstick");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_Up("MotionController_Left_Thumbstick_Up");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_Down("MotionController_Left_Thumbstick_Down");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_Left("MotionController_Left_Thumbstick_Left");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_Right("MotionController_Left_Thumbstick_Right");

	//		Right Controller
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton1("MotionController_Right_FaceButton1");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton2("MotionController_Right_FaceButton2");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton3("MotionController_Right_FaceButton3");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton4("MotionController_Right_FaceButton4");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton5("MotionController_Right_FaceButton5");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton6("MotionController_Right_FaceButton6");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton7("MotionController_Right_FaceButton7");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_FaceButton8("MotionController_Right_FaceButton8");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Shoulder("MotionController_Right_Shoulder");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Trigger("MotionController_Right_Trigger");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Grip1("MotionController_Right_Grip1");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Grip2("MotionController_Right_Grip2");

const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick("MotionController_Right_Thumbstick");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_Up("MotionController_Right_Thumbstick_Up");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_Down("MotionController_Right_Thumbstick_Down");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_Left("MotionController_Right_Thumbstick_Left");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_Right("MotionController_Right_Thumbstick_Right");

	//   Motion Controller Axes
	//		Left Controller
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_X("MotionController_Left_Thumbstick_X");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Thumbstick_Y("MotionController_Left_Thumbstick_Y");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_TriggerAxis("MotionController_Left_TriggerAxis");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Grip1Axis( "MotionController_Left_Grip1Axis" );
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Left_Grip2Axis( "MotionController_Left_Grip2Axis" );

	//		Right Controller
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_X("MotionController_Right_Thumbstick_X");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Thumbstick_Y("MotionController_Right_Thumbstick_Y");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_TriggerAxis("MotionController_Right_TriggerAxis");
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Grip1Axis( "MotionController_Right_Grip1Axis" );
const FGamepadKeFNames::Type FGamepadKeFNames::MotionController_Right_Grip2Axis( "MotionController_Right_Grip2Axis" );

float GDebugSafeZoneRatio = 1.0f;
float GDebugActionZoneRatio = 1.0f;

FAutoConsoleVariableRef GDebugSafeZoneRatioCVar(
	TEXT("r.DebugSafeZone.TitleRatio"),
	GDebugSafeZoneRatio,
	TEXT("The safe zone ratio that will be returned by FDisplayMetrics::GetDisplayMetrics on platforms that don't have a defined safe zone (0..1)\n")
	TEXT(" default: 1.0"));

FAutoConsoleVariableRef GDebugActionZoneRatioCVar(
	TEXT("r.DebugActionZone.ActionRatio"),
	GDebugActionZoneRatio,
	TEXT("The action zone ratio that will be returned by FDisplayMetrics::GetDisplayMetrics on platforms that don't have a defined safe zone (0..1)\n")
	TEXT(" default: 1.0"));

float FDisplayMetrics::GetDebugTitleSafeZoneRatio()
{
	return GDebugSafeZoneRatio;
}

float FDisplayMetrics::GetDebugActionSafeZoneRatio()
{
	return GDebugActionZoneRatio;
}

void FDisplayMetrics::ApplyDefaultSafeZones()
{
	const float SafeZoneRatio = GetDebugTitleSafeZoneRatio();
	if (SafeZoneRatio < 1.0f)
	{
		const float HalfUnsafeRatio = (1.0f - SafeZoneRatio) * 0.5f;
		TitleSafePaddingSize = YVector2D(PrimaryDisplayWidth * HalfUnsafeRatio, PrimaryDisplayHeight * HalfUnsafeRatio);
	}

	const float ActionSafeZoneRatio = GetDebugActionSafeZoneRatio();
	if (ActionSafeZoneRatio < 1.0f)
	{
		const float HalfUnsafeRatio = (1.0f - ActionSafeZoneRatio) * 0.5f;
		ActionSafePaddingSize = YVector2D(PrimaryDisplayWidth * HalfUnsafeRatio, PrimaryDisplayHeight * HalfUnsafeRatio);
	}
}

void FDisplayMetrics::PrintToLog() const
{
	UE_LOG(LogInit, Log, TEXT("Display metrics:"));
	UE_LOG(LogInit, Log, TEXT("  PrimaryDisplayWidth: %d"), PrimaryDisplayWidth);
	UE_LOG(LogInit, Log, TEXT("  PrimaryDisplayHeight: %d"), PrimaryDisplayHeight);
	UE_LOG(LogInit, Log, TEXT("  PrimaryDisplayWorkAreaRect:"));
	UE_LOG(LogInit, Log, TEXT("    Left=%d, Top=%d, Right=%d, Bottom=%d"), 
		PrimaryDisplayWorkAreaRect.Left, PrimaryDisplayWorkAreaRect.Top, 
		PrimaryDisplayWorkAreaRect.Right, PrimaryDisplayWorkAreaRect.Bottom);

	UE_LOG(LogInit, Log, TEXT("  VirtualDisplayRect:"));
	UE_LOG(LogInit, Log, TEXT("    Left=%d, Top=%d, Right=%d, Bottom=%d"), 
		VirtualDisplayRect.Left, VirtualDisplayRect.Top, 
		VirtualDisplayRect.Right, VirtualDisplayRect.Bottom);

	UE_LOG(LogInit, Log, TEXT("  TitleSafePaddingSize: %s"), *TitleSafePaddingSize.ToString());
	UE_LOG(LogInit, Log, TEXT("  ActionSafePaddingSize: %s"), *ActionSafePaddingSize.ToString());

	const int NumMonitors = MonitorInfo.Num();
	UE_LOG(LogInit, Log, TEXT("  Number of monitors: %d"), NumMonitors);

	for (int MonitorIdx = 0; MonitorIdx < NumMonitors; ++MonitorIdx)
	{
		const FMonitorInfo & Info = MonitorInfo[MonitorIdx];
		UE_LOG(LogInit, Log, TEXT("    Monitor %d"), MonitorIdx);
		UE_LOG(LogInit, Log, TEXT("      Name: %s"), *Info.Name);
		UE_LOG(LogInit, Log, TEXT("      ID: %s"), *Info.ID);
		UE_LOG(LogInit, Log, TEXT("      NativeWidth: %d"), Info.NativeWidth);
		UE_LOG(LogInit, Log, TEXT("      NativeHeight: %d"), Info.NativeHeight);
		UE_LOG(LogInit, Log, TEXT("      bIsPrimary: %s"), Info.bIsPrimary ? TEXT("true") : TEXT("false"));
	}
}
