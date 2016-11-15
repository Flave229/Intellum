#pragma once

class Box
{
public:
	float _width;
	float _height;

	Box() : Box(0, 0) {}
	Box(float width, float height) : _width(width), _height(height) {}
};