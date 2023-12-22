#pragma once
#ifndef __CarrotFantasy__MonsterFactory__
#define __CarrotFantasy__MonsterFactory__

#include "PublicDefine.h"

class MonsterFactory
{
public:

    static MonsterBase* CreateMonster(const MonsterType& MonsterType);
};

#endif /* defined(__CarrotFantasy__MonsterFactory__) */
