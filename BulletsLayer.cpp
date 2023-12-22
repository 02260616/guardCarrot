#include "BulletsLayer.h"
#include "BulletManager.h"

bool BulletsLayer::Init()
{
    bool bRet = false;

    do
    {
        CC_BREAK_IF(!EntityLayerBase::init());

        scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void BulletsLayer::Update(float dt)
{
    BulletManager::getInstance()->checkBulletIsDead();
}