#pragma once
#include <raylib.h>
class Button
{
public:
	Button(Vector2 position);
	~Button();
	void draw(const char* text);
	bool checkCollision(Vector2 mousePos, bool mousePressed);
	void update();
private:
	Vector2 position;
	bool goingUp = true;
	float hoverOffset = 0;
};
