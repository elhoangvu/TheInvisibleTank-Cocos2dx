#include "SimpleAudioEngine.h"

#include "Defination.h"
#include "BulletFactory.h"

Bullet * BulletFactory::create(int bulletType)
{
	switch (bulletType)
	{
	case BITMASK_SHOT:
		return NormalBullet::createBullet();
	case BITMASK_LAZER:
		return Lazer::createBullet();
	case BITMASK_BOOM:
		return Boom::createBullet();
	}
}
