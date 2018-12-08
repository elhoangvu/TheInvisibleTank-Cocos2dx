#include "InstructionScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* InstructionScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InstructionScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool InstructionScene::init()
{
    //////////////////////////////
    // 1. super init first
	
    if ( !Layer::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);

	_index = 0;
	_instructionName[0] = "instruction0.png";
	_instructionName[1] = "instruction1.png";
	_instructionName[2] = "instruction2.png";
	_instructionName[3] = "instruction3.png";

	_instructionBG = Sprite::create(_instructionName[0]);
	_instructionBG->setAnchorPoint(Point::ZERO);
	_instructionBG->setScale(WORLD_SCALE);
	_instructionBG->setPosition(Point::ZERO);
	this->addChild(_instructionBG);

	auto btnNext = MenuItemImage::create("btnnext.png", "btnnext.png", [this](Ref * sender)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		if (_index == 3)
		{
			_index = 0;
		}
		else
		{
			_index++;
		}
		_instructionBG->setTexture(_instructionName[_index]);
	});
	btnNext->setScale(WORLD_SCALE);
	btnNext->setPosition(Point(visibleSize.width*0.92, visibleSize.height*0.075));

	auto btnPrev = MenuItemImage::create("btnprev.png", "btnprev.png", [this](Ref * sender)
	{
		auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		if (_index == 0)
		{
			_index = 3;
		}
		else
		{
			_index--;
		}
		_instructionBG->setTexture(_instructionName[_index]);
	});
	btnPrev->setScale(WORLD_SCALE);
	btnPrev->setPosition(Point(visibleSize.width*0.08, visibleSize.height*0.075));

	auto btnMenu = MenuItemImage::create("btnmenu.png", "btnmenu.png", CC_CALLBACK_0(InstructionScene::popScene, this));
	btnMenu->setScale(WORLD_SCALE*0.8);
	btnMenu->setPosition(Point(visibleSize.width*0.0625f, visibleSize.height*0.8889f));

	auto menu = Menu::create(btnNext, btnPrev, btnMenu, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

    return true;
}

void InstructionScene::popScene()
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	Director::getInstance()->popScene();
}
