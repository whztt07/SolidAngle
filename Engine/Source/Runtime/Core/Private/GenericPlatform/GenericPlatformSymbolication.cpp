// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GenericPlatform/GenericPlatformSymbolication.h"
#include "Logging/LogMacros.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"


DEFINE_LOG_CATEGORY_STATIC(LogGenericPlatformSymbolication, Log, All);

bool FGenericPlatformSymbolication::LoadSymbolDatabaseForBinary(YString SourceFolder, YString Binary, YString ModuleSignature, FGenericPlatformSymbolDatabase& OutDatabase)
{
	bool bOk = false;
	
	YString ModuleName = YPaths::GetBaseFilename(Binary);
	YString InputFile = (SourceFolder / ModuleName) + TEXT(".udebugsymbols");
	if (IFileManager::Get().FileSize(*InputFile) > 0)
	{
		TArray<uint8> DataBuffer;
		if(FFileHelper::LoadFileToArray(DataBuffer, *InputFile))
		{
			YArchiveLoadCompressedProxy DataArchive(DataBuffer, (ECompressionFlags)(COMPRESS_Default));
			DataArchive << OutDatabase;
			if(!DataArchive.GetError())
			{
				if(OutDatabase.Signature == ModuleSignature)
				{
					bOk = true;
				}
			}
		}
	}
	
	return bOk;
}

bool FGenericPlatformSymbolication::SaveSymbolDatabaseForBinary(YString TargetFolder, YString Name, FGenericPlatformSymbolDatabase& Database)
{
	bool bOk = false;
	
	YString ModuleName = YPaths::GetBaseFilename(Name);
	YString OutputFile = (TargetFolder / ModuleName) + TEXT(".udebugsymbols");

	TArray<uint8> DataBuffer;
	YArchiveSaveCompressedProxy DataArchive(DataBuffer, (ECompressionFlags)(COMPRESS_ZLIB|COMPRESS_BiasSpeed));
	DataArchive << Database;
	DataArchive.Flush();
	if(!DataArchive.GetError())
	{
		if(FFileHelper::SaveArrayToFile(DataBuffer, *OutputFile))
		{
			bOk = true;
		}
		else
		{
			UE_LOG( LogGenericPlatformSymbolication, Error, TEXT( "Unable to write debug symbols to output file." ) );
		}
	}
	else
	{
		UE_LOG( LogGenericPlatformSymbolication, Error, TEXT( "Unable to serialise debug symbols." ) );
	}
	
	return bOk;
}

bool FGenericPlatformSymbolication::SymbolInfoForStrippedSymbol(FGenericPlatformSymbolDatabase const& Database, uint64 ProgramCounter, uint64 ModuleOffset, YString ModuleSignature, FProgramCounterSymbolInfo& Info)
{
	bool bOk = false;
	
	if(Database.Signature == ModuleSignature)
	{
		for(auto Symbol : Database.Symbols)
		{
			if((Symbol.Start <= (ProgramCounter - ModuleOffset)) && ((Symbol.Start + Symbol.Length) >= (ProgramCounter - ModuleOffset)))
			{
				FCStringAnsi::Strncpy(Info.ModuleName, TCHAR_TO_ANSI(*Database.Name), FProgramCounterSymbolInfo::MAX_NAME_LENGHT);
				
				YString SymbolName = Database.StringTable[Symbol.NameIdx];
				FCStringAnsi::Strncpy(Info.FunctionName, TCHAR_TO_ANSI(*SymbolName), FProgramCounterSymbolInfo::MAX_NAME_LENGHT);
				
				Info.ProgramCounter = ProgramCounter;
				Info.OffsetInModule = Symbol.Start;
				
				Info.LineNumber = -1;
				
				for(auto SymbolInfo : Symbol.SymbolInfo)
				{
					if((SymbolInfo.Start <= (ProgramCounter - ModuleOffset)) && ((SymbolInfo.Start + SymbolInfo.Length) >= (ProgramCounter - ModuleOffset)))
					{
						YString Path = Database.StringTable[SymbolInfo.PathIdx];
						FCStringAnsi::Strncpy(Info.Filename, TCHAR_TO_ANSI(*Path), FProgramCounterSymbolInfo::MAX_NAME_LENGHT);
						Info.LineNumber = SymbolInfo.Line;
						break;
					}
				}
				
				bOk = true;
			
				break;
			}
		}
	}
	
	return bOk;
}
