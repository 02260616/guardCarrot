#include "MoveControllerBase.h"
#include "MapUtil.h"
#define STEP 10.0

bool MoveControllerBase::Init(MonsterBase* Momster)
{
	if (!Node::init()) {// 确保初始化成功
		return false;
	}

	SetMonster(Momster);// 设置与怪物关联
	myMonsterSpeed = pMonster->getMySpeed();
	pMonster->addChild(this);// 将移动控制器添加到怪物节点上

	auto Points = MapUtil::getInstance()->getMovePoses();//获取地图上的移动点

	pMapPonits = new std::vector<Vec2>();// 初始化移动点容器

	for (int i = 0; i < Points.size(); i++) {// 根据怪物级别调整移动点位置
		Vec2 t_point;
		if (pMonster->getGrade() == 1) t_point.x = Points.at(i).x + pMonster->getContentSize().width / 2 - 15;
		if (pMonster->getGrade() == 2) t_point.x = Points.at(i).x + 30;
		if (pMonster->getGrade() == 3) t_point.x = Points.at(i).x + 40;
		t_point.y = Points.at(i).y - 5;// 调整纵坐标
		pMapPonits->push_back(t_point);// 添加到移动点容器中
	}
	pMonster->setPosition(*pMapPonits->begin());// 设置怪物起始位置为第一个移动点

	ListenerMonster(STEP / pMonster->getMySpeed());// 开始监听怪物移动

	return true;
}

MoveControllerBase* MoveControllerBase::Create(MonsterBase* Momster) {
	auto cMonster = new MoveControllerBase();
	if (cMonster && cMonster->Init(Momster)) {
		cMonster->autorelease();
		return cMonster;
	}
	CC_SAFE_DELETE(cMonster);
	return nullptr;
}

void MoveControllerBase::SetMonster(MonsterBase* Monster)
{
	pMonster = Monster;
}

void MoveControllerBase::ListenerMonster(float t)
{
	if (pMonster->getMyHp() <= 0 || pMonster->getIsDead())return;///判断怪物是否已经死亡或者生命值为0，如果是，则直接返回，不进行移动和处理状态
	// 移动到下一个路径点
	ErasePoint();
	Vec2 pot = *pMapPonits->begin();
	// 根据路径点位置设置怪物朝向
	if (pot.x < pMonster->getPositionX()) {
		pMonster->setMyIsLeft(true);
	}
	else {
		pMonster->setMyIsLeft(false);
	}
	MoveBy* pStep; // 根据怪物当前位置和路径点位置计算移动步骤
	if (pMonster->getPositionX() == pot.x) {
		if (pMonster->getPositionY() <= pot.y) {
			pStep = MoveBy::create(t, Vec2(0, STEP));
		}
		else {
			pStep = MoveBy::create(t, Vec2(0, -STEP));
		}
	}
	else {
		if (pMonster->getPositionX() <= pot.x) {
			pStep = MoveBy::create(t, Vec2(STEP, 0));
		}
		else {
			pStep = MoveBy::create(t, Vec2(-STEP, 0));
		}
	}
	// 移动完成后的回调函数
	auto pMoveCall = CallFuncN::create([=](Node* pSender) {
		this->ListenerMonster(STEP / pMonster->getMySpeed());
		});
	// 构建动作序列
	Sequence* seq = Sequence::create(pStep, pMoveCall, NULL);
	if (!pMonster->getMyIsLeft()) {// 根据怪物朝向设置旋转角度
		pMonster->getSprite()->setRotationSkewY(0);
	}
	else {
		pMonster->getSprite()->setRotationSkewY(180);
	}
	// 处理怪物状态
	int MonsterState = pMonster->getMyState();
	int State_Slow = MonsterState & 2;
	int State_Stop = MonsterState & 4;
	int State_Poison = MonsterState & 8;
	if (State_Slow && pMonster->getMySlowTime() > 0)// 减速状态
	{
		pMonster->setMySpeed(myMonsterSpeed / 3);
		float StateTime = pMonster->getMySlowTime() - t;
		if (StateTime < 0) 
			StateTime = 0;
		pMonster->setMySlowTime(StateTime);
		Sprite* pTemp = Sprite::create();// 创建减速状态的动画
		pTemp->setPosition(pMonster->getContentSize().width / 2, 40);
		pMonster->getSprite()->addChild(pTemp);
		Animation* pAnimation = AnimationMaker(pMonster->getMyBulletStateType(), t);
		auto pCallFunc = CallFunc::create([=]() {// 减速状态动画完成后的回调函数
			pTemp->removeFromParentAndCleanup(true);
			if (pMonster->getMySlowTime() <= 0)
			{
				pMonster->setMySpeed(myMonsterSpeed);
				pMonster->setMyState(pMonster->getMyState() & 13);
			}
			});
		Sequence* pSequence = Sequence::create(Animate::create(pAnimation), pCallFunc, NULL);// 创建动作序列并运行动画
		pTemp->runAction(pSequence);
	}
	if (State_Poison && pMonster->getMyPoisonTime() > 0)// 中毒状态
	{
		float StateTime = pMonster->getMyPoisonTime() - t;
		if (StateTime < 0) StateTime = 0;
		pMonster->setMyPoisonTime(StateTime);
		AtkProperty atk;
		atk._enAtkState = 1;
		atk.Atk = 2;
		atk.Duration = 0;
		Sprite* pTemp = Sprite::create();// 创建中毒状态的动画
		pTemp->setPosition(pMonster->getContentSize().width / 2, 40);
		pMonster->getSprite()->addChild(pTemp);
		Animation* pAnimation = AnimationMaker(6, t);
		auto pCallFunc = CallFunc::create([=]() { // 中毒状态动画完成后的回调函数
			if (!pMonster->getIsDead()) pMonster->Hurt(atk);
			if (pMonster->getMyPoisonTime() <= 0)
			{
				pMonster->setMyState(pMonster->getMyState() & 7);
			}
			pTemp->removeFromParentAndCleanup(true);
			});
		pTemp->runAction(Sequence::create(Animate::create(pAnimation), pCallFunc, NULL));
	}
	if (State_Stop && pMonster->getMyStopTime() > 0) // 停滞状态
	{
		float StateTime = pMonster->getMyStopTime() - t;
		if (StateTime < 0) StateTime = 0;
		pMonster->setMyStopTime(StateTime);
		Sprite* pTemp = Sprite::create();
		pTemp->setPosition(pMonster->getContentSize().width / 2, 40);
		pMonster->getSprite()->addChild(pTemp);
		Animation* pAnimation = AnimationMaker(pMonster->getMyBulletStateType(), t);
		auto pCallFunc = CallFunc::create([=]() {
			if (pMonster->getMyStopTime() <= 0)
			{
				pMonster->setMyState(pMonster->getMyState() & 11);
			}
			this->ListenerMonster(STEP / pMonster->getMySpeed());
			pTemp->removeFromParentAndCleanup(true);
			});
		pTemp->runAction(Sequence::create(Animate::create(pAnimation), pCallFunc, NULL));
		return;
	}
	pMonster->runAction(seq);
}

void MoveControllerBase::ErasePoint() {
	if (pMapPonits->size() == 1) {// 检查目标点数组是否只剩下一个点
		return;
	}
	if (pMonster->getPosition() == *pMapPonits->begin()) {// 如果怪物的位置等于当前目标点，说明怪物已经到达该点
		pMapPonits->erase(pMapPonits->begin());// 从目标点数组中擦除该点
		if (pMapPonits->size() == 1)// 如果目标点数组只剩下一个点，表示怪物已经到达终点
		{
			NOTIFY->postNotification("CarrotHurt", pMonster);// 发送"CarrotHurt"通知，表示怪物对萝卜产生伤害
			NOTIFY->postNotification("MonsterDead", pMonster);// 发送"MonsterDead"通知，表示怪物死亡
			pMonster->setIsDead(true);  // 设置怪物为死亡状态
			pMonster->removeFromParentAndCleanup(true); // 从父节点中移除怪物并清理资源
		}
	}
}

MoveControllerBase::~MoveControllerBase()
{
	delete(pMapPonits);
}

Animation* MoveControllerBase::AnimationMaker(int BulletType, float t)
{
	Animation* pAnimation = Animation::create();
	if (BulletType == 1)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PShit-11.png"));
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PShit-12.png"));
		pAnimation->setDelayPerUnit(t / 2);
	}
	else if (BulletType == 5)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PBStar-21.png"));
		pAnimation->setDelayPerUnit(t);
	}
	else if (BulletType == 15 || BulletType == 30 || BulletType == 45)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PSnow01.png"));
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PSnow02.png"));
		pAnimation->setDelayPerUnit(t / 2);
	}
	else if (BulletType == 6)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PPin01.png"));
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PPin02.png"));
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PPin03.png"));
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("PPin04.png"));
		pAnimation->setDelayPerUnit(t / 4);
	}
	pAnimation->setLoops(1);
	return pAnimation;
}