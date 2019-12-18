#pragma once

#ifndef YTEPlugin_WWise_WWiseSystem_hpp
#define YTEPlugin_WWise_WWiseSystem_hpp

#include <unordered_map>
#include <vector>

#include "YTE/Core/Component.hpp"

#include "YTE/Meta/Meta.hpp"

#include "YTE/Platform/ForwardDeclarations.hpp"

#include "YTE/Utilities/String/String.hpp"

#include "YTEPlugin/Engine/ForwardDeclarations.hpp"
#include "YTEPlugin/Engine/WWiseMeta.hpp"

namespace YTEPlugin::WWise
{
  struct AudioBank
  {
    struct AudioPair
    {
      AudioPair()
        : mId(0)
        , mName()
      {

      }

      AudioPair(YTE::u64 aId, std::string &aName)
        : mId(aId)
        , mName(aName)
      {

      }

      YTE::u64 mId;
      std::string mName;
    };

    std::string mName;
    std::vector<AudioPair> mEvents;
    std::unordered_map<std::string, std::pair<AudioPair, std::vector<AudioPair>>> mSwitchGroups;
    std::unordered_map<std::string, std::pair<AudioPair, std::vector<AudioPair>>> mStateGroups;
    std::vector<AudioPair> mRTPCs;

    WwiseBank mBankID;
  };

  class WWiseSystem : public YTE::Component
  {
  public:
    YTEWWiseDeclareType(WWiseSystem);

    // Sets up the system, probably could just be the constructor.

    YTEPlugin_WWise_Shared WWiseSystem(YTE::Composition* aOwner, YTE::Space* aSpace);

    YTEPlugin_WWise_Shared virtual void Initialize() override;
    // Updates the system to the current frame.
    YTEPlugin_WWise_Shared void Update(double);

    // Cleans up anything in the system.
    YTEPlugin_WWise_Shared ~WWiseSystem() override;

    YTEPlugin_WWise_Shared void RegisterObject(WwiseObject aId, std::string& aName);
    YTEPlugin_WWise_Shared void DeregisterObject(WwiseObject aId);
    YTEPlugin_WWise_Shared YTE::u8 RegisterListener(WwiseObject aId, std::string& aName);
    YTEPlugin_WWise_Shared void DeregisterListener(WwiseObject aId, YTE::u8 aListener);
    YTEPlugin_WWise_Shared void LoadAllBanks();
    YTEPlugin_WWise_Shared AudioBank& LoadBank(std::string const& aFilename);
    YTEPlugin_WWise_Shared void UnloadBank(std::string const& aBankName);
    YTEPlugin_WWise_Shared void UnloadAllBanks();
    YTEPlugin_WWise_Shared void SendEvent(std::string const& aEvent, WwiseObject aId);
    YTEPlugin_WWise_Shared void SendEvent(YTE::u64 aEventId, WwiseObject aId);
    YTEPlugin_WWise_Shared void SetSwitch(std::string const& aSwitchGroup, std::string const& aSwitch, WwiseObject aId);
    YTEPlugin_WWise_Shared void SetSwitch(YTE::u64 aSwitchGroupId, YTE::u64 aSwitchId, WwiseObject aId);
    YTEPlugin_WWise_Shared void SetState(std::string const& aStateGroup, std::string const& aState);
    YTEPlugin_WWise_Shared void SetState(YTE::u64 aStateGroupId, YTE::u64 aStateId);
    YTEPlugin_WWise_Shared YTE::u64 GetSoundIDFromString(std::string const& aName);
    YTEPlugin_WWise_Shared void SetRTPC(std::string const& aRTPC, float aValue);
    YTEPlugin_WWise_Shared void SetRTPC(YTE::u64 aRTPC, float aValue);
    YTEPlugin_WWise_Shared bool GetMute();
    YTEPlugin_WWise_Shared void SetMute(bool aMute);

    std::unordered_map<std::string, AudioBank>& GetBanks() { return mBanks; }

  private:
    void WindowLostOrGainedFocusHandler(const YTE::WindowFocusLostOrGained* aEvent);
    void WindowMinimizedOrRestoredHandler(const YTE::WindowMinimizedOrRestored* aEvent);

    void ReadTxtFile(std::string &aFile, AudioBank &bank);

    std::vector<YTE::u8> mAvailableListeners;
    std::unordered_map<std::string, AudioBank> mBanks;

    bool mPaused = false;
    bool mPriorToMinimize = false;
    bool mFocusHandled = false;

    bool mMuted;

    bool mMinimized = false;
    bool mFocused = false;
  };
}

#endif