#pragma once

#include "YTE/Platform/TargetDefinitions.hpp"

#if YTE_Windows
  #if defined(YTEPlugin_WWise_Internal)
    #define YTEPlugin_WWise_Shared __declspec( dllexport )
  #else
    #define YTEPlugin_WWise_Shared __declspec( dllimport )
  #endif
#elif YTE_Linux
  #if defined(YTEPlugin_WWise_Internal)
    #define YTEPlugin_WWise_Shared __attribute__((visibility("default")))
  #else
    #define YTEPlugin_WWise_Shared
  #endif
#endif


#define YTEWWiseDeclareType(Name)                                       \
void Dummy() {}                                                           \
typedef decltype(::YTE::GetDummy(&Name::Dummy)) TempSelfType;             \
typedef decltype(::YTE::GetSelfType<TempSelfType>(nullptr)) BaseType;     \
typedef TempSelfType SelfType;                                            \
YTEPlugin_WWise_Shared static ::YTE::Type sType;                               \
YTEPlugin_WWise_Shared static ::YTE::Type* GetStaticType() { return &sType; }; \
YTEPlugin_WWise_Shared ::YTE::Type* GetType() { return &sType; };              \
YTEPlugin_WWise_Shared static void InitializeType();


#define YTEWWiseDefineType(Name)                       \
::YTE::Type Name::sType{#Name,                           \
                 static_cast<Name*>(nullptr),            \
                 static_cast<Name::BaseType*>(nullptr)}; \
void Name::InitializeType()

#define YTEWWiseDeclareEvent(aName)                   \
  namespace Events                                      \
  {                                                     \
      YTEPlugin_WWise_Shared extern const std::string aName; \
  }

#define YTEWWiseDefineEvent(aName)    \
  namespace Events                      \
  {                                     \
      const std::string aName = #aName; \
  }
  