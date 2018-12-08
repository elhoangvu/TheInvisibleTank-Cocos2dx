#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"
USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void setTank(int index);
	CREATE_FUNC(MenuScene);
private:
	Size _visibleSize;
	int _audioBackgroundID;
	float _effectVolume;
	Sprite* _tank;
	Sprite* _shadow;
};

#endif //__MENU_SCENE_H__
