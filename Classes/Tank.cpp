#include "Tank.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "Defination.h"

Tank* Tank::createTank(Layer* layer, std::string filename)
{
	auto tank = new Tank;
	//tank->setAnchorPoint(Point(1, 0.5));
	tank->_layer = layer;

	tank->initWithFile(filename);
	
//	this->initWithFile("tank.png");
	tank->setPosition(tank->_visibleSize / 2);
	tank->_directionVector = Point(0, 1);
	tank->_line = LinearEquationHelper::createWithDirectionVector(Point(0, 1), tank->getPosition());

	auto bodyTank = PhysicsBody::createBox(tank->getContentSize());
	bodyTank->setDynamic(false);
	bodyTank->setContactTestBitmask(true);
	bodyTank->setCollisionBitmask(BITMASK_TANK);
	tank->setPhysicsBody(bodyTank);

	tank->_dynamicRada = Sprite::create("rada1.png");
	tank->_staticRada = Sprite::create("rada1.png");

	auto bodyRada = PhysicsBody::createCircle(tank->_dynamicRada->getContentSize().width / 2);
	bodyRada->setDynamic(false);
	bodyRada->setContactTestBitmask(true);
	bodyRada->setCollisionBitmask(BITMASK_RADA);
	tank->_dynamicRada->setPhysicsBody(bodyRada);

	tank->_dynamicRada->setPosition(Point(tank->getContentSize().width * 0.5f, tank->getContentSize().height * 0.42f));
	tank->_staticRada->setPosition(Point(tank->getContentSize().width * 0.5f, tank->getContentSize().height * 0.42f));

	//tank->_dynamicRada->setScale(1);
	//tank->_staticRada->setScale(1);
	tank->addChild(tank->_dynamicRada, -1);
	tank->addChild(tank->_staticRada, -1);
	tank->schedule(schedule_selector(Tank::_moveForward), 1.0 / 60.0);
	tank->scheduleUpdate();
	return tank;
}

Sprite * Tank::getStaticRada()
{
	return _staticRada;
}

void Tank::update(float dt)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
	{
		float dMin = INT_MAX;
		int n = _botTank->getBotCount();
		Point b = this->getPosition();
		auto parent = _botTank->getBot();
		for (auto tank: parent)
		{

			auto child = tank->getChildren();
	
			for (auto node : child)
			{
				auto p = node->getPosition();
				Point a = tank->convertToWorldSpace(p);
				float dTmp = LinearEquationHelper::distance(a, b);
				if (dMin > dTmp)
				{
					dMin = dTmp;
				}
			}

		}
		float d = _staticRada->getBoundingBox().size.height * this->getScale() / 2;
		if (dMin < d)
		{
			float scale = dMin / d;
			_dynamicRada->setScale(scale);
		}
		else
		{
			_dynamicRada->setScale(1);
		}
	});
}

void Tank::setBotTank(BotTank * botTank)
{
	_botTank = botTank;
}

void Tank::move(Point directionVector)
{
	if (!_isMoving)
	{
		_movingSoundID = cocos2d::experimental::AudioEngine::play2d("tankmoving.mp3", true);
		cocos2d::experimental::AudioEngine::setVolume(_movingSoundID, _effectVolume);
	}
	_isMoving = true;
	_directionVector = directionVector;
	long double sq = sqrt((long double)directionVector.x* (long double)directionVector.x + (long double)directionVector.y*(long double)directionVector.y);
	if (sq == 0)
	{
		return;
	}
	_alpha = acos(directionVector.y / sq);

	_alpha = _alpha * 180 / M_PI;
	if (directionVector.x < 0)
	{
		_alpha = -_alpha;
	}
	this->setRotation(_alpha);
	

	_line = LinearEquationHelper::createWithDirectionVector(directionVector, this->getPosition());

}

float Tank::shoot()
{
	Bullet* bullet = BulletFactory::create(_bulletType);
	bullet->setLayer(this->_layer);
	bullet->setRotation(this->getRotation());
	bullet->setDirectionVector(_directionVector);
	bullet->setLine(_line);
	Point p0;
	if (_bulletType == BITMASK_BOOM)
	{
		p0 = this->getPosition();
	}
	else
	{
		Point p1, p2;
		bool result = _line.createPointWithDistance(this->getPosition(), p1, p2, this->getBoundingBox().size.height*0.52f);
		if (!result)
			return -1;
		Point p;
		p = Point(p1.x - this->getPosition().x, p1.y - this->getPosition().y);

		if (p.x * _directionVector.x >= 0 && p.y * _directionVector.y >= 0)
		{
			p0 = p1;
		}
		else
		{
			p0 = p2;
		}
	}
	bullet->setDeparture(p0);
	_layer->addChild(bullet, Z_ORDER_BULLET);
	
	return bullet->run();
}


void Tank::stop()
{
	if (_movingSoundID != -1 && _isMoving)
	{
		cocos2d::experimental::AudioEngine::stop(_movingSoundID);
	}
	_isMoving = false;
}

Rect Tank::getBoundingAllBox()
{
	Rect rect;
	auto scale = this->getScale();
	rect.size = this->getStaticRada()->getBoundingBox().size*scale*1.1;
	rect.origin = Point(this->getPosition().x - this->getStaticRada()->getBoundingBox().size.width *scale / 2, this->getPosition().y - this->getStaticRada()->getBoundingBox().size.height * scale / 2) - this->getStaticRada()->getBoundingBox().size*0.05;
	return rect;
}

void Tank::setBulletType(int bulletType)
{
	_bulletType = bulletType;
}

float Tank::getShootingTime()
{
	return 0;
}

void Tank::setShadow(Shadow * shadow)
{
	_shadow = shadow;
	_shadow->setParent(this);
	_layer->addChild(_shadow, Z_ORDER_TANK - 1);
	_shadow->activate();
}

void Tank::setVisible(bool isVisible)
{
	Sprite::setVisible(isVisible);
	if (_shadow != NULL)
	{
		_shadow->setVisible(isVisible);
	}
	_dynamicRada->setVisible(false);
	_staticRada->setVisible(false);
}

//void Tank::setAnchorPoint(Point pos)
//{
//	Sprite::setAnchorPoint(pos);
//}

Tank::Tank()//: Sprite()
{
	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
	_flippedX = false;
	_flippedY = false;
	_movingSoundID = -1;
	_visibleSize = Director::getInstance()->getVisibleSize();
	_isMoving = false;
	_deltaMove = sqrt(_visibleSize.width*_visibleSize.width + _visibleSize.height*_visibleSize.height) / 750;
	_bulletType = BITMASK_SHOT;
	_shadow = NULL;
}

void Tank::_moveForward(float dt)
{
	if (_isMoving)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
		{
			Point p1, p2;
			if (!_line.createPointWithDistance(this->getPosition(), p1, p2, _deltaMove))
				return;
			Point p;
			p = Point(p1.x - this->getPosition().x, p1.y - this->getPosition().y);
			Point posDes;
			if (p.x * _directionVector.x >= 0 && p.y * _directionVector.y >= 0)
			{
				posDes = p1;
			}
			else
			{
				posDes = p2;
			}
			if (posDes.x < 0 || posDes.x > _visibleSize.width || posDes.y < 0 || posDes.y > _visibleSize.height)
				return;
			this->setPosition(posDes);
		});
	}
}