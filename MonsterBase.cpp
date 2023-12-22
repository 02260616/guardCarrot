#include "MonsterBase.h"
#include "MonsterManager.h"
#include "MoveControllerBase.h"
#include "SoundUtil.h"

bool MonsterBase::Init(const int& IId)
{
    bool Ret = false;

    do
    {
        CC_BREAK_IF(!VictimEntityBase::Init(IId, MONSTERCSVFILE));// 调用父类的初始化方法
        CC_BREAK_IF(!DynamicPropertyInterFace::Init(IId, MONSTERCSVFILE));// 调用接口类的初始化方法
        myIsLeft = true;
        CreateAnimation();// 创建动画
        DoAction(0.15f);// 执行动作
        pMoveController = MoveControllerBase::Create(this);// 创建移动控制器
        MonsterManager::getInstance()->addMonster(this);// 创建移动控制器
        Ret = true;
    } while (0);
    return  Ret;
}

MonsterBase* MonsterBase::Create(const int& IId)
{
    auto pMonster = new MonsterBase();
    if (pMonster && pMonster->Init(IId)) {
        pMonster->autorelease();
        return pMonster;
    }
    CC_SAFE_DELETE(pMonster);
    return nullptr;
}

void MonsterBase::HurtMove()
{

}

void MonsterBase::CreateAnimation()
{
    std::vector<std::string> AniamtionFiles;
    for (int i = 1; i <= _IAnimationFrameCount; i++)//
        AniamtionFiles.push_back(_sModelName + StringUtils::format("%d.png", i));
    CreateDynamicEntityAnimation(AniamtionFiles);
}

void MonsterBase::DoAction(const float& Dt)
{
    pAnimation->setDelayPerUnit(Dt);// 设置动画的延迟时间
    getSprite()->runAction(Animate::create(pAnimation));//运行
}

void MonsterBase::DeadMove(const std::string& SDeadImageFile)
{
    MonsterSound();// 播放怪物死亡音效
    NOTIFY->postNotification("MonsterDead", this);// 发送怪物死亡通知
    getSprite()->removeAllChildrenWithCleanup(true);
    pMoveController->stopAllActions();// 停止移动控制器的所有动作
    VictimEntityBase::DeadMove();
}

void MonsterBase::MonsterSound()
{
    int fFigure = rand() % 3 + 1;
    if (Name.find("land") != std::string::npos)
    {
        SoundUtil::getInstance()->playEffect(MONSTERMUSIC + StringUtils::format("Land11%d.mp3", fFigure));
    }
    else if (_sName.find("fly") != std::string::npos)
    {
        SoundUtil::getInstance()->playEffect(MONSTERMUSIC + StringUtils::format("Fat14%d.mp3", fFigure));
    }
    else if (_sName.find("fat") != std::string::npos)
    {
        SoundUtil::getInstance()->playEffect(MONSTERMUSIC + StringUtils::format("Fat14%d.mp3", fFigure));
    }
    else
    {
        SoundUtil::getInstance()->playEffect(MONSTERMUSIC + "BigBoss.mp3");
    }
}

void MonsterBase::SetMaxSpeed(const int& Speed)
{
    setMySpeed(Speed);
    pMoveController->setMyMonsterSpeed(Speed);
}
