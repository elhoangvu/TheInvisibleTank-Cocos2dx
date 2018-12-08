#include "Lazer.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "Defination.h"

Lazer* Lazer::createBullet()
{
	auto lazer = new Lazer;
	//Lazer->setTexture("dan.png");
	//Lazer->setAnchorPoint(Point(0.5, 0.5));

	lazer->initWithFile("lazer.png");
	lazer->setAnchorPoint(Point(0.5, 0));

	auto body = PhysicsBody::createBox(lazer->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(true);
	body->setCollisionBitmask(BITMASK_BULLET);
	lazer->setPhysicsBody(body);

	return lazer;
}

float Lazer::run(LinearEquationHelper line)
{
	this->setPosition(_departure);
	_line = line;
	auto soundID = cocos2d::experimental::AudioEngine::play2d("lazer.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	this->runAction(Sequence::create(ScaleTo::create(0.5, 0, 1), RemoveSelf::create(), nullptr));
	return 0.5;
}

void Lazer::remove(bool isCleanup)
{
	this->getPhysicsBody()->setEnabled(false);
	//this->runAction(Sequence::create(DelayTime::create(0.5), RemoveSelf::create(), nullptr));
}

void Lazer::setRotation(float rotation)
{
	/*if (rotation < 0)
	{
		rotation += 180;
	}
	if (rotation < 0)
	{
		rotation += 360;
	}*/
	Sprite::setRotation(rotation);
}

//void Lazer::addItem(Layer* layer, Point pos)
//{
//	auto item = Sprite::create("lazerItem.png");
//	item->setPosition(pos);
//	item->runAction(Place::create(pos));
//	auto body = PhysicsBody::createBox(item->getBoundingBox().size);
//	body->setDynamic(false);
//	body->setCollisionBitmask(LAZER);
//	body->setContactTestBitmask(true);
//
//	item->setPhysicsBody(body);
//	item->setTag(Bullet::BulletType::LAZER);
//	layer->addChild(item);
//}

Lazer::Lazer(): Bullet()
{
	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
}
