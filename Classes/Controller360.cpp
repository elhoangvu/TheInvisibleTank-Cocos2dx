#include "Controller360.h"
#include "SimpleAudioEngine.h"

#include "Defination.h"

Controller360* Controller360::createController(Layer* layer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto controller = new Controller360;
	controller->_layer = layer;
	controller->setTexture("control.png");
	controller->setAnchorPoint(Point(0.5, 0.5));

	controller->setPosition(controller->ControllerPosition);
	
	controller->_direcControl = Sprite::create("control2.png");

	controller->_direcControl->setAnchorPoint(Point(0.5, 0.5));
	controller->_direcControl->setPosition(controller->getPosition());
	
	layer->addChild(controller, Z_ORDER_TOP);
	layer->addChild(controller->_direcControl, Z_ORDER_TOP);
	return controller;
}

void Controller360::onTouchBegin(Point touchPos)
{
	this->setPosition(touchPos);
	this->_direcControl->setPosition(touchPos);
}

void Controller360::onControl(Point touchPos)
{
	_touchPos = touchPos;
	_line = LinearEquationHelper::createWithTwoPoint(this->getPosition(), touchPos);
	float d = this->getBoundingBox().size.width / 2 - this->_direcControl->getBoundingBox().size.width / 2;
	Point p1, p2;
	_line.createPointWithDistance(this->getPosition(), p1, p2, d);
	if (LinearEquationHelper::distance(p1, touchPos) < LinearEquationHelper::distance(p2, touchPos))
	{
		this->_direcControl->setPosition(p1);
		return;
	}
	this->_direcControl->setPosition(p2);
}

void Controller360::onControlEnded()
{
	this->setPosition(ControllerPosition);
	this->_direcControl->setPosition(ControllerPosition);

}

Point Controller360::getTouchPosition()
{
	return _touchPos;
}

void Controller360::setScale(float scale)
{
	Sprite::setScale(scale);
	this->_direcControl->setScale(scale);
}

Controller360::Controller360() :ControllerPosition(Point(Director::getInstance()->getVisibleSize().width / 7, Director::getInstance()->getVisibleSize().height / 4))
{
	_visibleSize = Director::getInstance()->getVisibleSize();
	_flippedX = false;
	_flippedY = false;
}


