#ifndef __TANK_H__
#define __TANK_H__

#include "cocos2d.h"
#include "BotTank.h"
#include "LinearEquationHelper.h"
#include "BulletFactory.h"
#include "Shadow.h"

USING_NS_CC;
class Tank : public cocos2d::Sprite
{
public:
	static Tank* createTank(Layer* layer, std::string filename);
	Sprite* getStaticRada();
	void update(float dt);
	void setBotTank(BotTank* botTank);
	void move(Point directionVector);
	float shoot();
	void stop();
	Rect getBoundingAllBox();
	void setBulletType(int bulletType);
	float getShootingTime();
	void setShadow(Shadow* shadow);
	void setVisible(bool isVisible);
	//void setAnchorPoint(Point pos);
	Tank();
private:
	Shadow* _shadow;
	void _moveForward(float dt);
	Size _visibleSize;
	BotTank* _botTank;
	Layer* _layer;
	Sprite* _dynamicRada;
	Sprite* _staticRada;
	LinearEquationHelper _line;
	Point _directionVector;
	float _alpha;
	bool _isMoving;
	float _deltaMove;
	int _bulletType;
	float _effectVolume;
	int _movingSoundID;
	bool isup = true;
	Sprite* dd;
};

#endif // __HELLOWORLD_SCENE_H__
