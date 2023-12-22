#ifndef __BulletManager__
#define __BulletManager__


#include "BulletManager.h"
#include "BulletBase.h"

BulletManager* BulletManager::_gInstance;

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
    CC_SAFE_DELETE(pBulletVec);
}

bool BulletManager::Init()
{
    bool Ret = false;
    do
    {
        pBulletVec = new Vector<BulletBase*>();// 创建一个 Vector 容器来存储子弹对象的指针

        Ret = true;
    } while (0);

    return Ret;
}

Vector<BulletBase*>& BulletManager::GetBulletVec()
{
    return *pBulletVec;
}

void BulletManager::AddBullet(BulletBase* Bullet)
{
    if (pBulletVec) pBulletVec->pushBack(Bullet);
    funcAddBulletLayer(Bullet);
}

void BulletManager::SetFuncAddBulletLayer(const std::function<void(Entity*)>& FuncAddBulletLayer)
{
    funcAddBulletLayer = FuncAddBulletLayer;
}

void BulletManager::CheckBulletIsDead()
{
    for (auto iterBullet = pBulletVec->begin(); iterBullet != pBulletVec->end();)//使用迭代器遍历子弹列表
    {
        if ((*iterBullet)->IsDead()) iterBullet = pBulletVec->erase(iterBullet);//如果子弹已死亡，从子弹列表中移除当前子弹，并返回下一个有效的迭代器
        else ++iterBullet;
    }
}

void BulletManager::ClearManager()
{
    pBulletVec->clear();
}

#endif