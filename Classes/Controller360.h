#ifndef __CONTROLLER_360_H__
#define __CONTROLLER_360_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"

USING_NS_CC;
class Controller360 : public cocos2d::Sprite
{
public:
	static Controller360* createController(Layer* layer);
	//void update(float dt);
	void onTouchBegin(Point touchPos);
	void onControl(Point touchPos);
	void onControlEnded();
	Point getTouchPosition();
	void setScale(float scale);
	const Point ControllerPosition;
	Controller360();
private:
	Point _touchPos;
	Layer* _layer;
	Sprite* _direcControl;
	LinearEquationHelper _line;
	Size _visibleSize;
};

#endif // __CONTROLLER_360_H__
