#include "Shadow.h"
#include "SimpleAudioEngine.h"

Shadow* Shadow::createShadow(const std::string& filename)
{
	auto shadow = new Shadow();
	
	shadow->setTexture(filename);
	shadow->setAnchorPoint(Point(0.5, 0.5));
	//CCLOG("rt %f %f", shadow->getRotation(), shadow->getRotationQuat());
	//CCLOG("fl %i %i", (bool)shadow->_flippedX, (bool)shadow->_flippedY);
	return shadow;
}

void Shadow::setParent(Sprite * parent)
{
	_parent = parent;
}

void Shadow::setDelta(float dt)
{
	_delta = dt;
}

void Shadow::setLightSource(Point pos)
{
	_lightSource = pos;
}

void Shadow::activate()
{
	this->scheduleUpdate();
}

void Shadow::update(float dt)
{
	Point parentPos = _parent->getPosition();
	if (_delta == 0 || _lightSource == parentPos)
	{
		return;
	}

	auto line = LinearEquationHelper::createWithTwoPoint(_lightSource, parentPos);
	if (line.isNotALine())
	{
		return;
	}
	Point p1, p2;
	line.createPointWithDistance(parentPos, p1, p2, _delta);

	Point directionVector = parentPos - _lightSource;
	Point p = p1 - parentPos;
	Point posDes;
	if (p.x * directionVector.x > 0 && p.y * directionVector.y > 0)
	{
		posDes = p1;
	}
	else
	{
		posDes = p2;
	}

	this->setPosition(posDes);
	this->setRotation(_parent->getRotation());
}

Shadow::Shadow()//: Sprite()
{
	_delta = 0;
	_flippedX = false;
	_flippedY = false;
}
