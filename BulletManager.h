#pragma once
#include "PublicDefine.h"
class Entity;
class BulletManager : public Ref//支持引用计数和内存管理
{
public:

    CREATE_INSTANCE_FUNC(BulletManager);//创建BulletManager类实例

    DESTROY_INSTANCE_FUNC();//销毁BulletManager类实例

    virtual BulletVec& GetBulletVec();//获取子弹容器

    virtual void SetFuncAddBulletLayer(const std::function<void(Entity*)>& FuncAddBulletLayer);//设置一个回调函数，用于添加子弹到图层中,并通知其他图层

    virtual void CheckBulletIsDead();//检查子弹是否已经死亡

    virtual void ClearManager();//清除管理器

protected:

    friend class BulletBase;

    virtual void AddBullet(BulletBase* Bullet);//添加子弹到管理器中

    virtual bool Init();//初始化

private:

    BulletVec* pBulletVec = nullptr;

    std::function<void(Entity*)> funcAddBulletLayer;//回调函数，用于添加子弹到图层中

private:

    BulletManager();

    virtual ~BulletManager();

    BulletManager(const BulletManager&) = delete;

    BulletManager& operator=(const BulletManager&) = delete;

};