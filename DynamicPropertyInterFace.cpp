#include "DynamicPropertyInterFace.h"

DynamicPropertyInterFace::~DynamicPropertyInterFace()
{
    CC_SAFE_RELEASE_NULL(pAnimation);
}

bool DynamicPropertyInterFace::Init(const int& Id, const std::string& SCsvFileName)
{
    bool Ret = false;
    do
    {
        auto CsvUtil = CsvUtil::getInstance();//读取 CSV 文件中的数据
        pAnimation = Animation::create();
        pAnimation->retain();//确保在后续使用中不会被提前释放
        myAtk = CsvUtil->getInt(Id, en_Atk, SCsvFileName);//获得攻击数据
        mySpeed = CsvUtil->getInt(Id, en_Speed,SCsvFileName);//获得速度
        Ret = true;
    } while (0);

    return Ret;
}

void DynamicPropertyInterFace::CreateDynamicEntityAnimation(const std::vector<std::string>& AnimationFiles)
{
    auto SpriteFrameCache = SpriteFrameCache::getInstance();//管理和缓存精灵帧
    for (auto& animationFile : AnimationFiles)//遍历 AnimationFiles 中的每一个动画文件名
        pAnimation->addSpriteFrame(SpriteFrameCache->getSpriteFrameByName(animationFile));//从缓存中获取指定文件名的精灵帧，并将其添加到动画对象 pAnimation 中
    pAnimation->setLoops(-1); //将动画设置为无限循环播放
}
