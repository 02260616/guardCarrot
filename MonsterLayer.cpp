#include "MonsterLayer.h"
#include "MapUtil.h"

bool MonsterLayer::Init()
{
    bool Ret = false;

    do
    {
        CC_BREAK_IF(!Layer::init());

        auto pStart = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("start01.png"));//// 创建并添加一个精灵 pStart，使用 SpriteFrameCache 获取精灵帧，并设置位置
        pStart->setPosition(MapUtil::getInstance()->getMovePoses().at(0) + Vec2(40, -15));
        addChild(pStart);// 将 pStart 添加到当前层

        RegisterMonsterDeadEvent();// 注册怪物死亡事件处理函数
        Ret = true;
    } while (0);

    return Ret;
}


void MonsterLayer::RegisterMonsterDeadEvent()
{
    NOTIFY->addObserver(this, callfuncO_selector(MonsterLayer::monsterDead), "monsterDeadMoney", nullptr);
}

void MonsterLayer::MonsterDead(Ref* Data)
{
    auto tValue = *(reinterpret_cast<std::tuple<Vec2, int>*>(pData));
    auto tMoney = std::get<1>(tValue);
    auto pMoney = Sprite::createWithSpriteFrameName(StringUtils::format("money%d.png", tMoney));
    auto pVec2 = std::get<0>(tValue);
    pMoney->setPosition(pVec2);
    addChild(pMoney);
    pMoney->runAction(Sequence::create(MoveBy::create(0.6f, Vec2(0, 100)), DelayTime::create(0.4), FadeOut::create(0.4f), CallFunc::create([=]() {pMoney->removeFromParent(); }), NULL));

}

void MonsterLayer::OnExit()
{
    Layer::onExit();
    NOTIFY->removeAllObservers(this);
}