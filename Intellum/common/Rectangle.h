#pragma once

class Rectangle
{
public:
	float _width;
	float _height;

	Rectangle() : Rectangle(0, 0) {}
	Rectangle(float width, float height) : _width(width), _height(height) {}
};