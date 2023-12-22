#include "Carrot.h"
#include "time.h"
#include "MapUtil.h"
#include "MonsterBase.h"
#include "MonsterManager.h"
#include "GameManager.h"
#include "SoundUtil.h"

Carrot::~Carrot()
{
	NOTIFY->removeAllObservers(this);
}

bool Carrot::Init()
{
	bool ret = false;

	do
	{
		CC_BREAK_IF(!Entity::Initiate(18, MONSTERCSVFILE));

		myHp = 10;//设置胡萝卜的初始血量为 10。

		GameManager::getInstance()->setMyCarrotHp(myHp);//设置游戏管理器中的胡萝卜血量属性
		auto tPoints = MapUtil::getInstance()->getMovePoses();//获取移动路径的坐标点
		getSprite()->setAnchorPoint(Vec2(0.5, 0));//设置胡萝卜精灵的锚点
		this->setPosition((*(tPoints.end() - 2)).x + getContentSize().width / 2 - 20, (*(tPoints.end() - 2)).y - getContentSize().height / 2);//设置胡萝卜的初始位置
		HpSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BossHP10.png"));//创建显示血量的精灵
		HpSprite->setPosition(*(tPoints.end() - 1) - *(tPoints.end() - 2) + Vec2(-20, 10));//血量精灵的位置
		addChild(HpSprite);

		NOTIFY->addObserver(this, callfuncO_selector(Carrot::CarrotBeHurt), "CarrotHurt", nullptr);//注册观察者,监听 "CarrotHurt"
		RegisterCarrotTouchEvent();//注册与胡萝卜相关的触摸事件

		srand(time(0));
		schedule(schedule_selector(Carrot::DoAction), 6);//调度 DoAction，每隔 6 秒执行一次

		ret = true;
	} while (0);

	return  ret;
}

void Carrot::RegisterCarrotTouchEvent()
{
	auto pTouchEvent = EventListenerTouchOneByOne::create();//创建一个单点触摸事件监听器

	pTouchEvent->onTouchBegan = [=](Touch* t, Event* e)//当触摸开始时
		{
			if (getBoundingBox().containsPoint(t->getLocation())) {//如果触摸点在胡萝卜的边界框内
				auto pCarrot = getSprite();
				pCarrot->stopAllActions();
				pCarrot->setRotation(0);
				pCarrot->setTexture(SpriteFrameCache::getInstance()->getSpriteFrameByName(_sModelName + PHOTOPOSTFIX)->getTexture());//设置胡萝卜精灵的纹理为正常状态的纹理
				pCarrot->runAction(DoShakeOrBlink(10, 9));//运行抖动或闪烁的动作
			}
			return false;
		};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchEvent, getSprite());//将触摸监听器添加到场景图优先级中，绑定到胡萝卜精灵上
}

ActionInterval* Carrot::DoShakeOrBlink(int ImageFirstNumber, int ImageCount)
{
	auto cache = SpriteFrameCache::getInstance();
	Animation* pAnimation = Animation::create();
	std::string sSpriteFrameName;
	for (int i = ImageFirstNumber; i < ImageFirstNumber + ImageCount; i++)//// 循环添加指定范围内的精灵帧到动画中
	{
		std::string sSpriteFrameName = Name + StringUtils::format("%2d.png", i);
		pAnimation->addSpriteFrame(cache->getSpriteFrameByName(sSpriteFrameName));
	}
	pAnimation->addSpriteFrame(cache->getSpriteFrameByName(_sModelName + PHOTOPOSTFIX));//// 添加最后一帧，用于显示最终状态
	// 设置动画的播放间隔和循环次数
	pAnimation->setDelayPerUnit(0.1);
	pAnimation->setLoops(1);
	Animate* pAnimate = Animate::create(pAnimation);
	return pAnimate;
}

void Carrot::DoAction(float t)
{
	getSprite()->stopAllActions();
	int tRandom = rand() % 3;
	switch (tRandom)
	{
	case 0: getSprite()->runAction(DoShakeOrBlink(10, 9));
		break;
	case 1: getSprite()->runAction(DoShakeOrBlink(21, 3));
		break;
	case 2: getSprite()->runAction(Sequence::create(RotateBy::create(0.2, -20), RotateBy::create(0.2, 20), RotateBy::create(0.2, 30), RotateBy::create(0.2, -30), NULL));//// 执行一系列旋转动作，使得萝卜产生摆动效果
		break;
	default:
		break;
	}
	SoundUtil::getInstance()->playEffect(StringUtils::format(CARROT, tRandom + 1));
}

void Carrot::CarrotBeHurt(Ref* pRef)
{
	SoundUtil::getInstance()->playEffect(CARROTCRASH);// 播放受伤音效
	MonsterBase* pMonster = (MonsterBase*)pRef;// 将传入的指针转换为怪物对象
	myHp -= pMonster->getMyAtk();// 减去怪物的攻击力值
	if (myHp <= 0)// 如果萝卜生命值小于等于0，表示游戏失败
	{
		GameManager::getInstance()->setICarrotHp(0);
		auto tGameEndType = en_GameLose;
		NOTIFY->postNotification("gameEnd", reinterpret_cast<Ref*>(&tGameEndType));
		NOTIFY->removeAllObservers(this);// 移除所有与萝卜相关的观察者
		return;
	}
	GameManager::getInstance()->setMyCarrotHp(myHp);// 更新萝卜生命值
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(getSprite());// 移除萝卜的触摸事件监听器和所有定时器
	unscheduleAllSelectors();
	HpSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("BossHP%02d.png", myHp)));// 更新萝卜生命值显示
	std::string HurtSpriteName;
	// 根据当前生命值选择不同的受伤图片
	if (myHp == 5 || myHp == 7)
	{
		HurtSpriteName = StringUtils::format("hlb%d.png", myHp + 1);
	}
	else
	{
		HurtSpriteName = StringUtils::format("hlb%d.png", myHp);
	}
	getSprite()->stopAllActions();// 停止所有动作，并设置受伤图片
	getSprite()->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(HurtSpriteName));
}

Rect Carrot::GetBoundingBox()const
{
	return Rect(getPositionX() - getContentSize().width / 2, getPositionY(), getContentSize().width, getContentSize().height);
}