#include "SplashScene.h"
#include "AudioEngine.h"
#include "MenuScene.h"

Scene* SplashScene::createScene()
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = SplashScene::create();
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if ( !Layer::init() )
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto splash = Sprite::create("splash.png");
	splash->setScale(WORLD_SCALE);
	splash->setAnchorPoint(Point::ZERO);
	splash->setPosition(Point::ZERO);
	this->addChild(splash);

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
	{
		this->preLoadImage();
	});

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
	{
		this->preLoadSound();
	});

	this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([]()
	{
		auto scene = MenuScene::createScene();
		auto trans = TransitionFade::create(0.3f, scene);
		Director::getInstance()->replaceScene(trans);
	}), nullptr));
	return true;
}

void SplashScene::preLoadImage()
{
	Director::getInstance()->getTextureCache()->addImage("additionalbg.png");
	Director::getInstance()->getTextureCache()->addImage("background.png");
	Director::getInstance()->getTextureCache()->addImage("boom.png");
	Director::getInstance()->getTextureCache()->addImage("bottank.png");
	Director::getInstance()->getTextureCache()->addImage("btnback.png");
	Director::getInstance()->getTextureCache()->addImage("btnchoice.png");
	Director::getInstance()->getTextureCache()->addImage("btninstruction.png");
	Director::getInstance()->getTextureCache()->addImage("btnmenu.png");
	Director::getInstance()->getTextureCache()->addImage("btnnochoice.png");
	Director::getInstance()->getTextureCache()->addImage("btnok.png");
	Director::getInstance()->getTextureCache()->addImage("btnopenbox.png");
	Director::getInstance()->getTextureCache()->addImage("btnpause.png");
	Director::getInstance()->getTextureCache()->addImage("btnplay.png");
	Director::getInstance()->getTextureCache()->addImage("btnrestart.png");
	Director::getInstance()->getTextureCache()->addImage("btnscanner.png");
	Director::getInstance()->getTextureCache()->addImage("btnsetting.png");
	Director::getInstance()->getTextureCache()->addImage("btnnext.png");
	Director::getInstance()->getTextureCache()->addImage("btnprev.png");
	Director::getInstance()->getTextureCache()->addImage("btnshoot.png");
	Director::getInstance()->getTextureCache()->addImage("btnshop.png");

	Director::getInstance()->getTextureCache()->addImage("bulletcount.png");
	Director::getInstance()->getTextureCache()->addImage("coin.png");
	Director::getInstance()->getTextureCache()->addImage("colorbackground.png");
	Director::getInstance()->getTextureCache()->addImage("control.png");
	Director::getInstance()->getTextureCache()->addImage("control2.png");
	Director::getInstance()->getTextureCache()->addImage("countdown.png");
	Director::getInstance()->getTextureCache()->addImage("explosion.png");
	Director::getInstance()->getTextureCache()->addImage("fire.png");

	Director::getInstance()->getTextureCache()->addImage("gameovermenu.png");
	Director::getInstance()->getTextureCache()->addImage("giftbox.png");
	Director::getInstance()->getTextureCache()->addImage("itemboom.png");
	Director::getInstance()->getTextureCache()->addImage("itemlazer.png");
	Director::getInstance()->getTextureCache()->addImage("itemshot.png");
	Director::getInstance()->getTextureCache()->addImage("lazer.png");
	Director::getInstance()->getTextureCache()->addImage("lazescanner.png");
	Director::getInstance()->getTextureCache()->addImage("menubackground.png");
	Director::getInstance()->getTextureCache()->addImage("overlay.png");

	Director::getInstance()->getTextureCache()->addImage("instruction0.png");
	Director::getInstance()->getTextureCache()->addImage("instruction1.png");
	Director::getInstance()->getTextureCache()->addImage("instruction2.png");
	Director::getInstance()->getTextureCache()->addImage("instruction3.png");

	Director::getInstance()->getTextureCache()->addImage("pausedmenu.png");
	Director::getInstance()->getTextureCache()->addImage("rada1.png");
	Director::getInstance()->getTextureCache()->addImage("scanner.png");
	Director::getInstance()->getTextureCache()->addImage("shopbackground.png");
	Director::getInstance()->getTextureCache()->addImage("shot.png");
	Director::getInstance()->getTextureCache()->addImage("sliderball.png");
	Director::getInstance()->getTextureCache()->addImage("sliderbar.png");

	Director::getInstance()->getTextureCache()->addImage("tank0.png");
	Director::getInstance()->getTextureCache()->addImage("tank1.png");
	Director::getInstance()->getTextureCache()->addImage("tank2.png");
	Director::getInstance()->getTextureCache()->addImage("tank3.png");
	Director::getInstance()->getTextureCache()->addImage("tank4.png");
	Director::getInstance()->getTextureCache()->addImage("tank5.png");
	Director::getInstance()->getTextureCache()->addImage("tank6.png");
	Director::getInstance()->getTextureCache()->addImage("tank7.png");
	Director::getInstance()->getTextureCache()->addImage("tank8.png");
	Director::getInstance()->getTextureCache()->addImage("tank9.png");
	Director::getInstance()->getTextureCache()->addImage("tank10.png");
	Director::getInstance()->getTextureCache()->addImage("tank11.png");

	Director::getInstance()->getTextureCache()->addImage("tankshadow0.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow1.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow2.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow3.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow4.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow5.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow6.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow7.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow8.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow9.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow10.png");
	Director::getInstance()->getTextureCache()->addImage("tankshadow11.png");
}

void SplashScene::preLoadSound()
{
	cocos2d::experimental::AudioEngine::preload("background.mp3");
	cocos2d::experimental::AudioEngine::preload("boom.mp3");
	cocos2d::experimental::AudioEngine::preload("button.mp3");
	cocos2d::experimental::AudioEngine::preload("chose.mp3");
	cocos2d::experimental::AudioEngine::preload("exploding.mp3");
	cocos2d::experimental::AudioEngine::preload("gameover.mp3");
	cocos2d::experimental::AudioEngine::preload("lazer.mp3");
	cocos2d::experimental::AudioEngine::preload("scanner.mp3");
	cocos2d::experimental::AudioEngine::preload("normalbullet.mp3");
	cocos2d::experimental::AudioEngine::preload("openbox.mp3");
	cocos2d::experimental::AudioEngine::preload("selectskin.mp3");
	cocos2d::experimental::AudioEngine::preload("tankmoving.mp3");
}
