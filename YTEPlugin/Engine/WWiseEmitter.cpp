#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/SoundEngine/Common/AkTypes.h"

#include "YTE/Core/Engine.hpp"
#include "YTE/Core/Space.hpp"

#include "YTE/Physics/Orientation.hpp"
#include "YTE/Physics/Transform.hpp"

#include "YTEPlugin/Engine/Utilities.hpp"
#include "YTEPlugin/Engine/WWiseEmitter.hpp"
#include "YTEPlugin/Engine/WWiseListener.hpp"
#include "YTEPlugin/Engine/WWiseSystem.hpp"

namespace YTEPlugin::WWise
{
  using namespace YTE;

  YTEWWiseDefineType(WWiseEmitter)
  {
    RegisterType<WWiseEmitter>();
    TypeBuilder<WWiseEmitter> builder;
    

    std::vector<std::vector<YTE::Type*>> deps = { { TypeId<Transform>() }, 
                                                  { TypeId<Orientation>() } };

    GetStaticType()->AddAttribute<ComponentDependencies>(deps);

    builder.Field<&WWiseEmitter::mSound>("Sound", PropertyBinding::GetSet)
      .AddAttribute<Serializable>()
      .AddAttribute<EditorProperty>();

    builder.Function<&WWiseEmitter::Play>("Play");
    builder.Function<SelectOverload<void (WWiseEmitter::*)(const std::string&)>(&WWiseEmitter::PlayEvent)>("PlaySound")
      .SetParameterNames("aSound");
    builder.Function<SelectOverload<void (WWiseEmitter::*)(u64)>(&WWiseEmitter::PlayEvent)>("PlaySound")
      .SetParameterNames("aSound");
  }

  WWiseEmitter::WWiseEmitter(Composition *aOwner, Space *aSpace)
    : Component(aOwner, aSpace)
  {
    mEmitterPosition.ConstructAndGet<AkSoundPosition>();
    mIsListening = true;
  }

  WWiseEmitter::~WWiseEmitter()
  {
    AK::SoundEngine::UnregisterGameObj(OwnerId());
  }
  
  void WWiseEmitter::NativeInitialize()
  {
    AK::SoundEngine::RegisterGameObj(OwnerId(), mOwner->GetName().c_str());

    auto view = mSpace->GetComponent<WWiseView>();

    if (view)
    {
      view->RegisterEvent<&WWiseEmitter::ListenerChanged>(Events::WWiseListenerChanged, this);
    }
    else
    {
      std::cout << "No WWiseView on the current space, playing will fail!\n";
    }
  }

  void WWiseEmitter::Play()
  {
    mSpace->GetEngine()->GetComponent<WWiseSystem>()->SendEvent(mSound, OwnerId());
  }

  void WWiseEmitter::PlayEvent(const std::string &aEvent)
  {
    mSpace->GetEngine()->GetComponent<WWiseSystem>()->SendEvent(aEvent, OwnerId());
  }

  void WWiseEmitter::PlayEvent(u64 aEvent)
  {
    mSpace->GetEngine()->GetComponent<WWiseSystem>()->SendEvent(aEvent, OwnerId());
  }

  void WWiseEmitter::Initialize()
  {
    auto view = mSpace->GetComponent<WWiseView>();

    if (view)
    {
      auto listener = view->GetActiveListener()->OwnerId();
      AK::SoundEngine::SetListeners(OwnerId(), &listener, 1);
    }
    else
    {
      std::cout << "No WWiseView on the current space, playing will fail!\n";
    }

    RegisterForListening();

    SetEmitterPosition();
  }

  void WWiseEmitter::ListenerChanged(WWiseListenerChanged *aListenerChange)
  {
    auto listener = aListenerChange->NewListener->OwnerId();
    AK::SoundEngine::SetListeners(OwnerId(), &listener, 1);
  }

  void WWiseEmitter::OnPositionChange(const TransformChanged *aEvent)
  {
    auto self = mEmitterPosition.Get<AkSoundPosition>();
    //std::cout << "Emitter Orientation changed\n";
    self->SetPosition(MakeAkVec(aEvent->Position));
    SetEmitterPosition();
  }

  void WWiseEmitter::OnOrientationChange(const OrientationChanged *aEvent)
  {
    auto self = mEmitterPosition.Get<AkSoundPosition>();

    self->SetOrientation(MakeAkVec(aEvent->ForwardVector),
                         MakeAkVec(aEvent->UpVector));

    SetEmitterPosition();
  }

  void WWiseEmitter::SetEmitterPosition()
  {
    auto self = mEmitterPosition.Get<AkSoundPosition>();
    AK::SoundEngine::SetPosition(OwnerId(), *self);
  }

  void WWiseEmitter::RegisterForListening()
  {
      auto transform = mOwner->GetComponent<Transform>();

      if (transform != nullptr)
      {
        auto self = mEmitterPosition.Get<AkSoundPosition>();

        mOwner->RegisterEvent<&WWiseEmitter::OnPositionChange>(YTE::Events::PositionChanged, this);
        mOwner->RegisterEvent<&WWiseEmitter::OnOrientationChange>(YTE::Events::OrientationChanged, this);
        self->SetPosition(MakeAkVec(transform->GetTranslation()));

        auto orientation = mOwner->GetComponent<Orientation>();

        self->SetOrientation(MakeAkVec(orientation->GetForwardVector()),
                             MakeAkVec(orientation->GetUpVector()));
      }
  }

  
  void WWiseEmitter::ListenToTransformEvents(bool aListen)
  {
    if (mIsListening == aListen)
    {
      return;
    }

    mIsListening = aListen;

    if (aListen)
    {
      RegisterForListening();
    }
    else
    {
      mOwner->DeregisterEvent<&WWiseEmitter::OnPositionChange>(YTE::Events::PositionChanged, this);
      mOwner->DeregisterEvent<&WWiseEmitter::OnOrientationChange>(YTE::Events::OrientationChanged, this);
    }
  }
}
