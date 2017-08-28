// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ENGINE_SlateTextureAtlasInterface_generated_h
#error "SlateTextureAtlasInterface.generated.h already included, missing '#pragma once' in SlateTextureAtlasInterface.h"
#endif
#define ENGINE_SlateTextureAtlasInterface_generated_h

#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_RPC_WRAPPERS
#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_RPC_WRAPPERS_NO_PURE_DECLS
#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USlateTextureAtlasInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USlateTextureAtlasInterface) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USlateTextureAtlasInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USlateTextureAtlasInterface); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USlateTextureAtlasInterface(USlateTextureAtlasInterface&&); \
	NO_API USlateTextureAtlasInterface(const USlateTextureAtlasInterface&); \
public:


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USlateTextureAtlasInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USlateTextureAtlasInterface(USlateTextureAtlasInterface&&); \
	NO_API USlateTextureAtlasInterface(const USlateTextureAtlasInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USlateTextureAtlasInterface); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USlateTextureAtlasInterface); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USlateTextureAtlasInterface)


#undef GENERATED_UINTERFACE_BODY_COMMON
#define GENERATED_UINTERFACE_BODY_COMMON() \
	private: \
	static void StaticRegisterNativesUSlateTextureAtlasInterface(); \
	friend ENGINE_API class UClass* Z_Construct_UClass_USlateTextureAtlasInterface(); \
public: \
	DECLARE_CLASS(USlateTextureAtlasInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), 0, TEXT("/Script/Engine"), NO_API) \
	DECLARE_SERIALIZER(USlateTextureAtlasInterface) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_GENERATED_BODY_LEGACY \
		PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	GENERATED_UINTERFACE_BODY_COMMON() \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_STANDARD_CONSTRUCTORS \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	GENERATED_UINTERFACE_BODY_COMMON() \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~ISlateTextureAtlasInterface() {} \
public: \
	typedef USlateTextureAtlasInterface UClassType; \
	virtual UObject* _getUObject() const = 0;


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_INCLASS_IINTERFACE \
protected: \
	virtual ~ISlateTextureAtlasInterface() {} \
public: \
	typedef USlateTextureAtlasInterface UClassType; \
	virtual UObject* _getUObject() const = 0;


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_54_PROLOG
#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_65_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_RPC_WRAPPERS \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_INCLASS_IINTERFACE \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_65_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_RPC_WRAPPERS_NO_PURE_DECLS \
	Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h_57_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Engine_Source_Runtime_Engine_Public_Slate_SlateTextureAtlasInterface_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS