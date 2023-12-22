#pragma once
#ifndef __Carrot__DynamicPropertyInterFace__
#define __Carrot__DynamicPropertyInterFace__

#include "PublicDefine.h"

class DynamicPropertyInterFace
{
public:

    virtual ~DynamicPropertyInterFace();

protected:

    virtual void CreateDynamicEntityAnimation(const std::vector<std::string>& AnimationFiles);//创建动态实体的动画

    virtual bool Init(const int& Id, const std::string& SCsvFileName);//初始化
   
    virtual void DoAction(const float& Dt) = 0;
   
    CC_SYNTHESIZE_PASS_BY_REF(unsigned int, mySpeed, MySpeed);
   
    CC_SYNTHESIZE_PASS_BY_REF(int, myAtk, MyAtk);

protected:

    Animation* pAnimation = nullptr;//存储动态实体的动画

};
#endif