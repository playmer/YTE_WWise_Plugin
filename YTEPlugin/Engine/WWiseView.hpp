#pragma once

#ifndef YTE_WWise_WWiseView_hpp
#define YTE_WWise_WWiseView_hpp

#include "YTE/Core/Composition.hpp"
#include "YTE/Core/ForwardDeclarations.hpp"
#include "YTE/Core/Engine.hpp"

#include "YTE/Platform/Keyboard.hpp"
#include "YTE/Platform/DeviceEnums.hpp"

#include "YTE/Utilities/Utilities.hpp"

#include "YTEPlugin/Engine/ForwardDeclarations.hpp"
#include "YTEPlugin/Engine/WWiseListener.hpp"
#include "YTEPlugin/Engine/WWiseMeta.hpp"

namespace YTEPlugin::WWise
{
  class WWiseListenerChanged : public YTE::Event
  {
  public:
    YTEWWiseDeclareType(WWiseListenerChanged);
    WWiseListener *NewListener;
  };

  YTEWWiseDeclareEvent(WWiseListenerChanged);

  class WWiseView : public YTE::Component
  {
  public:
    YTEWWiseDeclareType(WWiseView);
    WWiseView(YTE::Composition* aOwner, YTE::Space* aSpace)
      : Component(aOwner, aSpace)
    {
      
    }

    WWiseListener* GetActiveListener()
    {
      return mLastActiveListener;
    }

    void SetActiveListener(WWiseListener* aListener)
    {
      mLastActiveListener = aListener;

      WWiseListenerChanged event;
      event.NewListener = aListener;

      SendEvent(Events::WWiseListenerChanged, &event);
    }

  private:
    WWiseListener* mLastActiveListener;
  };

}

#endif