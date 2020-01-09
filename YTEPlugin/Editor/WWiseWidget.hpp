#pragma once

#include <QScrollArea>
#include <QLayout>
#include <QPushButton>
#include <QWidget>

#include "YTE/Core/ForwardDeclarations.hpp"

#include "YTEPlugin/Engine/WWiseSystem.hpp"

#include "YTEditor/Framework/MainWindow.hpp"
#include "YTEditor/Framework/Widget.hpp"

#include "YTEditor/YTELevelEditor/YTELevelEditor.hpp"

namespace YTEPlugin::WWise
{
  class WWiseWidget : public YTEditor::Framework::Widget<QWidget>
  {
  public:
    WWiseWidget(YTEditor::YTELevelEditor* aWorkspace, YTE::Engine* aEngine);
    ~WWiseWidget();

    YTEPlugin::WWise::WwiseObject OwnerId() { return reinterpret_cast<YTEPlugin::WWise::WwiseObject>(this); };

    void LoadEvents();

    static std::string GetName();

    ToolWindowManager::AreaReference GetToolArea() override;

  private:

    YTE::Engine *mEngine;
    YTEPlugin::WWise::WWiseSystem* mSystem;

    QVBoxLayout *mLayout;
    QWidget *mSubWidget;
    QVBoxLayout *mSubWidgetLayout;
    QScrollArea *mScrollArea;

    void ConstructSubWidgets();
    

  };
}
