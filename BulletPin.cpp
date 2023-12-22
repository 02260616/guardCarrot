#include "BulletPin.h"
#include "VictimEntityBase.h"

bool BulletPin::Init(const int& Id, VictimEntityBase* VictimEntity)
{
	bool Ret = false;
	do
	{
		CC_BREAK_IF(!BulletBase::Init(Id, VictimEntity));

		Ret = true;
	} while (0);
	return Ret;
}

BulletPin* BulletPin::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	BulletPin* pBullet = new BulletPin();
	if (pBullet && pBullet->Init(Id, VictimEntity))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

void BulletPin::DoAction()
{

	for (int i = 1; i <= _IAnimationFrameCount; ++i)//依次获取钉子子弹动画的每一帧
	{
		std::string SpriteFrameName = _sModelName + StringUtils::format("%d.png", i);
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(SpriteFrameName));
	}
	pAnimation->setDelayPerUnit(0.05);//每一帧的播放时间间隔为 0.05 秒
	pAnimation->setLoops(-1);//无限循环
	getSprite()->runAction(Animate::create(pAnimation));
}

void BulletPin::DoMove() {

	if (pAtkTarget->getIsDead())//检查攻击目标是否已经死亡
	{
		removeFromParent();
		setIsDead(true);
		return;
	}
	DoAction();

	auto deadActionCF = CallFunc::create([=]()//处理子弹击中目标后
		{

			if (pAtkTarget != nullptr && !pAtkTarget->getIsDead())
				pAtkTarget->Hurt(GetAtkProperty());
			DeadMove(Name);

		});

	float Hypotenuse = Vec2::ZERO.distance(Director::getInstance()->getVisibleSize());//计算屏幕对角线的长度
	Vec2 MovetoPosition = pAtkTarget->getPosition() - this->getPosition();//计算子弹移动的方向向量
	float MoveDistance = this->getPosition().distance(pAtkTarget->getPosition());//计算子弹需要移动的距离
	this->runAction(Sequence::create(MoveBy::create(MoveDistance / mySpeed, MovetoPosition), deadActionCF, NULL));
}