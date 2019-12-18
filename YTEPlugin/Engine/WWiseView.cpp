#include "YTEPlugin/Engine/WWiseMeta.hpp"
#include "YTEPlugin/Engine/WWiseView.hpp"


namespace YTEPlugin::WWise
{
  YTEWWiseDefineEvent(WWiseListenerChanged);

  YTEWWiseDefineType(WWiseListenerChanged)
  {
    YTE::RegisterType<WWiseListenerChanged>();
    YTE::TypeBuilder<WWiseListenerChanged> builder;
  }

  YTEWWiseDefineType(WWiseView)
  {
    YTE::RegisterType<WWiseView>();
    YTE::TypeBuilder<WWiseView> builder;

    GetStaticType()->AddAttribute<YTE::RunInEditor>();
  }
}