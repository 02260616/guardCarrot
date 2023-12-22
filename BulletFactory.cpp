#include "BulletFactory.h"
#include "BulletBase.h"
#include "VictimEntityBase.h"
#include "TowerBase.h"


BulletBase* BulletFactory::CreateBullet(const int& BulletId, TowerBase* Tower, VictimEntityBase* VictimEntity)
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Themes/Towers/TBottle-hd.plist", "Themes/Towers/TBottle-hd.png");
	cache->addSpriteFramesWithFile("Themes/Towers/TFan-hd.plist", "Themes/Towers/TFan-hd.png");
	cache->addSpriteFramesWithFile("Themes/Towers/TShit-hd.plist", "Themes/Towers/TShit-hd.png");

	auto pBullet = BulletBase::Create(BulletId, VictimEntity);//根据子弹类型加载不同的精灵帧
	pBullet->setRotation(90 - 180 * (VictimEntity->getPosition() - Tower->getPosition()).getAngle() / M_PI);// 设置子弹的旋转角度，使其朝向目标实体
	if (pBullet->getMyBulletType() == en_GunBullet) {//// 根据子弹类型设置子弹的初始位置
		pBullet->setPosition(Tower->getPosition() + Vec2(36 * sin(pBullet->getRotation() * M_PI / 180), 36 * cos(pBullet->getRotation() * M_PI / 180)));
	}
	else {
		pBullet->setPosition(Tower->getPosition());
	}

	pBullet->DoMove();// 执行子弹的移动行为
	return pBullet;
}