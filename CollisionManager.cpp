#include "CollisionManager.h"
#include "BarrierBase.h"
#include "MonsterBase.h"
#include "BulletBase.h"


void CollisionManager::Collision(Vector<BarrierBase*>& BarrierVec, Vector<MonsterBase*>& MonsterVec, Vector<BulletBase*>& BulletVec)
{
	for (int i = 0; i < BulletVec.size(); ++i)
	{
		BulletBase* pBullet = BulletVec.at(i);
		if (pBullet == nullptr || pBullet->getIsDead() || pBullet->getMyBulletType() != 2)
			continue;
		MonsterAndBullet(pBullet, MonsterVec);
		BarrierAndBullet(pBullet, BarrierVec);
	}
}

void CollisionManager::MonsterAndBullet(BulletBase* Bullet, Vector<MonsterBase*>& MonsterVec)
{
	for (int i = 0; i < MonsterVec.size(); ++i)
	{
		MonsterBase* pMonster = MonsterVec.at(i);
		if (Bullet == nullptr || Bullet->getIsDead() || pMonster == nullptr || pMonster->getIsDead())
			continue;
		if (Bullet->getBoundingBox().intersectsRect(pMonster->getBoundingBox()))
		{
			if (Bullet->GetIdMap().find(pMonster->getID()) == Bullet->GetIdMap().end())
			{
				Bullet->GetIdMap().insert(pMonster->getID());
				pMonster->Hurt(Bullet->getAtkPro());
				Sprite* HurtSprite = Sprite::create();
				pMonster->addChild(HurtSprite);
				CallFunc* ClearF = CallFunc::create([=]() {HurtSprite->removeFromParentAndCleanup(true); });
				HurtSprite->runAction(Sequence::create(Animate::create(AnimationMaker(Bullet->getName(), Bullet->getAnimationFrameCount() + 1)), ClearF, NULL));
			}
		}
	}
}
void CollisionManager::BarrierAndBullet(BulletBase* Bullet, Vector<BarrierBase*>& BarrierVec)
{
	for (int i = 0; i < BarrierVec.size(); ++i)
	{
		BarrierBase* pBarrier = BarrierVec.at(i);
		if (Bullet == nullptr || Bullet->getIsDead() || pBarrier == nullptr || pBarrier->getIsDead())
			continue;
		if (Bullet->getBoundingBox().intersectsRect(pBarrier->etBoundingBox()))
		{
			if (Bullet->GetIdMap().find(pBarrier->getID()) == Bullet->GetIdMap().end())
			{
				Bullet->GetIdMap().insert(pBarrier->getmID());
				pBarrier->Hurt(Bullet->getAtkPro());
				Sprite* HurtSprite = Sprite::create();
				pBarrier->addChild(HurtSprite);
				CallFunc* ClearF = CallFunc::create([=]() {HurtSprite->removeFromParentAndCleanup(true); });
				HurtSprite->runAction(Sequence::create(Animate::create(AnimationMaker(Bullet->getName(), Bullet->getAnimationFrameCount() + 1)), ClearF, NULL));
				if (pBarrier->GetAtkTarget())
				{
					Bullet->GetIdMap().erase(pBarrier->getID());
					Bullet->setPosition(pBarrier->GetPosition());
					Bullet->stopAllActions();
					Bullet->DeadMove();
					return;
				}
			}
		}
	}
}

Animation* CollisionManager::AnimationMaker(std::string BulletName, int SpriteFrameCount)
{
	Animation* pAnimation = Animation::create();
	if (SpriteFrameCount == 2)
	{
		for (int i = 1; i <= SpriteFrameCount; i++)
		{
			std::string SpriteFrameName = BulletName + StringUtils::format("%02d.png", i);
			pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(SpriteFrameName));
		}
	}
	else
	{
		for (int i = 1; i <= SpriteFrameCount; i++)
		{
			std::string SpriteFrameName = BulletName + StringUtils::format("-1%d.png", i);
			pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(SpriteFrameName));
		}
	}
	pAnimation->setDelayPerUnit(0.1);
	pAnimation->setLoops(1);
	return pAnimation;
}