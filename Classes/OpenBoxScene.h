#ifndef __OPEN_BOX_SCENE_H__
#define __OPEN_BOX_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"
#include "ui\UIButton.h"
#include "ui\UIPageView.h"

USING_NS_CC;

class OpenBoxScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(cocos2d::ui::Button** button, cocos2d::ui::PageView* pageView, std::string* charactor);
    virtual bool init();
	void createBoxes();
	void onChoose(int i);
	void popScene();
	void removeBoxes();
	CREATE_FUNC(OpenBoxScene);

private:
	int _index[3];
	cocos2d::Size _visibleSize;
	cocos2d::UserDefault* _def;
	cocos2d::ui::Button** _button;
	cocos2d::ui::PageView* _pageView;
	std::string* _charactor;
	cocos2d::MenuItemImage* _box[3];
	cocos2d::Sprite* _char[3];
	bool _isChosen;
	float _effectVolume;
};

#endif //__OPEN_BOX_SCENE_H__
