#pragma once
#ifndef __Carrot__Carrot__
#define __Carrot__Carrot__

#include "Entity.h"

class Carrot :public Entity
{

public:

	CREATE_FUNC(Carrot);

	virtual Rect GetBoundingBox()const;//获取 Carrot的边界框

	virtual ~Carrot();

protected:

	virtual bool Init();
	/**
	 * shake or blink action
	 */
	ActionInterval* DoShakeOrBlink(int ImageFirstNumber, int ImageCount);//萝卜抖动或眨眼
	/**
	 * action update
	 */
	virtual void DoAction(float t);

	virtual void CarrotBeHurt(Ref* pRef);

	virtual void RegisterCarrotTouchEvent();//注册与胡萝卜相关的触摸事件

	Sprite* HpSprite;//胡萝卜的血量显示

	CC_SYNTHESIZE_PASS_BY_REF(int, myHp, MyHP);

};

#endif 