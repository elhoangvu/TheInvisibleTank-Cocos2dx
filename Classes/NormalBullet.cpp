#include "NormalBullet.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "Defination.h"

NormalBullet* NormalBullet::createBullet()
{
	auto shot = new NormalBullet;
	//Shot->setTexture("dan.png");
	//Shot->setAnchorPoint(Point(0.5, 0.5));

	shot->initWithFile("dan.png");
	shot->setAnchorPoint(Point(1, 0.5));
	
	auto particle = ParticleSystemQuad::create("shot.plist");
	particle->setPosition(shot->getBoundingBox().size / 2);
	particle->setScale(0.2);
	shot->addChild(particle, Z_ORDER_BOT*10);
	//shot->setVisible(false);
	auto body = PhysicsBody::createBox(shot->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(true);
	body->setCollisionBitmask(BITMASK_BULLET);
	shot->setPhysicsBody(body);

	return shot;
}


float NormalBullet::run(LinearEquationHelper line)
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("normalbullet.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	this->setPosition(_departure);
	this->runAction(Place::create(_departure));
	_line = line;
	Point p1, p2;
	if (!line.createPointWithDistance(_departure, p1, p2, _visibleSize.width))
		return -1;
	Point p = Point(p1.x - _departure.x, p1.y - _departure.y);
	if (p.x * _directionVector.x >= 0 && p.y * _directionVector.y >= 0)
	{
		this->runAction(Sequence::create(MoveTo::create(2, p1), RemoveSelf::create(), nullptr));
		return 2;
	}
	this->runAction(Sequence::create(MoveTo::create(2, p2), RemoveSelf::create(), nullptr));
	return 2;
}
//
//void Shot::addItem(Layer * layer, Point pos)
//{
//	auto item = Sprite::create("dan.png");
//	item->setPosition(pos);
//	item->runAction(Place::create(pos));
//	auto body = PhysicsBody::createBox(item->getBoundingBox().size);
//	body->setDynamic(false);
//	body->setCollisionBitmask(SHOT);
//	body->setContactTestBitmask(true);
//
//	item->setPhysicsBody(body);
//	item->setTag(SHOT);
//	layer->addChild(item);
//}

NormalBullet::NormalBullet(): Bullet()
{
	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
}
