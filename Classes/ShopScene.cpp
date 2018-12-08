#include "ShopScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* ShopScene::createScene(const std::function<void(int)> &func)
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = ShopScene::create();
	layer->setTankFunc = func;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ShopScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if ( !Layer::init() )
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("shopbackground.png");
	background->setScale(WORLD_SCALE);
	background->setPosition(_visibleSize / 2);
	this->addChild(background);

	_def = UserDefault::getInstance();
	_effectVolume = _def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
	_charactor = _def->getStringForKey("INSIDTANK_TANKSKIND", "100000000000.0");
	//def->setStringForKey("INSIDTANK_TANKSKIND", "100000000000.0");
	auto pos = _charactor.find_last_of(".");
	std::string curChar = _charactor.substr(pos + 1, TANK_SKIN_COUNT);
	//for (int i = 0; i <= curChar.size(); i++)
	//{
	//	_charactor.pop_back();
	//}

	_curCharactor = atoi(curChar.c_str());

	_coins = _def->getIntegerForKey("INSIDTANK_COINS", 0);
	_pageView = ui::PageView::create();
	_pageView->setPosition(_visibleSize / 2);

	_pageView->setTouchEnabled(true);
	//pageView->setBounceEnabled(true);
	_pageView->setContentSize(Size(_visibleSize.width, _visibleSize.height / 3));
	//pageView->setClippingEnabled(true);
	_pageView->setClippingEnabled(false);
	_pageView->setAnchorPoint(Vec2::ZERO);
	_pageView->setPosition(Point(0, _visibleSize.height / 2));
	_pageView->setSwallowTouches(false);

	_pageView->setItemsMargin(-_visibleSize.width / 100);
	_pageView->setCurrentPageIndex(_curCharactor);


	for (int i = 0; i < TANK_SKIN_COUNT; i++)
	{
		std::string name = "tank";
		if (_charactor[i] == '0')		{
			name += "shadow";
		}
		name += StringUtils::format("%d", i) + ".png";
		_buttonChar[i] = ui::Button::create(name);
		_buttonChar[i]->setScale(0.4f);
		_pageView->pushBackCustomItem(_buttonChar[i]);
	}

	_pageView->setCurrentPageIndex(_curCharactor);
	this->addChild(_pageView);

	_btnChoice = MenuItemImage::create("btnchoice.png", "btnchoice.png", CC_CALLBACK_0(ShopScene::setCharactor, this));
	_btnChoice->setScale(0.4f);
	_btnChoice->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.4);
	
	auto backButton = MenuItemImage::create("btnback.png", "btnback.png", CC_CALLBACK_0(ShopScene::popScene, this));
	backButton->setScale(WORLD_SCALE);
	backButton->setPosition(Point(_visibleSize.width*0.0625f, _visibleSize.height*0.8889f));

	_btnOpenBox = MenuItemImage::create("btnopenbox.png", "btnopenbox.png", CC_CALLBACK_0(ShopScene::goToCardScene, this));
	_btnOpenBox->setScale(WORLD_SCALE);
	_btnOpenBox->setPosition(_visibleSize.width / 2, _visibleSize.height / 6);

	auto menu = Menu::create(_btnChoice, backButton, _btnOpenBox, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//labelCoins = Sprite::create("labelCoins.png");
	//labelCoins->setScale(0.8f);
	//labelCoins->setAnchorPoint(Point(1, 0.5f));
	//labelCoins->setPosition(Point(origin.x + visibleSize.width, origin.y + visibleSize.height * 9 / 10));

	auto coins = __String::createWithFormat("%i", _coins);
	_labelCoins = Label::createWithTTF(coins->getCString(), "fonts/FredokaOne-Regular.ttf", 50);
	_labelCoins->setPosition(Point(_visibleSize.width*0.875f, _visibleSize.height*0.889f));
	this->addChild(_labelCoins);

	_labelUnlock = Label::createWithTTF("", "fonts/FredokaOne-Regular.ttf", 30);
	_labelUnlock->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.33);
	_labelUnlock->setColor(Color3B(204, 54, 69));
	this->addChild(_labelUnlock);

	this->scheduleUpdate();
	return true;
}

void ShopScene::update(float dt)
{
	int index = _pageView->getCurrentPageIndex();
	static int oldIndex = index;
	if (oldIndex != index)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("selectskin.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		oldIndex = index;
	}
	for (int i = 0; i < TANK_SKIN_COUNT; i++)
	{
		if (i != index)
		{
			_pageView->getItem(i)->setScale(0.4f);
		}
		else
		{
			_pageView->getCenterItemInCurrentView()->setScale(1.2f);
		}
	}
	if (_charactor[index] == '0')
	{
		if (_labelUnlock->getString() == "")
			_labelUnlock->setString("Locked");
	}
	else
	{
		if (_labelUnlock->getString() == "Locked")
			_labelUnlock->setString("");
	}
	if (index != _curCharactor)
	{
		_btnChoice->setNormalImage(Sprite::create("btnnochoice.png"));
	}
	else
	{
		_btnChoice->setNormalImage(Sprite::create("btnchoice.png"));
	}
}

void ShopScene::goToCardScene()
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	if (_coins >= 100)
	{
		_coins -= 100;
		_def->setIntegerForKey("INSIDTANK_COINS", _coins);
		_def->flush();
		auto coins = __String::createWithFormat("%i", _coins);
		_labelCoins->setString(coins->getCString());
		auto scene = OpenBoxScene::createScene(_buttonChar, _pageView, &_charactor);
		Director::getInstance()->pushScene(scene);
	}
	else
	{
		_labelCoins->runAction(Sequence::create(MoveBy::create(0.03f, Point(0,_visibleSize.height/100)), MoveBy::create(0.03f, Point(0, -_visibleSize.height / 100)), MoveBy::create(0.03f, Point(0, _visibleSize.height / 100)), MoveBy::create(0.03f, Point(0, -_visibleSize.height / 100)), nullptr));
	}
}

void ShopScene::setCharactor()
{
	if (_charactor[_pageView->getCurrentPageIndex()] == '1')
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("chose.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		_curCharactor = _pageView->getCurrentPageIndex();
	}
}

void ShopScene::popScene()
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	_def->setStringForKey("INSIDTANK_TANKSKIND", _charactor + '.' + StringUtils::format("%d", _curCharactor));
	_def->flush();
	setTankFunc(_curCharactor);
	//passenger->setPassenger(_curCharactor);

	Director::getInstance()->popScene();
	//Director::getInstance()->popScene();
}

