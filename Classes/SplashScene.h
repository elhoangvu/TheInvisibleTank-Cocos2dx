#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"
USING_NS_CC;

class SplashScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void preLoadImage();
	void preLoadSound();
	CREATE_FUNC(SplashScene);
};

#endif //__SPLASH_SCENE_H__
