#pragma once

#ifndef __Carrot__MonsterManager__
#define __Carrot__MonsterManager__

#include "PublicDefine.h"

class Entity;

class MonsterManager : public Ref
{
public:

    CREATE_INSTANCE_FUNC(MonsterManager);

    DESTROY_INSTANCE_FUNC();

    virtual MonsterVec& GetMonsterVec();// 获取怪物容器的引用

    virtual void SetFuncAddMonsterToLayer(const std::function<void(Entity*)>& FuncAddMonsterToLayer);// 设置添加怪物到图层的回调函数

    virtual void ClearManager();// 清除管理器状态

    virtual void ClearAtkLockState();// 清除被攻击锁定的状态

protected:

    friend class MonsterBase;

    virtual void OpenCheckGameEnd(Ref* Sender);// 开启检查游戏结束的事件

    virtual void AddMonster(MonsterBase* Bullet); // 向怪物容器中添加怪物

    virtual bool Init();// 初始化

    virtual void RegisterDeadEvent();// 注册怪物死亡事件

    virtual void RemoveMonsterFromManager(Ref* Monster);// 从管理器中移除怪物

private:

    MonsterVec* pMonsterVec = nullptr;// 怪物容器

    std::function<void(Entity*)> funcAddMonsterToLayer;// 添加怪物到图层的回调函数

    bool IsOpenCheckGameEnd = false;

private:

    MonsterManager();

    virtual ~MonsterManager();

    MonsterManager(const MonsterManager&) = delete;// 禁止拷贝构造函数

    MonsterManager& operator=(const MonsterManager&) = delete;// 禁止赋值操作符重载

};
#endif