#include "SettingScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* SettingScene::createScene(int audioID, float* effectVolume)
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();
	layer->_audioID = audioID;
	layer->_effectVolume = effectVolume;
	layer->launch();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    //////////////////////////////
    // 1. super init first
	
    if ( !Layer::init() )
    {
        return false;
    }

	_visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("colorbackground.png");
	background->setScale(0.8f);
	background->setPosition(_visibleSize / 2);
	this->addChild(background);

	auto backButton = MenuItemImage::create("btnback.png", "btnback.png", CC_CALLBACK_0(SettingScene::popScene, this));
	backButton->setScale(WORLD_SCALE);
	backButton->setPosition(Point(_visibleSize.width*0.0625f, _visibleSize.height*0.8889f));

	auto menu = Menu::create(backButton, nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
    return true;
}

void SettingScene::popScene()
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, *_effectVolume);
	auto def = UserDefault::getInstance();
	def->setFloatForKey("INSIDTANK_BACKGROUNDMUSIC", cocos2d::experimental::AudioEngine::getVolume(_audioID));
	def->setFloatForKey("INSIDTANK_EFFECTSOUND", *_effectVolume);
	def->flush();
	Director::getInstance()->popScene();
}

void SettingScene::launch()
{
	createBMSlider();
	createESSlider();
}

void SettingScene::createBMSlider()
{
	float volume = cocos2d::experimental::AudioEngine::getVolume(_audioID);
	auto slider = ui::Slider::create();
	slider->loadBarTexture("sliderbar.png");
	slider->loadSlidBallTextureNormal("sliderball.png");
	//slider->loadProgressBarTexture("sliderBar_left.png");
	slider->setScale(WORLD_SCALE);
	slider->setAnchorPoint(Point(0, 0.5f));
	slider->setPosition(Point(_visibleSize.width*0.075f, _visibleSize.height*0.57f));
	slider->setPercent(volume * 100);
	this->addChild(slider, 100);
	slider->addEventListener(CC_CALLBACK_2(SettingScene::setVolumeBM, this));
	auto _labelMusic = Label::createWithTTF("Background", "fonts/FredokaOne-Regular.ttf", 50);
	_labelMusic->setAnchorPoint(Point::ZERO);
	_labelMusic->setPosition(Point(_visibleSize.width*0.075f, _visibleSize.height * 0.65f));
	_labelMusic->setColor(Color3B(240, 86, 90));


	auto buffer = __String::createWithFormat("%d %%", (int)(volume * 100));
	_labelBgrPer = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 35);
	_labelBgrPer->setAnchorPoint(Point(0, 0.5f));
	_labelBgrPer->setColor(Color3B(240, 86, 90));
	_labelBgrPer->setPosition(Point(_visibleSize.width*0.135f + slider->getBoundingBox().size.width, _visibleSize.height*0.57f));
	this->addChild(_labelBgrPer);
	this->addChild(_labelMusic);
}


void SettingScene::createESSlider()
{
	float volume = *_effectVolume;
	auto slider = ui::Slider::create();
	slider->loadBarTexture("sliderbar.png");
	slider->loadSlidBallTextureNormal("sliderball.png");
	//slider->loadProgressBarTexture("sliderBar_left.png");
	slider->setScale(WORLD_SCALE);
	slider->setAnchorPoint(Point(0, 0.5f));
	slider->setPosition(Point(_visibleSize.width*0.075f, _visibleSize.height*0.27f));
	slider->setPercent(volume * 100);
	this->addChild(slider, 100);
	slider->addEventListener(CC_CALLBACK_2(SettingScene::setVolumeES, this));
	auto _labelMusic = Label::createWithTTF("Effect", "fonts/FredokaOne-Regular.ttf", 50);
	_labelMusic->setAnchorPoint(Point::ZERO);
	_labelMusic->setPosition(Point(_visibleSize.width*0.075f, _visibleSize.height * 0.35f));
	_labelMusic->setColor(Color3B(240, 86, 90));


	auto buffer = __String::createWithFormat("%d %%", (int)(volume * 100));
	_labelEffPer = Label::createWithTTF(buffer->getCString(), "fonts/FredokaOne-Regular.ttf", 35);
	_labelEffPer->setAnchorPoint(Point(0, 0.5f));
	_labelEffPer->setColor(Color3B(240, 86, 90));
	_labelEffPer->setPosition(Point(_visibleSize.width*0.135f + slider->getBoundingBox().size.width, _visibleSize.height*0.27f));
	this->addChild(_labelEffPer);
	this->addChild(_labelMusic);
}

void SettingScene::setVolumeBM(cocos2d::Ref* sender, ui::Slider::EventType type)
{
	auto slider = dynamic_cast<ui::Slider*>(sender);
	if (ui::Slider::EventType::ON_PERCENTAGE_CHANGED == type)
	{
		cocos2d::experimental::AudioEngine::setVolume(_audioID, slider->getPercent() / 100.0f);
		auto buffer = __String::createWithFormat("%d %%", (int)slider->getPercent());
		_labelBgrPer->setString(buffer->getCString());
	}
}


void SettingScene::setVolumeES(cocos2d::Ref* sender, ui::Slider::EventType type)
{
	auto slider = dynamic_cast<ui::Slider*>(sender);
	if (ui::Slider::EventType::ON_PERCENTAGE_CHANGED == type)
	{
		*_effectVolume = slider->getPercent() / 100.0f;
		auto buffer = __String::createWithFormat("%d %%", (int)slider->getPercent());
		_labelEffPer->setString(buffer->getCString());
	}
}