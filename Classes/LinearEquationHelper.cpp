#include "LinearEquationHelper.h"

float LinearEquationHelper::distance(const Vec2 & po1, const Vec2 & po2)
{
	float x = (po1.x - po2.x);
	float y = (po1.y - po2.y);
	return sqrt(x*x + y*y);
}

Point LinearEquationHelper::_createPointWithDistance(const Vec2& pointScr, const Vec2& pointDes, float d)
{
	if (LinearEquationHelper::distance(pointScr, pointDes) <= d)
	{
		return pointDes;
	}
	if (a == 0)
	{
		float x;
		int direc = 1;
		if (b < 0)
		{
			direc = -1;
		}
		if (abs(pointDes.x- pointScr.x) <= d)
		{
			x = pointScr.x;
		}
		else
		{
			x = pointScr.x + direc*d;
		}
		return Point(x, pointScr.y);
	}
	else if (b == 0)
	{
		float y;
		int direc = 1;
		if (a > 0)
		{
			direc = -1;
		}
		if (abs(pointDes.y - pointScr.y) <= d)
		{
			y = pointScr.y;
		}
		else
		{
			y = pointScr.y + direc*d;
		}
		return Point(pointScr.x, y);
	}
	
	double A = a*a / (b*b) + 1;
	double B = -pointScr.x + pointScr.y*a / b + a*c / (b*b);
	double C = pointScr.x*pointScr.x + pointScr.y*pointScr.y + 2 * pointScr.y*c / b + c*c / (b*b) - d*d;
	double delta = B*B - A*C;
	Point p1, p2;
	p1.x = (-B + sqrt(delta)) / A;
	p1.y = -a*p1.x / b - c / b;
	p2.x = (-B - sqrt(delta)) / A;
	p2.y = -a*p2.x / b - c / b;
	if (LinearEquationHelper::distance(p1, pointDes) < LinearEquationHelper::distance(p2, pointDes))
	{
		return p1;
	}
	return p2;
}


void LinearEquationHelper::_createPointWithDistance(const Vec2& pointScr, Vec2& point1, Vec2& point2, float d)
{
	if (a == 0)
	{		
		point1 = Vec2(pointScr.x, pointScr.y + d);
		point2 = Vec2(pointScr.x, pointScr.y - d);
		return;
	}
	else if (b == 0)
	{
		point1 = Vec2(pointScr.x + d, pointScr.y);
		point2 = Vec2(pointScr.x - d, pointScr.y);
		return;
	}

	double _c = b*pointScr.x - a*pointScr.y;
	double A = (-b)*(-b) / (a*a) + 1;
	double B = -pointScr.x + pointScr.y*(-b) / a + (-b)*_c / (a*a);
	double C = pointScr.x*pointScr.x + pointScr.y*pointScr.y + 2 * pointScr.y*_c / a + _c*_c / (a*a) - d*d;
	double delta = B*B - A*C;
	point1.x = (-B + sqrt(delta)) / A;
	point1.y = -(-b)*point1.x / a - _c / a;
	point2.x = (-B - sqrt(delta)) / A;
	point2.y = -(-b)*point2.x / a - _c / a;
}

bool LinearEquationHelper::isNotALine()
{
	return _isNaL;
}

void LinearEquationHelper::drawDashedLine(DrawNode * drawNode, const Vec2& pointScr, const Vec2& pointDes, float dLine, float dSpace)
{
	if (!_isNaL)
	{		
		Point _pointScr = pointScr;
		while (_pointScr != pointDes)
		{
			Point p;
			Point point1, point2;

			p = _createPointWithDistance(_pointScr, pointDes, dLine);
			_createPointWithDistance(_pointScr, point1, point2, dLine*0.5);
			drawNode->drawTriangle(point1, point2, p, Color4F(1, 1, 1, 1));
		//	drawNode->drawSegment(_pointScr, p, 3, Color4F(1, 1, 1, 0.2));
			//drawNode->drawSegment(_pointScr, p, 2, Color4F(1, 1, 1, 0.5));
			_pointScr = _createPointWithDistance(p, pointDes, dSpace);
		}
	}
}

LinearEquationHelper LinearEquationHelper::createWithTwoPoint(const Vec2& p1, const Vec2& p2)
{
	LinearEquationHelper l;

	if (p1 == p2)
	{
		l._isNaL = true;
	}
	else
	{
		l._isNaL = false;
		l.a = round(p1.y - p2.y);
		l.b = round(p2.x - p1.x);
		l.c = round(-l.a*p1.x - l.b*p1.y);
	}

	return l;
}

LinearEquationHelper LinearEquationHelper::createWithDirectionVector(const Vec2 & directionVector, const Vec2 & point)
{
	LinearEquationHelper l;
	if (directionVector == Vec2::ZERO)
	{
		l._isNaL = true;
	}
	else
	{
		l.a = directionVector.y;
		l.b = -directionVector.x;
		l.c = - l.a*point.x - l.b*point.y;
		if (l.a != 0)
		{

			l.b /= l.a;
			l.c /= l.a;
			l.a = 1;
		}
		else if (l.b != 0)
		{

			l.a /= l.b;
			l.c /= l.b;
			l.b = 1;
		}
	}
	return l;
}

Point LinearEquationHelper::createPointWithY(float y)
{
	if (a == 0)
	{
		return 0;
	}
	return Point((b*y - c) / a, y);
}

bool LinearEquationHelper::createPointWithDistance(const Vec2 & pointScr, Vec2 & point1, Vec2 & point2, float d)
{
	if (_isNaL || (a == 0 && b == 0))
	{
		return false;
	}
	if (b == 0)
	{
		point1.x = -c / a;
		point2.x = -c / a;

		point1.y = d + pointScr.y;
		point2.y = -d + pointScr.y;
	}
	else
	{
		double A = ((a / b)*(a / b) + 1);
		double B = -pointScr.x + (a / b)*(pointScr.y + c / b);
		double C = (pointScr.y + c / b)*(pointScr.y + c / b) + pointScr.x*pointScr.x - d*d;
		double delta = B*B - A*C;
		point1.x = (-B + sqrt(delta)) / A;
		point1.y = (a*point1.x + c) / -b;
		point2.x = (-B - sqrt(delta)) / A;
		point2.y = (a*point2.x + c) / -b;
	}
	return true;
}

LinearEquationHelper::LinearEquationHelper()
{
	a = 1;
	b = 1;
	c = 1;
	_isNaL = false;
}
