#include "MonsterFactory.h"
#include "MonsterBase.h"

MonsterBase* MonsterFactory::CreateMonster(const MonsterType& MonsterType)
{
	MonsterBase* pMonster = nullptr;

	int MosterId = 0;
	switch (MonsterType) {
	case en_Land_Pink:MosterId = 2;
		break;
	case en_Land_Star:MosterId = 1;
		break;
	case en_Land_Nima:MosterId = 3;
		break;
	case en_Fly_Yellow:MosterId = 7;
		break;
	case en_Fly_Blue:MosterId = 8;
		break;
	case en_Fat_Green:MosterId = 11;
		break;
	case en_Land_Boss_Pink:MosterId = 5;
		break;
	case en_Land_Boss_Star:MosterId = 4;
		break;
	case en_Land_Boss_Nima:MosterId = 6;
		break;
	case en_Fly_Boss_Yellow:MosterId = 9;
		break;
	case en_Fly_Boss_Blue:MosterId = 10;
		break;
	case en_Fat_Boss_Green:MosterId = 12;
		break;
	case en_Big_Oldboss:MosterId = 14;
		break;
	case en_Fat_Oldboss:MosterId = 15;
		break;
	case en_Fly_Oldboss:MosterId = 16;
		break;
	case en_Land_Oldboss:MosterId = 17;
		break;
	case en_Boss_Big:MosterId = 13;
		break;
	default:
		break;
	}
	pMonster = MonsterBase::Create(MosterId);

	return pMonster;
}