#pragma once
#ifndef __Carrot__MonsterHelpLayer__
#define __Carrot__MonsterHelpLayer__

#include "PublicDefine.h"

class MonsterHelpLayer :public Layer
{
public:

    CREATE_FUNC(MonsterHelpLayer);

protected:

    virtual bool Init()override;
};

#endif
