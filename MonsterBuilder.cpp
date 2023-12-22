#include "MonsterBuilder.h"
#include "MapUtil.h"
#include "MonsterFactory.h"
#include "MonsterManager.h"
#include "LevelConfigUtil.h"
#include "MonsterBase.h"
#include "GameManager.h"
#include "SoundUtil.h"

bool MonsterBuilder::Init()
{
	bool Ret = false;
	do
	{
		CC_BREAK_IF(!Node::init());//如果Node初始化失败，则跳出循环
		myBuildNumber = 10;
		DelayTime = ConstDelayTime;
		MonsterCount = 0;
		MonsterBuilderNumber = 0;
		Delay = true;
		Number = 0;
		Batch = 0;
		GameManager::getInstance()->setMyMonsterBatch(0);//// 在游戏管理器中将怪物批次设置为0
		// 创建包含两个精灵帧的怪物动画，并设置属性
		createMonsterAnimation = Animation::create();
		createMonsterAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mcm01.png"));
		createMonsterAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mcm02.png"));
		createMonsterAnimation->setDelayPerUnit(0.15);
		createMonsterAnimation->setLoops(1);
		createMonsterAnimation->retain();
		// 从配置文件中读取怪物批次数量
		ReadFileMonsterCount = LevelConfigUtil::getInstance()->getCurLevelMonsterBatchCount();
		// 根据地图中的第一个移动位置设置MonsterBuilder的位置
		this->setPosition(MapUtil::getInstance()->getMovePoses().at(0) + Vec2(40, -15));

		Ret = true;
	} while (0);
	return Ret;
}

void MonsterBuilder::OnEnter()
{
	Node::onEnter(); 
	schedule(schedule_selector(MonsterBuilder::CreateMonsterUpdate), 1.5, 20001, 3); // 调度一个定时器，每隔1.5秒执行一次，重复次数为20001（即很大的值，相当于无限循环），优先级为3
}

void MonsterBuilder::CreateMonsterUpdate(float t)
{
	if (MonsterBuilderNumber == 10 * ReadFileMonsterCount)// 如果MonsterBuilderNumber达到了一定数量，播放音效，取消定时器，并发送通知打开游戏结束检查
	{
		SoundUtil::getInstance()->playEffect(FINALWAVE);
		this->unschedule(schedule_selector(MonsterBuilder::CreateMonsterUpdate));
		NOTIFY->postNotification("openCheckGameEnd");
	}
	if (DelayTime < ConstDelayTime && MonsterManager::getInstance()->getMonsterVec().size() == 0)// 如果DelayTime小于ConstDelayTime，并且MonsterManager中没有怪物存在，则增加DelayTime
	{
		DelayTime += t;
	}
	else if (DelayTime >= ConstDelayTime)// 如果DelayTime大于等于ConstDelayTime
	{
		Batch = MonsterBuilderNumber / 10 + 1;// 计算当前怪物批次
		GameManager::getInstance()->setIMonsterBatch(Batch);// 在游戏管理器中设置当前怪物批次
		NOTIFY->postNotification("monsterBatchChange", (Ref*)&Batch);// 发送怪物批次变化的通知
		MonsterFactoryController();// 调用怪物工厂控制器生成怪物
		MonsterCount++;
		MonsterBuilderNumber++;
		if (MonsterCount == 10)// 如果MonsterCount达到10，重置DelayTime和MonsterCount
		{
			DelayTime = 0;
			MonsterCount = 0;
		}
	}
}

void MonsterBuilder::MonsterFactoryController()
{
	SoundUtil::getInstance()->playEffect(BUILDMONSTER);
	if (Number == 10)Number = 0;// 如果Number达到10，重置为0
	int number = 10 - (Batch - 1) * 2;// 根据批次计算当前可以生成的怪物种类数量
	MonsterBase* pMonster;
	if (Batch == ReadFileMonsterCount && Number == 5)// 根据条件选择生成普通怪物或Boss怪物
	{
		pMonster = MonsterFactory::CreateMonster(en_Boss_Big);
	}
	else
	{
		if (Number < number)// 根据Number和计算得到的number，选择生成不同种类的普通怪物
		{
			pMonster = MonsterFactory::CreateMonster(MonsterType(1 + int(Batch / 5)));
		}
		else
		{
			pMonster = MonsterFactory::CreateMonster(MonsterType(4 + int(Batch / 5)));
		}
	}
	pMonster->setMyHp(pMonster->getMyHp() + (Batch - 1) / 5 * pMonster->getMyHp());// 根据批次设置怪物的初始血量和最大速度
	pMonster->SetMaxSpeed(pMonster->getMySpeed() + int(Batch / 5) * 10);
	pMonster->getSprite()->runAction(Animate::create(createMonsterAnimation));// 播放怪物生成动画
	Number++; //Number递增，准备生成下一个怪物
}

int MonsterBuilder::GetBatch()
{
	return Batch;
}

MonsterBuilder::~MonsterBuilder()
{
	CC_SAFE_RELEASE_NULL(createMonsterAnimation);
}