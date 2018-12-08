#ifndef __BOT_TANK_H__
#define __BOT_TANK_H__

#include "cocos2d.h"
#include "Shadow.h"

USING_NS_CC;
class BotTank: public Node
{
public:
	static BotTank* createTank(Layer* layer);
	void addBot(const Rect& prohibitedRect, int n);
	Sprite* getBot(int i);
	int getBotCount();
	void removeBotTank(Sprite* bot);
	void run();
	void setScale(float scale);
	Shadow* findShadow(Sprite* bot);
	void showAll();
	Vector<Sprite*> getBot();
	BotTank();
	~BotTank();
private:
	void preRun(Sprite* bot);
	Point randomPosition(const Rect& prohibitedRect);
private:
	Vector<Shadow*> _shadow;
	Size _visibleSize;
	Vector<Sprite*> _bot;
	Layer* _layer;
	float _scale;
	Sprite* _additionalBG;
};

#endif // __BOT_TANK_H__
