#ifndef __LAZER_H__
#define __LAZER_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"
#include "Bullet.h"

USING_NS_CC;
class Lazer : public Bullet
{
public:
	static Lazer* createBullet();
	float run(LinearEquationHelper line);
	void remove(bool isCleanup);
	void setRotation(float rotation);
	//void addItem(Layer* layer, Point pos);
	Lazer();
private:
	float _effectVolume;
};

#endif // __BULLET_H__
