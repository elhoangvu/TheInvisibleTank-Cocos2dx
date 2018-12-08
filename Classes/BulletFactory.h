#ifndef __BULLET_FACTORY_H__
#define __BULLET_FACTORY_H__

#include "cocos2d.h"
#include "Lazer.h"
#include "NormalBullet.h"
#include "Boom.h"

USING_NS_CC;
class BulletFactory
{
public:
	static Bullet* create(int bulletType);
};

#endif // __BULLET_FACTORY_H__
