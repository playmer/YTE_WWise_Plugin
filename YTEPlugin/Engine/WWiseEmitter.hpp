#pragma once

#ifndef YTE_WWise_Plugin_WWiseEmitter_h
#define YTE_WWise_Plugin_WWiseEmitter_h

#include "YTE/Core/Component.hpp"

#include "YTE/StandardLibrary/PrivateImplementation.hpp"

#include "YTEPlugin/Engine/ForwardDeclarations.hpp"
#include "YTEPlugin/Engine/WWiseMeta.hpp"
#include "YTEPlugin/Engine/WWiseView.hpp"


namespace YTEPlugin::WWise
{
  class WWiseEmitter : public YTE::Component
  {
  public:
    YTEWWiseDeclareType(WWiseEmitter);

    YTEPlugin_WWise_Shared WWiseEmitter(YTE::Composition* aOwner, YTE::Space* aSpace);

    YTEPlugin_WWise_Shared ~WWiseEmitter() override;
    
    YTEPlugin_WWise_Shared void NativeInitialize() override;
    YTEPlugin_WWise_Shared void Initialize() override;

    YTEPlugin_WWise_Shared void Play();
    YTEPlugin_WWise_Shared void PlayEvent(const std::string & aEvent);
    YTEPlugin_WWise_Shared void PlayEvent(YTE::u64 aEvent);

    YTEPlugin_WWise_Shared void ListenerChanged(WWiseListenerChanged *aListenerChange);

    inline WwiseObject OwnerId() { return reinterpret_cast<WwiseObject>(mOwner); };

  private:
    void SetEmitterPosition();
    void OnPositionChange(const YTE::TransformChanged *aEvent);
    void OnOrientationChange(const YTE::OrientationChanged *aEvent);

    YTE::PrivateImplementationLocal<128> mEmitterPosition;
    int mListenerId;

    std::string mSound;
    float mVolume;
  };
}

#endif
