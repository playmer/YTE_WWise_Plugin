#include "YTE/Core/CoreComponentFactoryInitilization.hpp"
#include "YTE/Core/Engine.hpp"
#include "YTE/Core/Plugin.hpp"

#include "YTEditor/EditorPlugin.hpp"
#include "YTEditor/EditorPlugin.hpp"
#include "YTEditor/EditorPlugin.hpp"

#include "YTEPlugin/Editor/WWiseEditorMeta.hpp"

namespace YTEPlugin::WWise
{
  using namespace YTE;

  class WWisePlugin : public YTEditor::EditorPlugin
  {
    public:
    WWisePlugin()
    {

    }

    void Load(YTEditor::YTEditorMainWindow* aEngine) override
    {
    }

    void Unload() override
    {
    }

    YTEditor::YTEditorMainWindow* mEngine;
  };
}


extern "C"
{
  YTEditorPlugin_WWise_Shared void* PluginNew()
  {
    return static_cast<void*>(new YTEPlugin::WWise::WWisePlugin{});
  }

  YTEditorPlugin_WWise_Shared void PluginDelete(void* aPlugin)
  {
    delete static_cast<YTE::Plugin*>(aPlugin);
  }
}