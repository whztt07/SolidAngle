// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Misc/OutputDeviceAnsiError.h"
#include "Templates/SolidAngleTemplate.h"
#include "Logging/LogMacros.h"
#include "CoreGlobals.h"
#include "Misc/OutputDeviceRedirector.h"
#include "Misc/CoreDelegates.h"
#include "Misc/App.h"

/** Constructor, initializing member variables */
YOutputDeviceAnsiError::YOutputDeviceAnsiError()
:	ErrorPos(0)
{}

/**
 * Serializes the passed in data unless the current event is suppressed.
 *
 * @param	Data	Text to log
 * @param	Event	Event name used for suppression purposes
 */
void YOutputDeviceAnsiError::Serialize( const TCHAR* Msg, ELogVerbosity::Type Verbosity, const class YName& Category )
{
	// Display the error and exit.
	YPlatformMisc::LocalPrint( TEXT("\nappError called: \n") );
	YPlatformMisc::LocalPrint( Msg );
	YPlatformMisc::LocalPrint( TEXT("\n") );

	if( !GIsCriticalError )
	{
		// First appError.
		GIsCriticalError = 1;
		UE_LOG(LogHAL, Error, TEXT("appError called: %s"), Msg );
		FCString::Strncpy( GErrorHist, Msg, ARRAY_COUNT(GErrorHist) );
		FCString::Strncat( GErrorHist, TEXT("\r\n\r\n"), ARRAY_COUNT(GErrorHist) );
		ErrorPos = FCString::Strlen(GErrorHist);
	}
	else
	{
		UE_LOG(LogHAL, Error, TEXT("Error reentered: %s"), Msg );
	}

	YPlatformMisc::DebugBreak();

	if( GIsGuarded )
	{
		// Propagate error so structured exception handler can perform necessary work.
#if PLATFORM_EXCEPTIONS_DISABLED
		YPlatformMisc::DebugBreak();
#endif
		YPlatformMisc::RaiseException( 1 );
	}
	else
	{
		// We crashed outside the guarded code (e.g. appExit).
		HandleError();
		// pop up a crash window if we are not in unattended mode
		if( FApp::IsUnattended() == false )
		{
			YPlatformMisc::RequestExit( true );
		}
		else
		{
			UE_LOG(LogHAL, Error, TEXT("%s"), Msg );
		}		
	}
}

/**
 * Error handling function that is being called from within the system wide global
 * error handler, e.g. using structured exception handling on the PC.
 */
void YOutputDeviceAnsiError::HandleError()
{
	GIsGuarded			= 0;
	GIsRunning			= 0;
	GIsCriticalError	= 1;
	GLogConsole			= NULL;
	GErrorHist[ARRAY_COUNT(GErrorHist)-1]=0;

	if (GLog)
	{
		// print to log and flush it
		UE_LOG( LogHAL, Log, TEXT( "=== Critical error: ===" ) LINE_TERMINATOR LINE_TERMINATOR TEXT( "%s" ) LINE_TERMINATOR, GErrorExceptionDescription );
		UE_LOG(LogHAL, Log, GErrorHist);

		GLog->Flush();
	}
	else
	{
		YPlatformMisc::LocalPrint( GErrorHist );
	}

	YPlatformMisc::LocalPrint( TEXT("\n\nExiting due to error\n") );

	FCoreDelegates::OnShutdownAfterError.Broadcast();
}
