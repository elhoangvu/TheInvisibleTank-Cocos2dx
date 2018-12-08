#ifndef __LINEAR_EQUATION_HELPER_H__
#define __LINEAR_EQUATION_HELPER_H__
#include "cocos2d.h"
USING_NS_CC;

class LinearEquationHelper
{
private:
	bool _isNaL;
	void _createPointWithDistance(const Vec2& pointScr, Vec2& point1, Vec2& point2, float d);
	Point _createPointWithDistance(const Vec2& pointScr, const Vec2& pointDes, float d);
public:
	double a;
	double b;
	double c;
public:
	bool isNotALine();
	static float distance(const Vec2& po1, const Vec2& po2);
	void drawDashedLine(DrawNode* drawNode, const Vec2& pointScr, const Vec2& pointDes, float dLine, float dSpace);
	static LinearEquationHelper createWithTwoPoint(const Vec2& p1, const Vec2& p2);
	static LinearEquationHelper createWithDirectionVector(const Vec2& directionVector, const Vec2& point);
	Point createPointWithY(float y);
	bool createPointWithDistance(const Vec2& pointScr, Vec2& point1, Vec2& point2, float d);
	LinearEquationHelper();
};

#endif