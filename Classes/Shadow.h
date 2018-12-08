#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "cocos2d.h"
#include "LinearEquationHelper.h"

USING_NS_CC;
class Shadow : public Sprite
{
private:
	Point _lastParentPos;
	Point _lightSource;
	float _delta;
	Sprite* _parent;
public:
	static Shadow* createShadow(const std::string& filename);
	void setParent(Sprite* parent);
	
	void setDelta(float dt);
	void setLightSource(Point pos);
	void activate();
	void update(float dt);
	//void onUpdate(Sprite* parent);
	Shadow();
};

#endif // __SHADOW_H__
