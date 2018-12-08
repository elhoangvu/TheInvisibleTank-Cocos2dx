#include "BotTank.h"
#include "SimpleAudioEngine.h"

#include "Defination.h"

BotTank* BotTank::createTank(Layer* layer)
{
	auto tank = new BotTank;
	tank->_layer = layer;
	layer->addChild(tank);

	tank->_additionalBG = Sprite::create("additionalbg.png");
	tank->_additionalBG->setAnchorPoint(Point::ZERO);
	tank->_additionalBG->setPosition(Point::ZERO);
	tank->setVisible(false);
	tank->_additionalBG->setScale(WORLD_SCALE);
	layer->addChild(tank->_additionalBG, Z_ORDER_BACKGROUND * 10);

	return tank;
}
void BotTank::addBot(const Rect& prohibitedRect, int n)
{
	this->runAction(Sequence::create(CallFunc::create([this]() {_additionalBG->setVisible(true); }), DelayTime::create(0.1), CallFunc::create([this]() {_additionalBG->setVisible(false); }), nullptr));
	for (int i = 0; i < n; i++)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, prohibitedRect]()
		{
			Point p = randomPosition(prohibitedRect);
			auto bot = Sprite::create();
			bot->initWithFile("bottank.png");
			bot->runAction(FadeOut::create(0.3));
			auto tankWidth = bot->getBoundingBox().size.width;
			auto tankHeight = bot->getBoundingBox().size.height;
			Point deltaPos[] = {	{ tankWidth, tankHeight },
									{ 0, 0 },
									{ 0, tankHeight },
									{ tankWidth, 0 },
									{ tankWidth / 2, 0 },
									{ 0, tankHeight / 2 },
									{ tankWidth / 2, tankHeight },
									{ tankWidth, tankHeight / 2 } };

			for (int j = 0; j < 8; j++)
			{
				auto node = Node::create();
				Point pos = bot->getPosition() + deltaPos[j];
				node->setPosition(pos);
				bot->addChild(node);
			}


			auto bodyBot = PhysicsBody::createBox(bot->getBoundingBox().size, PhysicsMaterial(1, 0, 0));
			bodyBot->setDynamic(false);
			bodyBot->setContactTestBitmask(true);
			bodyBot->setCollisionBitmask(BITMASK_BOTTANK);
			bot->setPhysicsBody(bodyBot);

			bot->setPosition(p);
			bot->setScale(_scale);

			//bot->setVisible(false);

			this->_bot.pushBack(bot);
			preRun(bot);

			auto shadow = Shadow::createShadow("tankshadow0.png");
			shadow->setDelta(_visibleSize.width / 120);
			shadow->setLightSource(Point::ZERO);
			shadow->setScale(0.6f);
			shadow->setParent(bot);
			_layer->addChild(shadow, Z_ORDER_BOT / 10);
			shadow->activate();
			shadow->runAction(FadeOut::create(0.3));
			_shadow.pushBack(shadow);
			_layer->addChild(bot, Z_ORDER_BOT);
		});
	}
}

void BotTank::run()
{
	for (int i = 0; i < _bot.size(); i++)
	{
		auto action = CallFunc::create(CC_CALLBACK_0(BotTank::preRun, this, _bot.at(i)));
		_bot.at(i)->stopAllActions();
		_bot.at(i)->runAction(action);
	}
}

void BotTank::setScale(float scale)
{
	_scale = scale;
	for (int i = 0; i < _bot.size(); i++)
	{
		_bot.at(i)->setScale(scale);
	}
}


Shadow* BotTank::findShadow(Sprite * bot)
{
	return _shadow.at(_bot.getIndex(bot));
}

void BotTank::showAll()
{
	for (int i = 0; i < _bot.size(); i++)
	{
		_bot.at(i)->runAction(FadeIn::create(0.5));
		_shadow.at(i)->runAction(FadeIn::create(0.5));
	}
}

Vector<Sprite*> BotTank::getBot()
{
	return _bot;
}

Sprite* BotTank::getBot(int i)
{
	if (i >= _bot.size())
	{
		return NULL;
	}
	return _bot.at(i);
}

int BotTank::getBotCount()
{
	return _bot.size();
}

void BotTank::removeBotTank(Sprite* bot)
{
	auto particle = ParticleSystemQuad::create("fire.plist");
	particle->setPosition(Point(bot->getPositionX(), bot->getPositionY()));
	particle->setScale(0.4f);
	_layer->addChild(particle, Z_ORDER_EXLODING);

	auto shadow = findShadow(bot);
	_bot.eraseObject(bot);
	_shadow.eraseObject(shadow);
	bot->stopAllActions();
	bot->getPhysicsBody()->setEnabled(false);
	bot->runAction(Sequence::create(DelayTime::create(0.25), RemoveSelf::create(), nullptr));
	shadow->runAction(Sequence::create(DelayTime::create(0.25), RemoveSelf::create(), nullptr));
	//bot->setVisible(true);
	
}

BotTank::BotTank()
{
	this->_visibleSize = Director::getInstance()->getVisibleSize();
}

BotTank::~BotTank()
{
}

void BotTank::preRun(Sprite* bot)
{
	int x = RandomHelper::random_int(0, (int)_visibleSize.width);
	int y = RandomHelper::random_int(0, (int)_visibleSize.height);
	
	Point directionVector = Point(x - bot->getPositionX(), y - bot->getPositionY());
	float alpha = acos(directionVector.y / sqrt(directionVector.x*directionVector.x + directionVector.y*directionVector.y));

	alpha = alpha * 180 / M_PI;
	if (directionVector.x < 0)
	{
		alpha = -alpha;
	}

	bot->setRotation(alpha);

	float d = sqrt((x - bot->getPosition().x)*(x - bot->getPosition().x) + (y - bot->getPosition().y)*(y - bot->getPosition().y));
	
	auto action = Sequence::create(MoveTo::create(d / 25, Point(x, y)), CallFunc::create(CC_CALLBACK_0(BotTank::preRun, this, bot)), nullptr);
	
	bot->runAction(action);
}

Point BotTank::randomPosition(const Rect & prohibitedRect)
{
	Point p[4];
	int n = 0;
	if (prohibitedRect.origin.x > 0)
	{
		p[n].x = RandomHelper::random_real((float)0, prohibitedRect.origin.x);
		p[n].y = RandomHelper::random_real((float)0, _visibleSize.height);
		n++;
	}
	if (prohibitedRect.origin.x + prohibitedRect.size.width < _visibleSize.width)
	{
		p[n].x = RandomHelper::random_real(prohibitedRect.origin.x + prohibitedRect.size.width, _visibleSize.width);
		p[n].y = RandomHelper::random_real((float)0, _visibleSize.height);
		n++;
	}
	if (prohibitedRect.origin.y > 0)
	{
		p[n].x = RandomHelper::random_real((float)0, _visibleSize.width);
		p[n].y = RandomHelper::random_real((float)0, prohibitedRect.origin.y);
		n++;
	}
	if (prohibitedRect.origin.y + prohibitedRect.size.height < _visibleSize.height)
	{
		p[n].x = RandomHelper::random_real((float)0, _visibleSize.width);
		p[n].y = RandomHelper::random_real(prohibitedRect.origin.y + prohibitedRect.size.height, _visibleSize.height);
		n++;
	}
	int i = RandomHelper::random_int(0, n - 1);
	return p[i];
}
