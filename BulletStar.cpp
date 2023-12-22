#include "BulletStar.h"
#include "VictimEntityBase.h"
#include "MonsterManager.h"
#include "MonsterBase.h"
#include "BarrierManager.h"
#include "BarrierBase.h"

bool BulletStar::Init(const int& Id, VictimEntityBase* VictimEntity)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BulletBase::Init(Id, VictimEntity));

		bRet = true;
	} while (0);
	return bRet;
}

BulletStar* BulletStar::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	BulletStar* pBullet = new BulletStar();
	if (pBullet && pBullet->Init(Id, VictimEntity))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

void BulletStar::DoAction()
{
	getSprite()->runAction(RepeatForever::create(RotateBy::create(0.7, 360)));//永久让子弹旋转360
}

void BulletStar::DoMove() {
	if (pAtkTarget->getIsDead())
	{
		removeFromParent();
		setIsDead(true);
		return;
	}
	DoAction();

	Animation* pAnimation = Animation::create();
	for (int i = 1; i < 7; i++)// 添加子弹的飞行动画帧
	{
		std::string SpriteFrameName = StringUtils::format("PStar-1%d.png", i);
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(SpriteFrameName));
	}
	// 设置帧动画的播放速度和循环次数
	pAnimation->setDelayPerUnit(0.05);
	pAnimation->setLoops(1);
	// 创建一个回调动作，用于处理子弹击中目标后的逻辑
	auto deadActionCF = CallFunc::create([=]()
		{
			Rect rect = Rect(pAtkTarget->getPositionX() - 100, pAtkTarget->getPositionY() - 100, 200, 200);// 创建一个矩形区域，用于检测子弹是否击中目标
			if (pAtkTarget->getAnimationFrameCount() != 0)// 根据攻击目标的动画帧数量判断是怪物还是障碍物,如果是怪物
			{
				Vector<MonsterBase*>  MonsterVector = Vector<MonsterBase*>(MonsterManager::getInstance()->getMonsterVec());
				for (auto mIter = MonsterVector.begin(); mIter != MonsterVector.end();)
				{
					MonsterBase* pMonster = (MonsterBase*)(*mIter);// 获取当前怪物对象
					if (rect.intersectsRect(pMonster->getBoundingBox()))//检查当前怪物的边界框是否与指定的矩形区域 rect 相交
					{
						pMonster->Hurt(GetAtkProperty()); //处理怪物的受伤					
					}
					if (pMonster->getMyHp() <= 0 || pMonster->getIsDead())//检查是否死亡
					{
						MonsterVector.eraseObject(pMonster);
					}
					else
					{
						++mIter; //如果怪物未死亡，将迭代器递增，继续处理下一个怪物
					}
				}
			}
			else//如果为障碍物
			{
				Vector<BarrierBase*>  BarrierVector = Vector<BarrierBase*>(BarrierManager::getInstance()->getBarrierVec());
				for (auto bIter = BarrierVector.begin(); bIter != BarrierVector.end();)
				{
					BarrierBase* pBarrier = (BarrierBase*)(*bIter);
					if (rect.intersectsRect(pBarrier->getBoundingBox()))
					{
						pBarrier->Hurt(GetAtkProperty());
					}
					if (pBarrier->getMyHp() <= 0 || pBarrier->getIsDead())
					{
						/*bIter =*/ BarrierVector.eraseObject(pBarrier);
					}
					else
					{
						++bIter;
					}
				}
			}
			DeadMove(Name); // 执行子弹死亡动作
		});
	// 计算子弹的移动方向和距离，并执行动作序列
	Vec2 MovetoPosition = pAtkTarget->getPosition() - this->getPosition();
	float MoveDistance = this->getPosition().distance(pAtkTarget->getPosition());
	getSprite()->runAction(Sequence::create(MoveBy::create(MoveDistance / mySpeed, MovetoPosition), Animate::create(pAnimation), deadActionCF, NULL));
}

