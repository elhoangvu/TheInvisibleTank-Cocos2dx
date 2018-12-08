#include "Bullet.h"
#include "SimpleAudioEngine.h"

#include "Defination.h"

Bullet* Bullet::createBullet()
{
	auto bullet = new Bullet;
	//bullet->setTexture("dan.png");
	//bullet->setAnchorPoint(Point(0.5, 0.5));

	bullet->setTexture("dan.png");
	bullet->setAnchorPoint(Point(1, 0.5));

	auto body = PhysicsBody::createBox(bullet->getBoundingBox().size);
	body->setDynamic(false);
	body->setContactTestBitmask(true);
	body->setCollisionBitmask(BITMASK_BULLET);
	bullet->setPhysicsBody(body);
	return bullet;
}

void Bullet::setDeparture(Point departure)
{
	_departure = departure;
}

void Bullet::setDirectionVector(Point directionVector)
{
	_directionVector = directionVector;

}

void Bullet::setLine(LinearEquationHelper line)
{
	_line = line;
}

float Bullet::run(LinearEquationHelper line)
{
	this->setPosition(_departure);
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

void Bullet::remove(bool isCleanup)
{
	this->removeFromParentAndCleanup(isCleanup);
}

void Bullet::setRotation(float rotation)
{
	Node::setRotation(rotation);
}

void Bullet::addItem(Layer * layer, Point pos)
{
	int type = RandomHelper::random_int(0, 2);
	Sprite* item;
	switch (type)
	{
	case BITMASK_SHOT:
		item = Sprite::create("itemshot.png");
		item->setTag(BITMASK_SHOT);
		break;
	case BITMASK_LAZER:
		item = Sprite::create("itemlazer.png");
		item->setTag(BITMASK_LAZER);
		break;
	case BITMASK_BOOM:
		item = Sprite::create("itemboom.png");
		item->setTag(BITMASK_BOOM);
		break;
	}
	item->setScale(0.25f);
	
	item->setPosition(pos);
	item->runAction(Sequence::create(CallFunc::create([item]() {item->setVisible(false); }), Place::create(pos), CallFunc::create([item]() {item->setVisible(true); }), nullptr));
	auto body = PhysicsBody::createBox(item->getBoundingBox().size);
	body->setDynamic(false);
	
	body->setCollisionBitmask(BITMASK_ITEM);
	body->setContactTestBitmask(true);

	item->setPhysicsBody(body);
	layer->addChild(item);
}

void Bullet::setLayer(Layer * layer)
{
	_layer = layer;
}

float Bullet::run()
{
	return run(_line);
}

Bullet::Bullet()// : Sprite()
{
	_directionVector = Point(0, 1);
	_visibleSize = Director::getInstance()->getVisibleSize();
	_layer = NULL;
	_flippedX = false;
	_flippedY = false;
}
