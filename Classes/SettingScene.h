#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"
#include "ui\UIButton.h"
#include "ui\UIPageView.h"
#include "ui\UISlider.h"
USING_NS_CC;

class SettingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(int audioID, float* effectVolume);
    virtual bool init();
	void popScene();
	CREATE_FUNC(SettingScene);

private:
	void launch();
	void createBMSlider();
	void createESSlider();
	void setVolumeBM(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void setVolumeES(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	int _audioID;
	Size _visibleSize;
	Label* _labelBgrPer;
	Label* _labelEffPer;
	float* _effectVolume;
};

#endif 
