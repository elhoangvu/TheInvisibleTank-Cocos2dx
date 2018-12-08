#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "ShopScene.h"
#include "SettingScene.h"
#include "InstructionScene.h"
#include "AudioEngine.h"

Scene* MenuScene::createScene()
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = MenuScene::create();
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if ( !Layer::init() )
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
	auto _origin = Director::getInstance()->getVisibleOrigin();

	UserDefault *def = UserDefault::getInstance();
	float volume = def->getFloatForKey("INSIDTANK_BACKGROUNDMUSIC", 1.0f);
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);
	_audioBackgroundID = cocos2d::experimental::AudioEngine::play2d("background.mp3", true);
	cocos2d::experimental::AudioEngine::setVolume(_audioBackgroundID, volume);

	auto charactor = def->getStringForKey("INSIDTANK_TANKSKIND", "100000000000.0");
	auto pos = charactor.find_last_of(".");
	std::string curChar = charactor.substr(pos + 1, charactor.size());
	
	_tank = Sprite::create("tank" + curChar + ".png");
	_tank->setScale(0.8f);
	_tank->setPosition(Point(_visibleSize.width*0.5f, _visibleSize.height*0.85f));
	this->addChild(_tank, 10000);

	_shadow = Sprite::create("tankshadow" + curChar + ".png");
	_shadow->setScale(0.8f);
	///
	auto line = LinearEquationHelper::createWithTwoPoint(Point::ZERO, _tank->getPosition());
	if (!line.isNotALine())
	{
		Point p1, p2;
		line.createPointWithDistance(_tank->getPosition(), p1, p2, _visibleSize.width / 150);

		Point directionVector = _tank->getPosition();
		Point p = p1 - _tank->getPosition();
		Point posDes;
		if (p.x * directionVector.x > 0 && p.y * directionVector.y > 0)
		{
			posDes = p1;
		}
		else
		{
			posDes = p2;
		}

		_shadow->setPosition(posDes);
	}
	this->addChild(_shadow, 1000);
	///

	auto background = Sprite::create("menubackground.png");
	background->setScale(WORLD_SCALE);
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background);

	auto btnInstruction = MenuItemImage::create("btninstruction.png", "btninstruction.png", [this](Ref * sender) 
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		auto scene = InstructionScene::createScene();
		Director::getInstance()->pushScene(scene);
	});

	btnInstruction->setScale(WORLD_SCALE);
	btnInstruction->setAnchorPoint(Point(0.5f,0));
	btnInstruction->setPosition(Point(_visibleSize.width*0.3f, _visibleSize.height*0.074f));

	auto btnPlay = MenuItemImage::create("btnplay.png", "btnplay.png", [this](Ref * sender) 
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		auto scene = GameScene::createScene();
		Director::getInstance()->pushScene(scene);
	});
	btnPlay->setScale(0.4f);
	btnPlay->setAnchorPoint(Point(0.5f, 0));
	btnPlay->setPosition(Point(_visibleSize.width*0.5f, _visibleSize.height*0.074f));

	auto btnShop = MenuItemImage::create("btnshop.png", "btnshop.png", [this](Ref * sender)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		//CallFunc::create(CC_CALLBACK_1(MenuScene::setTank));
		auto scene = ShopScene::createScene(CC_CALLBACK_1(MenuScene::setTank, this));
		Director::getInstance()->pushScene(scene);
	});
	btnShop->setScale(WORLD_SCALE);
	btnShop->setAnchorPoint(Point(0.5f, 0));
	btnShop->setPosition(Point(_visibleSize.width*0.7f, _visibleSize.height*0.074f));

	auto btnSetting = MenuItemImage::create("btnsetting.png", "btnsetting.png", [this](Ref * sender) 
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		auto scene = SettingScene::createScene(_audioBackgroundID, &_effectVolume);
		Director::getInstance()->pushScene(scene);
	});
	btnSetting->setScale(WORLD_SCALE);
	btnSetting->setPosition(Point(_visibleSize.width*0.0625f, _visibleSize.height*0.8889f));

	auto menu = Menu::create(btnInstruction, btnPlay, btnShop, btnSetting, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	return true;
}

void MenuScene::setTank(int index)
{
	_tank->setTexture("tank" + StringUtils::format("%d", index) + ".png");
	_shadow->setTexture("tankshadow" + StringUtils::format("%d", index) + ".png");
}
