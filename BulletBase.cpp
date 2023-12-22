#include "BulletBase.h"
#include "BulletManager.h"
#include "VictimEntityBase.h"

BulletBase* BulletBase::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	auto Bullet = new BulletBase();
	if (Bullet && Bullet->Init(Id, VictimEntity)) Bullet->autorelease();
	else CC_SAFE_DELETE(Bullet);
	return Bullet;
}

bool BulletBase::Init(const int& _Id, VictimEntityBase* VictimEntity)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!Entity::Initiate(Id, BULLETCSVFILE));
		CC_BREAK_IF(!DynamicPropertyInterFace::Init(_Id, BULLETCSVFILE));

		myAtkState = CsvUtil::GetInstance()->getInt(Id, en_AtkState, BULLETCSVFILE);//从CSV文件中获取并设置AtkState属性
		myBulletType = CsvUtil::GetInstance()->getInt(Id, en_BulletType, BULLETCSVFILE);// 从CSV文件中获取并设置BulletType属性
		myDeadActCount = CsvUtil::GetInstance()->getInt(Id, en_DeadActCount, BULLETCSVFILE);// 从CSV文件中获取并设置DeadActCount属性
		CC_SAFE_RETAIN(VictimEntity);
		pAtkTarget = VictimEntity;//// 保留目标实体对象的引用
		IdMap = new std::set<int>();
		BulletManager::getInstance()->addBullet(this);// 将BulletBase实例添加到BulletManager

		bRet = true;
	} while (0);

	return  bRet;
}

void BulletBase::DoAction(const float& rDt)
{
	switch (myBulletType)
	{
	case en_NormalBullet:
	case en_GunBullet:
	{
		std::vector<std::string> AnimationFiles;// // 创建动画文件路径列表
		for (int i = 1; i <= _IAnimationFrameCount; ++i)//
			AnimationFiles.push_back(_sModelName + StringUtils::format("%d", i) + PHOTOPOSTFIX);//
		//// 创建并设置动画
		CreateDynamicEntityAnimation(AnimationFiles);
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_sModelName + "1" + PHOTOPOSTFIX));
		pAnimation->setDelayPerUnit(rDt);
		GetSprite()->runAction(Animate::create(pAnimation));//// 播放动画
		break;
	}
	case en_NoDisspper:
	{
		GetSprite()->runAction(RepeatForever::create(RotateBy::create(0.7, 360)));// 以0.7秒的速度不断旋转
		break;
	}
	default:
		break;
	}
}

void BulletBase::DoMove() {
	if (pAtkTarget->getIsDead()) // 如果攻击目标已经死亡，移除子弹并设置为死亡状态
	{
		removeFromParent();
		setIsDead(true);
		return;
	}
	DoAction(0.2f);// 执行子弹的动作，以0.2秒的延迟

	auto deadActionCF = CallFunc::create([=]()///定义一个动作序列，包括移动和死亡动作
		{
			if (myBulletType == en_NormalBullet || myBulletType == en_GunBullet || myBulletType == en_StarBullet) {
				if (pAtkTarget != nullptr && !pAtkTarget->getIsDead())// 如果攻击目标存在且未死亡，对目标造成伤害
					pAtkTarget->Hurt(GetAtkProperty());
				this->setPosition(pAtkTarget->getPosition());//// 设置子弹位置为攻击目标的位置
			}
			DeadMove(Name);//// 执行死亡动作
		});

	Vec2 MovetoPosition;//// 计算移动位置和距离，根据子弹类型不同采用不同的计算方式
	float MoveDistance;
	switch (myBulletType)
	{
	case en_NormalBullet:
	case en_GunBullet:
		MovetoPosition = pAtkTarget->getPosition() - this->getPosition(); //计算从子弹当前位置指向攻击目标位置的向量
		MoveDistance = this->getPosition().distance(pAtkTarget->getPosition()); //计算子弹当前位置与攻击目标位置的距离
		break;
	case en_NoDisspper:
	{
		float Hypotenuse = Vec2::ZERO.distance(Director::getInstance()->getVisibleSize());
		MovetoPosition = Vec2(Hypotenuse * sin(this->getRotation() * M_PI / 180), Hypotenuse * cos(this->getRotation() * M_PI / 180));//计算一个向量，指向以屏幕中心为原点、子弹当前旋转角度为方向的位置
		MoveDistance = MovetoPosition.distance(this->getPosition());
		break;
	}
	default:
		break;
	}
	this->runAction(Sequence::create(MoveBy::create(MoveDistance / mySpeed, MovetoPosition), deadActionCF, NULL));//// 执行移动和死亡动作序列
}

AtkProperty BulletBase::GetAtkProperty()
{
	AtkProperty myAtkProperty;
	myAtkProperty._enAtkState = myAtkState;
	myAtkProperty.Atk = getMyAtk();
	myAtkProperty.Duration = CsvUtil::getInstance()->getInt(Id, en_Duration, BULLETCSVFILE);//
	myAtkProperty.ButtltType = myBulletType;
	return myAtkProperty;
}

void BulletBase::DeadMove(const std::string& DeadImageFile)
{
	delete(IdMap);
	Entity::DeadMove(DeadImageFile);//
}

std::set<int>& BulletBase::GetIdMap()
{
	return *IdMap;
}
