#ifndef __NORMAL_BULLET_H__
#define __NORMAL_BULLET_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"
#include "Bullet.h"

USING_NS_CC;
class NormalBullet: public Bullet
{
public:
	static NormalBullet* createBullet();
	float run(LinearEquationHelper line);
//	void addItem(Layer* layer, Point pos);
	NormalBullet();
private:
	float _effectVolume;
};

#endif // __NORMAL_BULLET_H__
