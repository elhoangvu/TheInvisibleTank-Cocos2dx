#ifndef __BOOM_H__
#define __BOOM_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"
#include "Bullet.h"

USING_NS_CC;
class Boom : public Bullet
{
public:
	static Boom* createBullet();
	float run(LinearEquationHelper line);
	void remove(bool isCleanup);
	void setRotation(float rotation);
	//void addItem(Layer* layer, Point pos);
	Boom();
private:
	float _effectVolume;
};

#endif // __BOOM_H__
