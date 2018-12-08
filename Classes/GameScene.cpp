#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <math.h>
#include "Defination.h"
#include "AudioEngine.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	//layer->_sceneWorld->step(1 / 30);
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//auto buffer2 = __String::createWithFormat("x: %f y: %f", origin.x, origin.y);
	//_labelBullet = Label::createWithTTF(buffer2->getCString(), "fonts/FredokaOne-Regular.ttf", 45);
	//_labelBullet->setColor(Color3B(240, 86, 90));
	//_labelBullet->setAnchorPoint(Point::ZERO);
	//_labelBullet->setPosition(_visibleSize / 8);
	//this->addChild(_labelBullet, 100000);
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	auto background = Sprite::create("background.png");
	background->setScale(WORLD_SCALE);
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background, Z_ORDER_BACKGROUND);

	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
	_score = 0;
	_coins = def->getIntegerForKey("INSIDTANK_COINS", 0);
	def->setIntegerForKey("INSIDTANK_COINS", 2000);
	def->flush();
	_plusedCoins = 0;
	auto charactor = def->getStringForKey("INSIDTANK_TANKSKIND", "100000000000.0");
	auto pos = charactor.find_last_of(".");
	std::string curChar = charactor.substr(pos + 1, charactor.size());

	_tank = Tank::createTank(this, "tank" + curChar + ".png");

	_botTank = BotTank::createTank(this);
	_botTank->setScale(0.6f);
	_tank->setScale(0.6f);
	_botTank->addBot(_tank->getBoundingAllBox(), 4);


	_tank->setBotTank(_botTank);

	this->addChild(_tank, Z_ORDER_TANK);
	auto tankShadow = Shadow::createShadow("tankshadow" + curChar + ".png");
	tankShadow->setDelta(_visibleSize.width / 150);
	tankShadow->setLightSource(Point::ZERO);
	tankShadow->setScale(0.6f);
	_tank->setShadow(tankShadow);

	_itemDisplayCount = 0;
	_controller = Controller360::createController(this);
	_controller->setScale(0.5);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	_shoot = MenuItemImage::create("btnshoot.png", "btnshoot.png", CC_CALLBACK_1(GameScene::shoot, this));
	_shoot->setPosition(Point(_visibleSize.width * 0.898f, _visibleSize.height * 0.166f));
	_shoot->setScale(0.6f);

	_btnBullet = Sprite::create("itemshot.png");
	_btnBullet->setPosition(Point(_visibleSize.width * 0.81f, _visibleSize.height * 0.3f));
	_btnBullet->setScale(0.65f);
	this->addChild(_btnBullet, Z_ORDER_TOP);

	auto _btnBulletCount = Sprite::create("bulletcount.png");
	_btnBulletCount->setPosition(Point(_visibleSize.width * 0.901f, _visibleSize.height * 0.389f));
	_btnBulletCount->setScale(0.65f);
	this->addChild(_btnBulletCount, Z_ORDER_TOP);

	auto _coin = Sprite::create("coin.png");
	_coin->setPosition(Point(_visibleSize.width * 0.83f, _visibleSize.height * 0.917f));
	_coin->setScale(0.65f);
	this->addChild(_coin, Z_ORDER_TOP);

	_scanner = MenuItemImage::create("btnscanner.png", "btnscanner.png",
		[this](Ref * sender)
	{
		if (isScanable)
		{
			auto audio = cocos2d::experimental::AudioEngine::play2d("scanner.mp3", false);
			cocos2d::experimental::AudioEngine::setVolume(audio, _effectVolume);
			auto scanner = Sprite::create("scanner.png");
			//scanner->setVisible(false);
			scanner->setPosition(Point(0, _visibleSize.height / 2));
			auto particle = ParticleSystemQuad::create("lazescanner.plist");
			particle->setPosition(Point(scanner->getPositionX(), scanner->getPositionY()));
			particle->setScale(0.8f);
			this->addChild(particle, Z_ORDER_SCANNER);

			auto body = PhysicsBody::createBox(scanner->getBoundingBox().size, PhysicsMaterial(0, 0, 0));
			body->setDynamic(false);
			body->setContactTestBitmask(true);
			body->setCollisionBitmask(BITMASK_SCANNER);

			scanner->setPhysicsBody(body);
			this->addChild(scanner, Z_ORDER_SCANNER);
			auto action = Sequence::create(MoveTo::create(2.5, Point(_visibleSize.width, _visibleSize.height / 2)), CallFunc::create(
				[audio]()
			{
				cocos2d::experimental::AudioEngine::stop(audio);
			}), RemoveSelf::create(), nullptr);
			scanner->runAction(action);
			particle->runAction(action->clone());
			auto bg = Sprite::create("countdown.png");
			auto countdownTimer = ProgressTimer::create(bg);
			countdownTimer->setMidpoint(Vec2(0.5, 0.5));
			countdownTimer->setType(ProgressTimer::Type::RADIAL);
			countdownTimer->setBarChangeRate(Vec2(0, 1));
			countdownTimer->setPosition(_scanner->getPosition());
			countdownTimer->setPercentage(100);
			countdownTimer->runAction(Sequence::create(ProgressTo::create(TIME_RADA_COUNTDOWN, 0), RemoveSelf::create(), nullptr));
			countdownTimer->setReverseProgress(true);
			this->addChild(countdownTimer, Z_ORDER_TOP * 10);
			isScanable = false;
			this->runAction(Sequence::create(DelayTime::create(TIME_RADA_COUNTDOWN), CallFunc::create([this]()
			{
				isScanable = true;
			}), nullptr));

		}
	});
	_scanner->setPosition(Point(_visibleSize.width * 0.784f, _visibleSize.height * 0.106f));
	_scanner->setScale(0.6f);

	auto _pause = MenuItemImage::create("btnpause.png", "btnpause.png",
		[this](Ref * sender)
	{
		if (_isGameOver == true)
		{
			return;
		}
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		onPauseGame();
		this->pauseNodeAndDescendants(this);
	});
	_pause->setScale(0.6f);
	_pause->setPosition(Point(_visibleSize.width * 0.05f, _visibleSize.height * 0.911f));
	auto menu = Menu::create(_shoot, _scanner, _pause, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, Z_ORDER_TOP);



	_bulletCount = TANK_BULLET_COUNT;
	auto buffer = __String::createWithFormat("%d", _bulletCount);
	_labelBullet = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 45);
	_labelBullet->setColor(Color3B(240, 86, 90));
	_labelBullet->setPosition(_btnBulletCount->getPosition());
	this->addChild(_labelBullet, Z_ORDER_TOP);

	buffer = __String::createWithFormat("%d", _score);
	_labelScore = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 120);
	_labelScore->setOpacity(128);
	_labelScore->setPosition(Point(_visibleSize.width / 2, _visibleSize.height*0.9f));
	this->addChild(_labelScore, Z_ORDER_TOP);

	buffer = __String::createWithFormat("%d", _coins);
	_labelCoins = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 50);
	_labelCoins->setOpacity(128);
	_labelCoins->setAnchorPoint(Point(0, 0.5));
	_labelCoins->setPosition(Point(_visibleSize.width * 0.83f + _coin->getBoundingBox().size.width * 0.75, _visibleSize.height * 0.917f));
	this->addChild(_labelCoins, Z_ORDER_TOP);

	auto wall = Node::create();
	auto bodyWall = PhysicsBody::createEdgeBox(_visibleSize,
		PhysicsMaterial(1, 0, 1));
	bodyWall->setDynamic(false);
	wall->setPosition(_visibleSize / 2);
	wall->setPhysicsBody(bodyWall);
	this->addChild(wall);
	

	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//buffer = __String::createWithFormat("x: %f y: %f", _tank->getPositionX(), _tank->getPositionY());
	//_labelPos = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 50);
	//_labelPos->setAnchorPoint(Point(0, 0.5));
	//_labelPos->setPosition(Point(_visibleSize.width * 0.1, _visibleSize.height * 0.7));
	//this->addChild(_labelPos, 100000);
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	return true;
}

//
//void HelloWorld::moveBot(int i)
//{
//	int x = RandomHelper::random_int(0, (int)visibleSize.width);
//	int y = RandomHelper::random_int(0, (int)visibleSize.height);
//
//	float d = sqrt((x - tankBot[i]->getPosition().x)*(x - tankBot[i]->getPosition().x) + (y - tankBot[i]->getPosition().y)*(y - tankBot[i]->getPosition().y));
//
//	CCLOG("%f %f", x, y);
//	auto action = Sequence::create(MoveTo::create(d / 25, Point(x, y)), DelayTime::create(d / 25), CallFunc::create(CC_CALLBACK_0(HelloWorld::moveBot, this, i)), nullptr);
//
//	tankBot[i]->runAction(action);
//}

bool GameScene::onContactBegin(cocos2d::PhysicsContact & contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, a, b]()
	{
		if (a->getCollisionBitmask() == BITMASK_BULLET && b->getCollisionBitmask() == BITMASK_BOTTANK)
		{
			auto soundID = cocos2d::experimental::AudioEngine::play2d("exploding.mp3", false);
			cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
			_itemDisplayCount++;
			_score++;

			auto bullet = dynamic_cast<Bullet*>(a->getNode());
			auto botTank = dynamic_cast<Sprite*>(b->getNode());
			bullet->remove(true);
			_botTank->removeBotTank(botTank);
			botTank->getPhysicsBody()->setEnabled(false);
			int x = RandomHelper::random_int(1, 5);
			if (x == 1)
			{
				x = 2;
			}
			else
			{
				x = 1;
			}
			_botTank->addBot(_tank->getBoundingAllBox(), x);
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, bullet, botTank]()
			{
				bullet->addItem(this, botTank->getPosition());
			});

			int r = RandomHelper::random_int(0, 100);
			if (r < PROBABILITY_COINS)
			{
				int coinPlus = RandomHelper::random_int(1, 2);
				this->_coins += coinPlus;
				this->_plusedCoins += coinPlus;
				Point pos = Point(_visibleSize.width * 0.83f, _visibleSize.height * 0.75f);
				raiseCoins(coinPlus, pos);
			}

			this->scoreUpdate();
		}
		else if (a->getCollisionBitmask() == BITMASK_BOTTANK && b->getCollisionBitmask() == BITMASK_BULLET)
		{
			auto soundID = cocos2d::experimental::AudioEngine::play2d("exploding.mp3", false);
			cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
			_itemDisplayCount++;
			_score++;
			auto botTank = dynamic_cast<Sprite*>(a->getNode());
			auto bullet = dynamic_cast<Bullet*>(b->getNode());
			bullet->remove(true);
			_botTank->removeBotTank(botTank);
			botTank->getPhysicsBody()->setEnabled(false);
			int x = RandomHelper::random_int(1, 5);
			if (x == 1)
			{
				x = 2;
			}
			else
			{
				x = 1;
			}
			_botTank->addBot(_tank->getBoundingAllBox(), x);
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, bullet, botTank]()
			{
				bullet->addItem(this, botTank->getPosition());
			});

			int r = RandomHelper::random_int(0, 100);
			if (r < PROBABILITY_COINS)
			{
				int coinPlus = RandomHelper::random_int(1, 2);
				this->_coins += coinPlus;
				this->_plusedCoins += coinPlus;
				Point pos = Point(_visibleSize.width * 0.83f, _visibleSize.height * 0.75f);
				raiseCoins(coinPlus, pos);
			}

			this->scoreUpdate();
		}
	});

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, a, b]()
	{
		if (a->getCollisionBitmask() == BITMASK_BOTTANK && b->getCollisionBitmask() == BITMASK_SCANNER)
		{
			auto botTank = dynamic_cast<Sprite*>(a->getNode());
			botTank->runAction(Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5), nullptr));
			_botTank->findShadow(botTank)->runAction(Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5), nullptr));
		}
		else if (a->getCollisionBitmask() == BITMASK_SCANNER && b->getCollisionBitmask() == BITMASK_BOTTANK)
		{
			auto botTank = dynamic_cast<Sprite*>(b->getNode());
			botTank->runAction(Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5), nullptr));
			_botTank->findShadow(botTank)->runAction(Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5), nullptr));
		}
	});

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, a, b]()
	{
		if (a->getCollisionBitmask() == BITMASK_TANK && b->getCollisionBitmask() == BITMASK_ITEM)
		{
			_bulletCount++;
			_itemDisplayCount--;
			auto bullet = dynamic_cast<Sprite*>(b->getNode());
			int type = bullet->getTag();
			_tank->setBulletType(type);
			bullet->removeFromParentAndCleanup(true);

			switch (type)
			{
			case BITMASK_SHOT:
				_btnBullet->setTexture("itemshot.png");
				break;
			case BITMASK_LAZER:
				_btnBullet->setTexture("itemlazer.png");
				break;
			case BITMASK_BOOM:
				_btnBullet->setTexture("itemboom.png");
				break;
			}
			this->bulletUpdate();

		}
		else if (a->getCollisionBitmask() == BITMASK_ITEM && b->getCollisionBitmask() == BITMASK_TANK)
		{
			_bulletCount++;
			_itemDisplayCount--;
			auto bullet = dynamic_cast<Sprite*>(a->getNode());
			int type = bullet->getTag();
			_tank->setBulletType(type);
			bullet->removeFromParentAndCleanup(true);

			switch (type)
			{
			case BITMASK_SHOT:
				_btnBullet->setTexture("itemshot.png");
				break;
			case BITMASK_LAZER:
				_btnBullet->setTexture("itemlazer.png");
				break;
			case BITMASK_BOOM:
				_btnBullet->setTexture("itemboom.png");
				break;
			}
			this->bulletUpdate();

		}
	});
	/*if (a->getCollisionBitmask() == BITMASK_TANK && b->getCollisionBitmask() == BITMASK_SHOT)
	{
		auto bullet = dynamic_cast<Sprite*>(b->getNode());
		_tank->setBulletType(BITMASK_LAZER);
		bullet->removeFromParentAndCleanup(true);
	}
	else if (a->getCollisionBitmask() == BITMASK_SHOT && b->getCollisionBitmask() == BITMASK_TANK)
	{
		auto bullet = dynamic_cast<Sprite*>(a->getNode());
		_tank->setBulletType(BITMASK_LAZER);
		bullet->removeFromParentAndCleanup(true);
	}*/

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, a, b]()
	{
		if (a->getCollisionBitmask() == BITMASK_TANK && b->getCollisionBitmask() == BITMASK_BOTTANK)
		{
			auto bot = dynamic_cast<Sprite*>(b->getNode());
			//bot->runAction(FadeIn::create(0.5));
			this->preEndGamePlay();
		}
		else if (a->getCollisionBitmask() == BITMASK_BOTTANK && b->getCollisionBitmask() == BITMASK_TANK)
		{
			auto bot = dynamic_cast<Sprite*>(a->getNode());
			//bot->runAction(FadeIn::create(0.5));
			this->preEndGamePlay();
		}
	});


	return true;
}


bool GameScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	/*--------------------------------
	bird->stopAllActions();
	auto ac1 = MoveBy::create(0.25, Point(0, visibleSize.height / 5));
	auto ac2 = MoveBy::create(2, Point(0, -visibleSize.height));

	bird->runAction(Sequence::create(ac1, ac2, nullptr));
----------------------------------*/

	Rect rect(0, 0, _visibleSize.width * 0.5, _visibleSize.height*0.5);
	if (rect.containsPoint(touch->getLocation()))
	{
		_isControl = true;
		_controller->onTouchBegin(touch->getLocation());
	}
	else
	{
		_isControl = false;
	}
	
	/*Point directionVector;
	directionVector = Point(p.x - _controller->getPosition().x, p.y - _controller->getPosition().y);
	_tank->move(directionVector);*/
	//_tank->stopAllActions();
	//Point a = _tank->getPosition();
	//Point b = touch->getLocation();
	//float d = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	//_tank->runAction(MoveTo::create(d/50, touch->getLocation()));

	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//auto buffer = __String::createWithFormat("x: %f y: %f", _tank->getPositionX(), _tank->getPositionY());
	//_labelPos->setString(buffer->getCString());
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (!_isControl)
	{
		return;
	}

	Point p = touch->getLocation();
	Point directionVector;
	directionVector = Point(p.x - _controller->getPosition().x, p.y - _controller->getPosition().y);

	if (directionVector.x == 0 && directionVector.y == 0)
	{
		return;
	}

	//_tank->stop();
	_tank->move(directionVector);

	this->_controller->onControl(touch->getLocation());

	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//auto buffer = __String::createWithFormat("x: %f y: %f", _tank->getPositionX(), _tank->getPositionY());
	//_labelPos->setString(buffer->getCString());
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

}

void GameScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (!_isControl)
	{
		_isControl = true;
		return;
	}

	_controller->onControlEnded();
	_tank->stop();

}
//
//void HelloWorld::update(float dt)
//{
//	float dMin = 1000;
//	for (int i = 0; i < 4; i++)
//	{
//		Point a = tankBot[i]->getPosition();
//		Point b = tank->getPosition();
//		float dTmp = range(a, b);
//
//		if (dMin > dTmp)
//			dMin = dTmp;
//
//
//		float d = sqrt((a.x-b.x)*(a.x - b.x) + (a.y - b.y)*(a.y-b.y));
//	/*	if (d < rada->getContentSize().width*0.5)
//		{
//			rada->setTexture("rada2.png");
//		
//		}
//		else
//			rada->setTexture("rada1.png");*/
//	}
//
//
//	CCLOG("dMin: %f , radius: %f", dMin, rada->getContentSize().height / 2);
//	if (dMin < rada->getContentSize().height / 2)
//	{
//		float scale = dMin / (rada->getContentSize().height / 2);
//		CCLOG("scale: %f", scale);
//		rada->setScale(scale);
//	}
//}


float GameScene::range(Point a, Point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void GameScene::setTouchMoveable()
{
	_isTouchMoveable = true;
}

//HelloWorld::~HelloWorld()
//{
//	if (_botTank)
//	{
//		delete _botTank;
//	}
//}

void GameScene::onPauseGame()
{
	cocos2d::experimental::AudioEngine::pauseAll();
	_overlay = Sprite::create("overlay.png");
	_overlay->setScale(0.6f);
	_overlay->setPosition(Point(this->_visibleSize / 2));
	this->addChild(_overlay, Z_ORDER_TOP*10);

	_backgroundPause = Sprite::create("pausedmenu.png");
	_backgroundPause->setScale(0.6f);
	_backgroundPause->setPosition(Point(this->_visibleSize / 2));
	//backgroundPause->setAnchorPoint(Point::ZERO);
	this->addChild(_backgroundPause, Z_ORDER_TOP*10);

	auto restart = MenuItemImage::create("btnrestart.png", "btnrestart.png",
		[this](Ref * sender)
	{
		cocos2d::experimental::AudioEngine::resumeAll();
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
//	restart->setScale(0.6f);
	restart->setPosition(Point(_backgroundPause->getBoundingBox().size.width*0.4f, _backgroundPause->getBoundingBox().size.height*0.65f));

	auto play = MenuItemImage::create("btnplay.png", "btnplay.png",
		[this](Ref * sender)
	{
		cocos2d::experimental::AudioEngine::resumeAll();
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		_overlay->removeFromParentAndCleanup(true);
		_backgroundPause->removeFromParentAndCleanup(true);
		this->resumeNodeAndDescendants(this);
	});
	play->setScale(0.376f);
	play->setPosition(Point(_backgroundPause->getBoundingBox().size.width*0.84f, _backgroundPause->getBoundingBox().size.height*0.65f));

	auto returnMenu = MenuItemImage::create("btnmenu.png", "btnmenu.png",
		[this](Ref * sender)
	{
		cocos2d::experimental::AudioEngine::resumeAll();
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		Director::getInstance()->popScene();
	});
//	returnMenu->setScale(0.6f);
	returnMenu->setPosition(Point(_backgroundPause->getBoundingBox().size.width*1.28f, _backgroundPause->getBoundingBox().size.height*0.65f));

	auto menu = Menu::create(restart, play, returnMenu, nullptr);
	menu->setPosition(Point::ZERO);
	_backgroundPause->addChild(menu, 10000);
}

void GameScene::bulletUpdate()
{
	auto buffer = __String::createWithFormat("%d", _bulletCount);
	_labelBullet->setString(buffer->getCString());
}

void GameScene::scoreUpdate()
{
	auto buffer = __String::createWithFormat("%d", _score);
	_labelScore->setString(buffer->getCString());
}

void GameScene::endGamePlay()
{
	//this->pauseNodeAndDescendants(this);
	///
	auto soundID = cocos2d::experimental::AudioEngine::play2d("gameover.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);

	UserDefault *def = UserDefault::getInstance();
	_bestScore = def->getIntegerForKey("INSIDTANK_BESTSCORE", 0);
	_overlay = Sprite::create("overlay.png");
	_overlay->setScale(0.6f);
	_overlay->setPosition(Point(this->_visibleSize / 2));
	this->addChild(_overlay, Z_ORDER_TOP*10);

	_backgroundGameOver = Sprite::create("gameovermenu.png");
	_backgroundGameOver->setScale(0.6f);
	_backgroundGameOver->setPosition(Point(this->_visibleSize / 2));
	//backgroundPause->setAnchorPoint(Point::ZERO);
	this->addChild(_backgroundGameOver, Z_ORDER_TOP*10);

	auto restart = MenuItemImage::create("btnrestart.png", "btnrestart.png",
		[this](Ref * sender)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
	//	restart->setScale(0.6f);
	restart->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*1.12f, _backgroundGameOver->getBoundingBox().size.height*0.3f));

	auto returnMenu = MenuItemImage::create("btnmenu.png", "btnmenu.png",
		[this](Ref * sender)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		Director::getInstance()->popScene();
	});
	//	returnMenu->setScale(0.6f);
	returnMenu->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*0.56f, _backgroundGameOver->getBoundingBox().size.height*0.3f));

	auto menu = Menu::create(restart, returnMenu, nullptr);
	menu->setPosition(Point::ZERO);
	_backgroundGameOver->addChild(menu, 10000);

	Color3B color1;
	if (_score > _bestScore)
	{
		_bestScore = _score;
		color1 = Color3B(243, 121, 114);
	}
	else
	{
		color1 = Color3B(248, 169, 157);
	}
	def->setIntegerForKey("INSIDTANK_BESTSCORE", _bestScore);
	def->setIntegerForKey("INSIDTANK_COINS", _coins);
	def->flush();


	auto str = __String::create("Your Score:");
	auto labelScore = Label::createWithTTF(str->getCString(), "fonts/FredokaOne-Regular.ttf", 90);
	labelScore->setColor(color1);
	labelScore->setAnchorPoint(Point(0, 0));
	labelScore->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*0.2f, _backgroundGameOver->getBoundingBox().size.height*0.9f));
	_backgroundGameOver->addChild(labelScore, 10000);
	str = __String::createWithFormat("%d", _score);
	auto score = Label::createWithTTF(str->getCString(), "fonts/FredokaOne-Regular.ttf", 90);
	score->setColor(color1);
	score->setAnchorPoint(Point(1, 0));
	score->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*1.45f, _backgroundGameOver->getBoundingBox().size.height*0.9f));
	_backgroundGameOver->addChild(score, 10000);


	str = __String::create("Best Score:");
	auto labelBest = Label::createWithTTF(str->getCString(), "fonts/FredokaOne-Regular.ttf", 90);
	labelBest->setColor(Color3B(240, 86, 90));
	labelBest->setAnchorPoint(Point(0, 0));
	labelBest->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*0.2f, _backgroundGameOver->getBoundingBox().size.height*0.6f));
	_backgroundGameOver->addChild(labelBest, 10000);
	str = __String::createWithFormat("%d", _bestScore);
	auto best = Label::createWithTTF(str->getCString(), "fonts/FredokaOne-Regular.ttf", 90);
	best->setColor(Color3B(240, 86, 90));
	best->setAnchorPoint(Point(1, 0));
	best->setPosition(Point(_backgroundGameOver->getBoundingBox().size.width*1.45f, _backgroundGameOver->getBoundingBox().size.height*0.6f));
	_backgroundGameOver->addChild(best, 10000);

	///
	//auto replayButton = MenuItemImage::create("btnrestart.png", "btnrestart.png", [](Ref * sender)
	//{
	//	auto scene = HelloWorld::createScene();
	//	Director::getInstance()->replaceScene(scene);
	//});
	//replayButton->setPosition(_visibleSize / 2);
	//replayButton->setScale(4);
	//auto menu = Menu::create(replayButton, nullptr);
	//menu->setPosition(Point::ZERO);
	//this->addChild(menu);
}

void GameScene::preEndGamePlay()
{
	_isGameOver = true;
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	auto soundID = cocos2d::experimental::AudioEngine::play2d("exploding.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	_botTank->showAll();
	_controller->onControlEnded();
	_tank->stop();
	_tank->setVisible(false);

	auto particle = ParticleSystemQuad::create("fire.plist");
	particle->setPosition(Point(_tank->getPositionX(), _tank->getPositionY()));
	particle->setScale(0.4f);
	this->addChild(particle, Z_ORDER_EXLODING);
	auto action = Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(GameScene::endGamePlay, this)), nullptr);
	this->runAction(action);
}

void GameScene::shoot(Ref * sender)
{
	if (_bulletCount == 0 || _isGameOver)
	{
		return;
	}
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
	{
		float time = -1;
		time = this->_tank->shoot();
		_bulletCount--;
		if (_bulletCount == 0 && time != -1)
		{
			this->runAction(Sequence::create(DelayTime::create(time + 0.1), CallFunc::create([this]()
			{
				if (this->_itemDisplayCount == 0 && _bulletCount == 0)
				{
					this->preEndGamePlay();
				}
			}), nullptr));
		}
		this->bulletUpdate();
	});
}

//
//void HelloWorld::update(float dt)
//{
//	int n = _sceneWorld->getAllBodies().size();
//	for (int i = 0; i < n; i++)
//	{
//		Sprite* p = dynamic_cast<Sprite*>(_sceneWorld->getAllBodies().at(i)->getOwner());
//		p->setPosition(_sceneWorld->getAllBodies().at(i)->getPosition());
//		CCLOG("ppp %f - %f", _sceneWorld->getAllBodies().at(i)->getPosition().x/32.0, _sceneWorld->getAllBodies().at(i)->getPosition().y/32.0);
//	}
//}

void GameScene::pauseNodeAndDescendants(Node *pNode)
{
	pNode->pauseSchedulerAndActions();
	for (const auto &child : pNode->getChildren())
	{
		this->pauseNodeAndDescendants(child);
	}
}
 
void GameScene::resumeNodeAndDescendants(Node *pNode)
{
	pNode->resumeSchedulerAndActions();
	for (const auto &child : pNode->getChildren())
	{
		this->resumeNodeAndDescendants(child);
	}
}


void GameScene::raiseCoins(int coins, Point pos)
{
	auto _coinsStr = __String::createWithFormat("+ %i", coins);
	auto text = Label::createWithTTF(_coinsStr->getCString(), "fonts/FredokaOne-Regular.ttf", 40);
	text->setPosition(pos);
	this->addChild(text, Z_ORDER_TOP);
	auto _coin = Sprite::create("coin.png");
	_coin->setScale(0.6f);
	_coin->setPosition(text->getContentSize().width + _coin->getBoundingBox().size.width * 13 / 20, text->getContentSize().height / 2);
	text->addChild(_coin);
	text->runAction(Sequence::create(MoveBy::create(1, Point(0, _visibleSize.height / 20)), RemoveSelf::create(), nullptr));;

	auto buffer = __String::createWithFormat("%i", this->_coins);
	_labelCoins->setString(buffer->getCString());
}
