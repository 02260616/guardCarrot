#pragma once
#ifndef __Carrot__BulletsLayer__
#define __Carrot__BulletsLayer__

#include "EntityLayerBase.h"

/**
 *  BulletsLayer use to show Bullet
 */

class BulletsLayer :public EntityLayerBase
{
public:

    CREATE_FUNC(BulletsLayer);

protected:

    virtual bool Init();

    virtual void Update(float dt);
};

#endif 
