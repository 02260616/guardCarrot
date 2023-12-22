#include "Hp.h"
#include "VictimEntityBase.h"

Hp::~Hp()
{
    CC_SAFE_RELEASE_NULL(pVictimEntity);
    CC_SAFE_RELEASE_NULL(pHp);
}

Hp* Hp::Create(VictimEntityBase* _VictimEntity)
{
    auto pHp = new Hp(); // 创建血条示例
    if (pHp && pHp->Init(_VictimEntity))//如果分配内存成功且初始化成功, 自动释放内存
        pHp->autorelease(); 
    else CC_SAFE_DELETE(pHp); //否则安全删除 HpSlot 对象：
    return pHp;
}

bool Hp::Init(VictimEntityBase* _VictimEntity)
{
    bool Ret = false;//表示初始化是否成功

    do
    {
        CC_SAFE_RETAIN(_VictimEntity);//对传入的 _VictimEntity进行引用计数增加，然后将其赋值给pVictimEntity
        pVictimEntity = _VictimEntity;
        HpMax = pVictimEntity->getMyHp();//获取目标实体的最大生命值

        auto pHpBg = Sprite::createWithSpriteFrameName("MonsterHP01.png");//创建血条背景精灵 pHpBg，设置其锚点为左中点，并将其添加到 HpSlot 对象中。
        pHpBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        addChild(pHpBg);
        //创建血条,设置血条属性
        pHp = ProgressTimer::create(Sprite::createWithSpriteFrameName("MonsterHP02.png"));
        pHp->setType(ProgressTimer::Type::BAR);
        pHp->setMidpoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        pHp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        pHp->setBarChangeRate(Vec2::ANCHOR_BOTTOM_RIGHT);
        pHp->setPercentage(0);
        pHp->setPositionX(2);

        addChild(pHp); //将血条添加到 Hp 对象中：
        Ret = true;
    } while (0);

    return Ret;
}

void Hp::SetHp(const int& rIHp)
{
    pHp->setPercentage(100 - pVictimEntity->getMyHp() * 1.0f / HpMax * 100);
}

const Size& Hp::GetSize()const
{
    return pHp->getContentSize();
}