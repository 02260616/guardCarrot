#include "MoveControllerBase.h"
#include "MapUtil.h"
#define STEP 10.0

bool MoveControllerBase::Init(MonsterBase* Momster)
{
	if (!Node::init()) {// ȷ����ʼ���ɹ�
		return false;
	}

	SetMonster(Momster);// ������������
	myMonsterSpeed = pMonster->getMySpeed();
	pMonster->addChild(this);// ���ƶ���������ӵ�����ڵ���

	auto Points = MapUtil::getInstance()->getMovePoses();//��ȡ��ͼ�ϵ��ƶ���

	pMapPonits = new std::vector<Vec2>();// ��ʼ���ƶ�������

	for (int i = 0; i < Points.size(); i++) {// ���ݹ��Ｖ������ƶ���λ��
		Vec2 t_point;
		if (pMonster->getGrade() == 1) t_point.x = Points.at(i).x + pMonster->getContentSize().width / 2 - 15;
		if (pMonster->getGrade() == 2) t_point.x = Points.at(i).x + 30;
		if (pMonster->getGrade() == 3) t_point.x = Points.at(i).x + 40;
		t_point.y = Points.at(i).y - 5;// ����������
		pMapPonits->push_back(t_point);// ��ӵ��ƶ���������
	}
	pMonster->setPosition(*pMapPonits->begin());// ���ù�����ʼλ��Ϊ��һ���ƶ���

	ListenerMonster(STEP / pMonster->getMySpeed());// ��ʼ���������ƶ�

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
	if (pMonster->getMyHp() <= 0 || pMonster->getIsDead())return;///�жϹ����Ƿ��Ѿ�������������ֵΪ0������ǣ���ֱ�ӷ��أ��������ƶ��ʹ���״̬
	// �ƶ�����һ��·����
	ErasePoint();
	Vec2 pot = *pMapPonits->begin();
	// ����·����λ�����ù��ﳯ��
	if (pot.x < pMonster->getPositionX()) {
		pMonster->setMyIsLeft(true);
	}
	else {
		pMonster->setMyIsLeft(false);
	}
	MoveBy* pStep; // ���ݹ��ﵱǰλ�ú�·����λ�ü����ƶ�����
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
	// �ƶ���ɺ�Ļص�����
	auto pMoveCall = CallFuncN::create([=](Node* pSender) {
		this->ListenerMonster(STEP / pMonster->getMySpeed());
		});
	// ������������
	Sequence* seq = Sequence::create(pStep, pMoveCall, NULL);
	if (!pMonster->getMyIsLeft()) {// ���ݹ��ﳯ��������ת�Ƕ�
		pMonster->getSprite()->setRotationSkewY(0);
	}
	else {
		pMonster->getSprite()->setRotationSkewY(180);
	}
	// �������״̬
	int MonsterState = pMonster->getMyState();
	int State_Slow = MonsterState & 2;
	int State_Stop = MonsterState & 4;
	int State_Poison = MonsterState & 8;
	if (State_Slow && pMonster->getMySlowTime() > 0)// ����״̬
	{
		pMonster->setMySpeed(myMonsterSpeed / 3);
		float StateTime = pMonster->getMySlowTime() - t;
		if (StateTime < 0) 
			StateTime = 0;
		pMonster->setMySlowTime(StateTime);
		Sprite* pTemp = Sprite::create();// ��������״̬�Ķ���
		pTemp->setPosition(pMonster->getContentSize().width / 2, 40);
		pMonster->getSprite()->addChild(pTemp);
		Animation* pAnimation = AnimationMaker(pMonster->getMyBulletStateType(), t);
		auto pCallFunc = CallFunc::create([=]() {// ����״̬������ɺ�Ļص�����
			pTemp->removeFromParentAndCleanup(true);
			if (pMonster->getMySlowTime() <= 0)
			{
				pMonster->setMySpeed(myMonsterSpeed);
				pMonster->setMyState(pMonster->getMyState() & 13);
			}
			});
		Sequence* pSequence = Sequence::create(Animate::create(pAnimation), pCallFunc, NULL);// �����������в����ж���
		pTemp->runAction(pSequence);
	}
	if (State_Poison && pMonster->getMyPoisonTime() > 0)// �ж�״̬
	{
		float StateTime = pMonster->getMyPoisonTime() - t;
		if (StateTime < 0) StateTime = 0;
		pMonster->setMyPoisonTime(StateTime);
		AtkProperty atk;
		atk._enAtkState = 1;
		atk.Atk = 2;
		atk.Duration = 0;
		Sprite* pTemp = Sprite::create();// �����ж�״̬�Ķ���
		pTemp->setPosition(pMonster->getContentSize().width / 2, 40);
		pMonster->getSprite()->addChild(pTemp);
		Animation* pAnimation = AnimationMaker(6, t);
		auto pCallFunc = CallFunc::create([=]() { // �ж�״̬������ɺ�Ļص�����
			if (!pMonster->getIsDead()) pMonster->Hurt(atk);
			if (pMonster->getMyPoisonTime() <= 0)
			{
				pMonster->setMyState(pMonster->getMyState() & 7);
			}
			pTemp->removeFromParentAndCleanup(true);
			});
		pTemp->runAction(Sequence::create(Animate::create(pAnimation), pCallFunc, NULL));
	}
	if (State_Stop && pMonster->getMyStopTime() > 0) // ͣ��״̬
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
	if (pMapPonits->size() == 1) {// ���Ŀ��������Ƿ�ֻʣ��һ����
		return;
	}
	if (pMonster->getPosition() == *pMapPonits->begin()) {// ��������λ�õ��ڵ�ǰĿ��㣬˵�������Ѿ�����õ�
		pMapPonits->erase(pMapPonits->begin());// ��Ŀ��������в����õ�
		if (pMapPonits->size() == 1)// ���Ŀ�������ֻʣ��һ���㣬��ʾ�����Ѿ������յ�
		{
			NOTIFY->postNotification("CarrotHurt", pMonster);// ����"CarrotHurt"֪ͨ����ʾ������ܲ������˺�
			NOTIFY->postNotification("MonsterDead", pMonster);// ����"MonsterDead"֪ͨ����ʾ��������
			pMonster->setIsDead(true);  // ���ù���Ϊ����״̬
			pMonster->removeFromParentAndCleanup(true); // �Ӹ��ڵ����Ƴ����ﲢ������Դ
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