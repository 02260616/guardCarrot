#include "MonsterHelpLayer.h"


bool MonsterHelpLayer::Init()
{
    bool Ret = false;

    do
    {
        CC_BREAK_IF(!Layer::Init());

        Ret = true;
    } while (0);
    auto monsterHelpLayer = Sprite::createWithSpriteFrameName("help_monster.png");
    monsterHelpLayer->setPosition(480, 280);
    monsterHelpLayer->setScale(0.95f);
    addChild(monsterHelpLayer);
    return Ret;
}