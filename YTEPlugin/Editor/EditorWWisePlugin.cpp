#include "YTE/Core/CoreComponentFactoryInitilization.hpp"
#include "YTE/Core/Engine.hpp"
#include "YTE/Core/Plugin.hpp"

#include "YTEditor/YTELevelEditor/YTEditorMainWindow.hpp"
#include "YTEditor/YTELevelEditor/YTELevelEditor.hpp"
#include "YTEditor/EditorPlugin.hpp"

#include "YTEPlugin/Editor/WWiseEditorMeta.hpp"
#include "YTEPlugin/Editor/WWiseWidget.hpp"

namespace YTEPlugin::WWise
{
  using namespace YTE;

  class WWisePlugin : public YTEditor::EditorPlugin
  {
    public:
    WWisePlugin()
      : mMainWindow{ nullptr }
      , mWorkSpace{ nullptr }
      , mWWiseWidget{ nullptr }
    {

    }

    void Load(YTEditor::YTEditorMainWindow* aMainWindow) override
    {
      mMainWindow = aMainWindow;

      mWorkSpace = mMainWindow->GetWorkspace<YTEditor::YTELevelEditor>();
      mWWiseWidget = mWorkSpace->AddWidget<WWiseWidget>("WWiseWidget", mWorkSpace, aMainWindow->GetRunningEngine());
    }

    void Unload() override
    {
      mWorkSpace->RemoveWidget<WWiseWidget>(mWWiseWidget);
    }

    YTEditor::YTEditorMainWindow* mMainWindow;
    YTEditor::YTELevelEditor* mWorkSpace;
    WWiseWidget* mWWiseWidget;
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