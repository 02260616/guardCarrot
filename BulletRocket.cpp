#include "BulletRocket.h"
#include "VictimEntityBase.h"

bool BulletRocket::Init(const int& Id, VictimEntityBase* VictimEntity)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletBase::Init(Id, VictimEntity));

		bRet = true;
	} while (0);
	return bRet;
}

BulletRocket* BulletRocket::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	BulletRocket* pBullet = new BulletRocket();
	if (pBullet && pBullet->Init(Id, VictimEntity))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

void BulletRocket::DoAction()
{

	for (int i = 1; i <= _IAnimationFrameCount; ++i)//获取火箭子弹的每一帧图片
	{
		std::string SpriteFrameName = _sModelName + StringUtils::format("%d.png", i);
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(SpriteFrameName));
	}
	pAnimation->setDelayPerUnit(0.05);//设置动画每一帧的播放间隔时间为 0.05 秒
	pAnimation->setLoops(-1);//无限循环
	getSprite()->runAction(Animate::create(pAnimation));
}

void BulletRocket::DoMove() {
	if (pAtkTarget->getIsDead())//检查被攻击目标是否已死亡
	{
		removeFromParent();
		setIsDead(true);
		return;
	}
	DoAction();

	float Hypotenuse = Vec2::ZERO.distance(Director::getInstance()->getVisibleSize());//计算屏幕对角线的长度
	Vec2 MovetoPosition = Vec2(Hypotenuse * sin(this->getRotation() * M_PI / 180), Hypotenuse * cos(this->getRotation() * M_PI / 180));//根据火箭子弹当前的旋转角度，计算子弹移动的目标位置，即根据角度计算位移向量
	float MoveDistance = MovetoPosition.distance(this->getPosition());//计算子弹当前位置到目标位置的距离

	this->runAction(MoveBy::create(MoveDistance / mySpeed, MovetoPosition));
}