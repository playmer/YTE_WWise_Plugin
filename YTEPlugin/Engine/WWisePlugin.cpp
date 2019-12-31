#include "YTE/Core/CoreComponentFactoryInitilization.hpp"
#include "YTE/Core/Engine.hpp"
#include "YTE/Core/Plugin.hpp"

#include "WWiseEmitter.hpp"
#include "WWiseListener.hpp"
#include "WWiseSystem.hpp"
#include "WWiseView.hpp"

namespace YTEPlugin::WWise
{
  using namespace YTE;

  class WWisePlugin : public YTE::Plugin
  {
    public:
    WWisePlugin()
    {

    }

    void Load(YTE::Engine* aEngine) override
    {
      mEngine = aEngine;

      InitializeType<WWiseSystem>();
      InitializeType<WWiseEmitter>();
      InitializeType<WWiseListener>();
      InitializeType<WWiseView>();

      YTE::ComponentFactoryHelper helper{ aEngine, aEngine->GetComponentSystem()->GetComponentFactories() };

      helper.CreateComponentFactory<WWiseSystem>();
      helper.CreateComponentFactory<WWiseEmitter>();
      helper.CreateComponentFactory<WWiseListener>();
      helper.CreateComponentFactory<WWiseView>();
      
      aEngine->AddComponent<WWiseSystem>();
    }

    void Unload() override
    {
      YTE::ComponentFactoryHelper helper{ mEngine, mEngine->GetComponentSystem()->GetComponentFactories() };

      helper.DestroyComponentFactory<WWiseSystem>();
      helper.DestroyComponentFactory<WWiseEmitter>();
      helper.DestroyComponentFactory<WWiseListener>();
      helper.DestroyComponentFactory<WWiseView>();

      mEngine->RemoveComponent(WWiseSystem::GetStaticType());
    }

    YTE::Engine *mEngine;
  };
}


extern "C"
{
  YTEPlugin_WWise_Shared void* PluginNew()
  {
    return static_cast<void*>(new YTEPlugin::WWise::WWisePlugin{});
  }

  YTEPlugin_WWise_Shared void PluginDelete(void* aPlugin)
  {
    delete static_cast<YTE::Plugin*>(aPlugin);
  }
}