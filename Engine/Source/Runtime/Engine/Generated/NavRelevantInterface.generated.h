// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ENGINE_NavRelevantInterface_generated_h
#error "NavRelevantInterface.generated.h already included, missing '#pragma once' in NavRelevantInterface.h"
#endif
#define ENGINE_NavRelevantInterface_generated_h

#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_RPC_WRAPPERS
#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_RPC_WRAPPERS_NO_PURE_DECLS
#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ENGINE_API UNavRelevantInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UNavRelevantInterface) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ENGINE_API, UNavRelevantInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNavRelevantInterface); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ENGINE_API UNavRelevantInterface(UNavRelevantInterface&&); \
	ENGINE_API UNavRelevantInterface(const UNavRelevantInterface&); \
public:


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ENGINE_API UNavRelevantInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ENGINE_API UNavRelevantInterface(UNavRelevantInterface&&); \
	ENGINE_API UNavRelevantInterface(const UNavRelevantInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ENGINE_API, UNavRelevantInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNavRelevantInterface); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UNavRelevantInterface)


#undef GENERATED_UINTERFACE_BODY_COMMON
#define GENERATED_UINTERFACE_BODY_COMMON() \
	private: \
	static void StaticRegisterNativesUNavRelevantInterface(); \
	friend ENGINE_API class UClass* Z_Construct_UClass_UNavRelevantInterface(); \
public: \
	DECLARE_CLASS(UNavRelevantInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), 0, TEXT("/Script/Engine"), ENGINE_API) \
	DECLARE_SERIALIZER(UNavRelevantInterface) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_GENERATED_BODY_LEGACY \
		PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	GENERATED_UINTERFACE_BODY_COMMON() \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_STANDARD_CONSTRUCTORS \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	GENERATED_UINTERFACE_BODY_COMMON() \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~INavRelevantInterface() {} \
public: \
	typedef UNavRelevantInterface UClassType; \
	virtual UObject* _getUObject() const = 0;


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_INCLASS_IINTERFACE \
protected: \
	virtual ~INavRelevantInterface() {} \
public: \
	typedef UNavRelevantInterface UClassType; \
	virtual UObject* _getUObject() const = 0;


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_14_PROLOG
#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_22_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_RPC_WRAPPERS \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_INCLASS_IINTERFACE \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h_17_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Engine_Source_Runtime_Engine_Classes_AI_Navigation_NavRelevantInterface_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS