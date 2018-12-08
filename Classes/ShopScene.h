#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"
#include "Defination.h"
#include "ui\UIListView.h"
#include "ui\UIPageView.h"
#include "ui\UIButton.h"
#include "OpenBoxScene.h"
USING_NS_CC;

class ShopScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(const std::function<void (int)> &func);
    virtual bool init();
	void update(float dt);
	void goToCardScene();
	CREATE_FUNC(ShopScene);
private:
	void setCharactor();
	void popScene();
	std::function<void(int)> setTankFunc;
private:
	cocos2d::Label* _labelUnlock;
	ui::Button* _buttonChar[12];
	cocos2d::UserDefault* _def;
	int _index;
	int _curCharactor;
	int _coins;
	cocos2d::Label* _labelCoins;
	std::string _charactor;
	cocos2d::ui::PageView* _pageView;
	cocos2d::MenuItemImage* _btnChoice;
	cocos2d::MenuItemImage* _btnOpenBox;
	Vec2 _origin;
	Size _visibleSize;
	float _effectVolume;
};

#endif 
