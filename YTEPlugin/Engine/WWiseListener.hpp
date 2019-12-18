#pragma once

#ifndef YTE_WWise_Plugin_WWiseListener_h
#define YTE_WWise_Plugin_WWiseListener_h

#include "YTE/Core/Component.hpp"

#include "YTE/Physics/Transform.hpp"

#include "YTE/StandardLibrary/PrivateImplementation.hpp"

#include "YTEPlugin/Engine/ForwardDeclarations.hpp"
#include "YTEPlugin/Engine/WWiseMeta.hpp"

namespace YTEPlugin::WWise
{
  class WWiseListener : public YTE::Component
  {
  public:
    YTEWWiseDeclareType(WWiseListener);

    YTEPlugin_WWise_Shared WWiseListener(YTE::Composition* aOwner, YTE::Space* aSpace);
    
    YTEPlugin_WWise_Shared ~WWiseListener() override;

    YTEPlugin_WWise_Shared void Initialize() override;

    WwiseObject OwnerId() { return reinterpret_cast<WwiseObject>(this); };

  private:
    void SetListenerPosition();
    void OnPositionChange(const YTE::TransformChanged *aEvent);
    void OnOrientationChange(const YTE::OrientationChanged *aEvent);

    YTE::PrivateImplementationLocal<128> mListenerPosition;
  };
}

#endif
