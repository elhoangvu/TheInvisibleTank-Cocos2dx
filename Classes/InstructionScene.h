#ifndef __INSTRUCTION_SCENE_H__
#define __INSTRUCTION_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"

USING_NS_CC;

class InstructionScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void popScene();
	CREATE_FUNC(InstructionScene);

private:
	std::string _instructionName[4];
	float _effectVolume;
	int _index;
	Sprite* _instructionBG;
};

#endif //__INSTRUCTION_SCENE_H__
