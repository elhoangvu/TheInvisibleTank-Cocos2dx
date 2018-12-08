#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Tank.h"
#include "BotTank.h"
#include "Controller360.h"
#include "Box2D\Box2D.h"
USING_NS_CC;
class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	bool onContactBegin(cocos2d::PhysicsContact & contact);
	void pauseNodeAndDescendants(Node *pNode);
	void resumeNodeAndDescendants(Node *pNode);
	float range(Point a, Point b);
	void setTouchMoveable();
	void raiseCoins(int coins, Point pos);
//	void update(float dt);
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	//~HelloWorld();
private:
	void onPauseGame();
	bool _isTouchMoveable = true;
	Size _visibleSize;
	void bulletUpdate();
	void scoreUpdate();
	void endGamePlay();
	void preEndGamePlay();
	void shoot(Ref * sender);
	cocos2d::PhysicsWorld* _sceneWorld;
	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { _sceneWorld = world; }
	Tank* _tank;
	BotTank* _botTank;
	Controller360* _controller;
	Label* _labelBullet;
	Label* _labelScore;
	Label* _labelCoins;
	int _bulletCount;
	int _itemDisplayCount;
	int _score;
	int _coins;
	int _plusedCoins;
	int _bestScore;
	float _effectVolume;
	MenuItemImage* _scanner;
	bool isScanable = true;
	MenuItemImage* _shoot;
	bool _isControl = true;
	bool _isGameOver = false;
	Sprite* _btnBullet;
	Sprite* _overlay;
	Sprite* _backgroundPause;
	Sprite* _backgroundGameOver;
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	///Label* _labelPos;
	///->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
};

#endif // __GAME_SCENE_H__
