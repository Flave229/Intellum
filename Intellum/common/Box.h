#pragma once

class Box
{
public:
	float Width;
	float Height;

	Box() : Box(0, 0) {}
	Box(float width, float height) : Width(width), Height(height) {}
};