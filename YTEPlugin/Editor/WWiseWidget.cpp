#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

#include "AK/SoundEngine/Common/AkSoundEngine.h"

#include "YTE/Core/Engine.hpp"

#include "YTEditor/YTELevelEditor/Widgets/ComponentBrowser/ComponentBrowser.hpp"
#include "YTEPlugin/Editor/WWiseWidget.hpp"

namespace YTEPlugin::WWise
{
  class SetWWiseSwitch : public QComboBox
  {
  public:
    SetWWiseSwitch(QWidget *aOwner, YTE::u64 aGroupId, YTEPlugin::WWise::WWiseSystem *aSystem, WWiseWidget *aWidget, bool aSwitch)
      : QComboBox(aOwner)
      , mGroupId(aGroupId)
      , mSystem(aSystem)
      , mWidget(aWidget)
      , mSwitch(aSwitch)
    {

    }

    void SetCurrentSwitchOrState()
    {
      auto id = this->currentData().toULongLong();

      if (mSwitch)
      {
        mSystem->SetSwitch(mGroupId, id, OwnerId());
      }
      else
      {
        mSystem->SetState(mGroupId, id);
      }
    }

    ~SetWWiseSwitch()
    {

    }

    AkGameObjectID OwnerId() { return reinterpret_cast<AkGameObjectID>(mWidget); };

    void indexChanged(int aIndex)
    {
      YTE::UnusedArguments(aIndex);
      SetCurrentSwitchOrState();
    }

  private:
    YTEPlugin::WWise::WWiseSystem *mSystem;
    YTE::u64 mGroupId;
    WWiseWidget *mWidget;
    bool mSwitch;
  };

  class SetWWiseRTPC : public QLineEdit
  {
  public:
    SetWWiseRTPC(QWidget *aOwner, YTE::u64 aRTPCId, YTEPlugin::WWise::WWiseSystem *aSystem)
      : QLineEdit(aOwner)
      , mRTPCId(aRTPCId)
      , mSystem(aSystem)
    {
      this->setValidator(new QDoubleValidator(this));
    }

    void SetRTPC(float aValue)
    {
      mSystem->SetRTPC(mRTPCId, aValue);
    }

    ~SetWWiseRTPC()
    {

    }

    void changedText(const QString &aText)
    {
      SetRTPC(aText.toFloat());
    }

  private:
    YTEPlugin::WWise::WWiseSystem *mSystem;
    YTE::u64 mRTPCId;
  };

  class SendWWiseEvent : public QPushButton
  {
  public:
    SendWWiseEvent(YTEPlugin::WWise::WWiseSystem *aSystem, 
                   const std::string &aEvent, 
                   YTE::u64 aEventId, 
                   WWiseWidget *aWidget)
      : QPushButton(aEvent.c_str())
      , mEventId(aEventId)
      , mSystem(aSystem)
      , mWidget(aWidget)
    {
      auto listenerId = static_cast<AkGameObjectID>(aWidget->OwnerId());
      aSystem->RegisterObject(OwnerId(), aEvent);

      auto check = AK::SoundEngine::SetListeners(OwnerId(), &listenerId, 1);
      assert(check == AK_Success);
    }

    ~SendWWiseEvent()
    {

    }

    void clicked()
    {
      mSystem->SendEvent(mEventId, OwnerId());
    }
    
    AkGameObjectID OwnerId() { return static_cast<AkGameObjectID>(reinterpret_cast<YTEPlugin::WWise::WwiseObject>(this)); };

  private:
    WWiseSystem* mSystem;
    YTE::u64 mEventId;
    WWiseWidget* mWidget;
  };

  WWiseWidget::WWiseWidget(YTEditor::YTELevelEditor* aWorkspace, YTE::Engine* aEngine)
    : Widget{ aWorkspace }
    , mEngine{ aEngine }
  {
    std::string name{ "WWiseWidget" };
    mSystem = mEngine->GetComponent<WWiseSystem>();
    mSystem->RegisterObject(static_cast<AkGameObjectID>(OwnerId()), name);

    ConstructSubWidgets();

    LoadEvents();
  }

  WWiseWidget::~WWiseWidget()
  {
    mSystem->DeregisterObject(static_cast<AkGameObjectID>(OwnerId()));
  }

  void WWiseWidget::LoadEvents()
  {
    auto &banks = mSystem->GetBanks();

    for (auto &bank : banks)
    {
      auto bankGroupBox = new QGroupBox(bank.second.mName.c_str(), this);
      QVBoxLayout *bankVbox = new QVBoxLayout(bankGroupBox);

      /////////////////////////////////////////////////////////////////////////////
      // RTPCs
      /////////////////////////////////////////////////////////////////////////////
      QGroupBox *rtpcGroupBox = nullptr;

      if (bank.second.mRTPCs.size())
      {
        rtpcGroupBox = new QGroupBox("Game Parameters", bankGroupBox);

        QVBoxLayout *rtpcGroupVbox = new QVBoxLayout(rtpcGroupBox);

        rtpcGroupBox->setLayout(rtpcGroupVbox);

        for (auto &rtpc : bank.second.mRTPCs)
        {
          auto dummy = new QWidget(rtpcGroupBox);
          auto hbox = new QHBoxLayout(dummy);

          hbox->addWidget(new QLabel(rtpc.mName.c_str(), dummy));

          auto lineEdit = new SetWWiseRTPC(dummy,
                                           rtpc.mId,
                                           mSystem);

          lineEdit->SetRTPC(0.0f);

          this->connect(static_cast<QLineEdit*>(lineEdit),
                        static_cast<void (QLineEdit::*)(const QString &)>(&QLineEdit::textChanged),
                        lineEdit,
                        &SetWWiseRTPC::changedText);

          hbox->addWidget(lineEdit);

          dummy->setLayout(hbox);

          rtpcGroupVbox->addWidget(dummy);
        }
        rtpcGroupVbox->addStretch(1);
        bankVbox->addWidget(rtpcGroupBox);
      }

      /////////////////////////////////////////////////////////////////////////////
      // Switches
      /////////////////////////////////////////////////////////////////////////////
      QGroupBox *switchGroupGroupBox = nullptr;

      if (bank.second.mSwitchGroups.size())
      {
        switchGroupGroupBox = new QGroupBox("Switch Groups", bankGroupBox);

        QVBoxLayout *switchGroupVbox = new QVBoxLayout(switchGroupGroupBox);

        switchGroupGroupBox->setLayout(switchGroupVbox);

        for (auto &switchGroup : bank.second.mSwitchGroups)
        {
          auto dummy = new QWidget(switchGroupGroupBox);
          auto hbox = new QHBoxLayout(dummy);

          hbox->addWidget(new QLabel(switchGroup.first.c_str(), dummy));

          auto comboBox = new SetWWiseSwitch(dummy,
            switchGroup.second.first.mId,
            mSystem,
            this,
            true);



          for (auto &aSwitch : switchGroup.second.second)
          {
            comboBox->addItem(aSwitch.mName.c_str(), static_cast<qulonglong>(aSwitch.mId));
          }

          comboBox->SetCurrentSwitchOrState();

          this->connect(static_cast<QComboBox*>(comboBox),
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            comboBox,
            &SetWWiseSwitch::indexChanged);

          hbox->addWidget(comboBox);

          dummy->setLayout(hbox);

          switchGroupVbox->addWidget(dummy);
        }
        switchGroupVbox->addStretch(1);
        bankVbox->addWidget(switchGroupGroupBox);
      }

      /////////////////////////////////////////////////////////////////////////////
      // States
      /////////////////////////////////////////////////////////////////////////////
      QGroupBox *stateGroupGroupBox = nullptr;

      if (bank.second.mStateGroups.size())
      {
        stateGroupGroupBox = new QGroupBox("State Groups", bankGroupBox);


        QVBoxLayout *stateGroupVbox = new QVBoxLayout(stateGroupGroupBox);
        for (auto &stateGroup : bank.second.mStateGroups)
        {
          auto dummy = new QWidget(stateGroupGroupBox);
          auto hbox = new QHBoxLayout(dummy);

          hbox->addWidget(new QLabel(stateGroup.first.c_str(), dummy));

          auto comboBox = new SetWWiseSwitch(dummy,
            stateGroup.second.first.mId,
            mSystem,
            this,
            false);

          for (auto &state : stateGroup.second.second)
          {
            comboBox->addItem(state.mName.c_str(), static_cast<qulonglong>(state.mId));
          }

          comboBox->SetCurrentSwitchOrState();

          this->connect(static_cast<QComboBox*>(comboBox),
                        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                        comboBox,
                        &SetWWiseSwitch::indexChanged);

          hbox->addWidget(comboBox);

          dummy->setLayout(hbox);

          stateGroupVbox->addWidget(dummy);
        }
        stateGroupVbox->addStretch(1);
        bankVbox->addWidget(stateGroupGroupBox);
      }

      /////////////////////////////////////////////////////////////////////////////
      // Events
      /////////////////////////////////////////////////////////////////////////////
      QGroupBox *eventGroupBox = nullptr;

      if (bank.second.mEvents.size())
      {
        eventGroupBox = new QGroupBox("Events", bankGroupBox);


        QVBoxLayout *eventVbox = new QVBoxLayout(eventGroupBox);
        for (auto &event : bank.second.mEvents)
        {
          SendWWiseEvent *toggleButton = new SendWWiseEvent(mSystem,
                                                            event.mName,
                                                            event.mId,
                                                            this);

          this->connect(toggleButton,
                        &SendWWiseEvent::released,
                        toggleButton,
                        &SendWWiseEvent::clicked);

          eventVbox->addWidget(toggleButton);
        }
        eventVbox->addStretch(1);
        bankVbox->addWidget(eventGroupBox);
      }

      //bankVbox->addWidget(rtpcGroupBox);
      bankVbox->addStretch(1);

      bankGroupBox->setLayout(bankVbox);

      mSubWidgetLayout->addWidget(bankGroupBox);
    }
  }

  std::string WWiseWidget::GetName()
  {
    return "WWiseWidget";
  }

  ToolWindowManager::AreaReference WWiseWidget::GetToolArea()
  {
    auto area = mWorkspace->GetMainWindow()->GetToolWindowManager()->areaOf(mWorkspace->GetWidget<YTEditor::ComponentBrowser>());

    if (area != nullptr)
    {
      return ToolWindowManager::AreaReference{
        ToolWindowManager::AreaReferenceType::BottomOf,
        area
      };
    }
    else
    {
      return ToolWindowManager::AreaReference{
        ToolWindowManager::AreaReferenceType::LastUsedArea
      };
    }
  }

  void WWiseWidget::ConstructSubWidgets()
  {
    mLayout = new QVBoxLayout(this);

    mScrollArea = new QScrollArea(this);
    mScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    mSubWidget = new QWidget(mScrollArea);

    mSubWidgetLayout = new QVBoxLayout(mSubWidget);
    mSubWidget->setLayout(mSubWidgetLayout);

    mScrollArea->setWidget(mSubWidget);

    mLayout->addWidget(mScrollArea);

    mLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    mScrollArea->setWidgetResizable(true);

  }
}