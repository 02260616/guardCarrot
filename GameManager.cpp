#include "GameManager.h"

GameManager* GameManager::_gInstance;


bool GameManager::Init()
{
    myIsPause = false;
    return true;
}