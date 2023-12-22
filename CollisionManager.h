#pragma once
#ifndef __Carrot__CollisionManager__
#define __Carrot__CollisionManager__

#include "PublicDefine.h"

class CollisionManager
{
public:

	static void Collision(Vector<BarrierBase*>& BarrierVec, Vector<MonsterBase*>& MonsterVec, Vector<BulletBase*>& BulletVec);

protected:
	/**
	 * the Collision between Monsters and Bullets
	 */
	static void MonsterAndBullet(BulletBase* Bullet, Vector<MonsterBase*>& MonsterVec);
	/**
	 * the Collision between Barriers and bullets
	 */
	static void BarrierAndBullet(BulletBase* Bullet, Vector<BarrierBase*>& BarrierVec);
	/**
	 * just make the Animation
	 */
	static Animation* AnimationMaker(std::string BulletName, int SpriteFrameCount);

};

#endif /* defined(__CarrotFantasy__CollisionManager__) */
