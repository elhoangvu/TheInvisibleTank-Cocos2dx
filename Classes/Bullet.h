#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"

USING_NS_CC;
class Bullet : public cocos2d::Sprite
{
public:
	static Bullet* createBullet();
	virtual void setDeparture(Point departure);
	virtual void setDirectionVector(Point directionVector);
	virtual void setLine(LinearEquationHelper line);
	virtual float run(LinearEquationHelper line);
	virtual void remove(bool isCleanup);
	virtual void setRotation(float rotation);
	void addItem(Layer* layer, Point pos);
	void setLayer(Layer* layer);
	float run();
	Bullet();
protected:
	Point _departure;
	Point _directionVector;
	LinearEquationHelper _line;
	Size _visibleSize;
	Layer* _layer;
};

#endif // __BULLET_H__
