#include "MonsterManager.h"
#include "MonsterBase.h"
#include "GameManager.h"

MonsterManager* MonsterManager::_gInstance;

MonsterManager::MonsterManager()
{
}

void MonsterManager::ClearAtkLockState()
{
    for (auto& IterMonster : *pMonsterVec)
    {
        IterMonster->SetAtkTarget(false);
    }
}

MonsterManager::~MonsterManager()
{
    CC_SAFE_DELETE(pMonsterVec);
}

bool MonsterManager::Init()
{
    bool Ret = false;
    do
    {
        pMonsterVec = new Vector<MonsterBase*>;
        RegisterDeadEvent();// 注册怪物死亡事件
        Ret = true;
    } while (0);

    return Ret;
}

Vector<MonsterBase*>& MonsterManager::GetMonsterVec()
{
    return *pMonsterVec;
}

void MonsterManager::AddMonster(MonsterBase* pMonster)
{
    if (pMonsterVec) pMonsterVec->pushBack(pMonster);
    funcAddMonsterToLayer(pMonster);
}

void MonsterManager::SetFuncAddMonsterToLayer(const std::function<void(Entity*)>& FuncAddMonsterToLayer)
{
    funcAddMonsterToLayer = FuncAddMonsterToLayer;
}

void MonsterManager::RegisterDeadEvent()
{
    NOTIFY->addObserver(this, callfuncO_selector(MonsterManager::removeMonsterFromManager), "MonsterDead", nullptr);// 注册怪物死亡事件的监听器
    NOTIFY->addObserver(this, callfuncO_selector(MonsterManager::openCheckGameEnd), "openCheckGameEnd", nullptr);// 注册打开检查游戏结束事件的监听器
}

void MonsterManager::RemoveMonsterFromManager(Ref* Monster)
{
    if (!(pMonsterVec && pMonsterVec->size())) return;// 检查怪物容器是否为空
    pMonsterVec->eraseObject(reinterpret_cast<MonsterBase*>(Monster));// 从怪物容器中移除指定的怪物对象

    if (IsOpenCheckGameEnd)
    {
        if (!pMonsterVec->size())
        {
            auto tEnGameWin = en_GameWin;// 设置游戏胜利状态
            if (GameManager::getInstance()->getICarrotHp() >= 1)// 如果胡萝卜生命值大于等于1，发送游戏结束事件
                NOTIFY->postNotification("gameEnd", reinterpret_cast<Ref*>(&tEnGameWin));
            IsOpenCheckGameEnd = false;
        }
    }
}

void MonsterManager::ClearManager()
{
    pMonsterVec->clear();
}

void MonsterManager::OpenCheckGameEnd(cocos2d::Ref* pSender)
{
    IsOpenCheckGameEnd = true;
}