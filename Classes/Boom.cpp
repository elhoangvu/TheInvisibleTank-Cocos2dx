#include "Boom.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "Defination.h"

Boom* Boom::createBullet()
{
	auto boom = new Boom;
	//Boom->setTexture("dan.png");
	//Boom->setAnchorPoint(Point(0.5, 0.5));

	boom->initWithFile("boom.png");
	boom->setScale(0.8f);
	return boom;
}

float Boom::run(LinearEquationHelper line)
{
	this->setAnchorPoint(Point(0.5, 0.5));
	this->setPosition(_departure);
	auto buffer = __String::createWithFormat("%d", 2);
	auto labelScore = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 18);
	labelScore->setColor(Color3B(253, 183, 20));
	labelScore->setPosition(Point(this->getBoundingBox().size.width*0.6, this->getBoundingBox().size.height*0.7));
	this->addChild(labelScore);
	
	this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([labelScore]()
	{
		auto buffer = __String::createWithFormat("%d", 1);
		labelScore->setString(buffer->getCString());
	}), nullptr));

	this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([labelScore]()
	{
		auto buffer = __String::createWithFormat("%d", 0);
		labelScore->setString(buffer->getCString());
	}), nullptr));

	this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([this, labelScore]()
	{

		auto soundID = cocos2d::experimental::AudioEngine::play2d("boom.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		this->setTexture("explodingSpace.png");
		auto body = PhysicsBody::createCircle(this->getBoundingBox().size.width / 2);
		body->setDynamic(false);
		body->setContactTestBitmask(true);
		body->setCollisionBitmask(BITMASK_BULLET);
		this->setPhysicsBody(body);
		this->setScale(0.05);
		this->setVisible(false);
		this->runAction(Sequence::create(ScaleTo::create(1.2, 1.1), RemoveSelf::create(), nullptr));

		auto particle = ParticleSystemQuad::create("explosion.plist");
		particle->setPosition(this->getPosition());
		particle->setScale(0.35f);
		particle->setAutoRemoveOnFinish(true);
		_layer->addChild(particle, Z_ORDER_EXLODING);

	}), nullptr));
	return 4.2;
}

void Boom::remove(bool isCleanup)
{
}

void Boom::setRotation(float rotation)
{
}

//void Boom::addItem(Layer* layer, Point pos)
//{
//	auto item = Sprite::create("BoomItem.png");
//	item->setPosition(pos);
//	item->runAction(Place::create(pos));
//	auto body = PhysicsBody::createBox(item->getBoundingBox().size);
//	body->setDynamic(false);
//	body->setCollisionBitmask(Boom);
//	body->setContactTestBitmask(true);
//
//	item->setPhysicsBody(body);
//	item->setTag(Bullet::BulletType::Boom);
//	layer->addChild(item);
//}

Boom::Boom(): Bullet()
{
	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
}
