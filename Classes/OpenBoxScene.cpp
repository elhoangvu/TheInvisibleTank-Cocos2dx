#include "OpenBoxScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* OpenBoxScene::createScene(cocos2d::ui::Button** button, cocos2d::ui::PageView* pageView, std::string* charactor)
{

	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = OpenBoxScene::create();
	layer->_button = button;
	layer->_charactor = charactor;
	layer->_pageView = pageView;
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool OpenBoxScene::init()
{
	//////////////////////////////
	// 1. super init first
	
	if ( !Layer::init() )
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	UserDefault *def = UserDefault::getInstance();
	_effectVolume = def->getFloatForKey("INSIDTANK_EFFECTSOUND", 1.0f);

	_isChosen = false;
	auto background = Sprite::create("colorbackground.png");
	background->setScale(WORLD_SCALE);
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background);

	_def = UserDefault::getInstance();
	Point pos[] = { Point(_visibleSize.width / 6, _visibleSize.height / 2),
					Point(_visibleSize.width / 2, _visibleSize.height / 2),
					Point(_visibleSize.width * 5 / 6, _visibleSize.height / 2) };

	for (int i = 0; i < 3; i++)
	{
		_index[i] = RandomHelper::random_int(0, TANK_SKIN_COUNT - 1);
		_char[i] = Sprite::create("tankshadow" + StringUtils::format("%d", _index[i]) + ".png");
		_char[i]->setScale(1.0f);
		_char[i]->setPosition(pos[i]);
		this->addChild(_char[i]);
	}
	
	this->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(OpenBoxScene::createBoxes, this)), nullptr));

	return true;
}

void OpenBoxScene::createBoxes()
{
	_box[0] = MenuItemImage::create("giftbox.png", "giftbox.png", CC_CALLBACK_0(OpenBoxScene::onChoose, this, 0));
	_box[0]->setScale(WORLD_SCALE);
	_box[0]->setPosition(Point(_visibleSize.width / 6, _visibleSize.height / 2));

	_box[1] = MenuItemImage::create("giftbox.png", "giftbox.png", CC_CALLBACK_0(OpenBoxScene::onChoose, this, 1));
	_box[1]->setScale(WORLD_SCALE);
	_box[1]->setPosition(Point(_visibleSize.width / 2, _visibleSize.height / 2));

	_box[2] = MenuItemImage::create("giftbox.png", "giftbox.png", CC_CALLBACK_0(OpenBoxScene::onChoose, this, 2));
	_box[2]->setScale(WORLD_SCALE);
	_box[2]->setPosition(Point(_visibleSize.width * 5 / 6, _visibleSize.height / 2));

	auto menu = Menu::create(_box[0], _box[1], _box[2], nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
}

void OpenBoxScene::onChoose(int i)
{
	if (!_isChosen)
	{	
		auto soundID = cocos2d::experimental::AudioEngine::play2d("openbox.mp3", false);
		cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
		_isChosen = true;
		removeBoxes();
		if ((*_charactor)[_index[i]] == '0')
		{
			(*_charactor)[_index[i]] = '1';
			_button[_index[i]]->loadTextures("tank" + StringUtils::format("%d", _index[i]) + ".png", "tank" + StringUtils::format("%d", _index[i]) + ".png");
			_def->setStringForKey("INSIDTANK_TANKSKIND", *_charactor);
			_def->flush();
		}
		_pageView->setCurrentPageIndex(_index[i]);
		_char[i]->setTexture("tank" + StringUtils::format("%d", _index[i]) + ".png");

		auto btnOK = MenuItemImage::create("btnok.png", "btnok.png", CC_CALLBACK_0(OpenBoxScene::popScene, this));
		btnOK->setScale(WORLD_SCALE);
		btnOK->setPosition(Point(_visibleSize.width*0.5f, _visibleSize.height*0.2f));
		
		auto menu = Menu::create(btnOK, nullptr);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);
		
	}
}

void OpenBoxScene::popScene()
{
	auto soundID = cocos2d::experimental::AudioEngine::play2d("button.mp3", false);
	cocos2d::experimental::AudioEngine::setVolume(soundID, _effectVolume);
	Director::getInstance()->popScene();
}

void OpenBoxScene::removeBoxes()
{
	_box[0]->removeFromParentAndCleanup(true);
	_box[1]->removeFromParentAndCleanup(true);
	_box[2]->removeFromParentAndCleanup(true);
}

