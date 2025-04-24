#include "Button.h"

Button::Button(Vector2 position)
{
	this->position = position;
}

Button::~Button()
{
}

void Button::draw(const char* text)
{
	Rectangle rect = { position.x, position.y+hoverOffset, 300, 100 };

	bool isHovered = CheckCollisionPointRec(GetMousePosition(), rect);

	Color buttonColor = isHovered ? LIGHTGRAY : RAYWHITE;

	DrawRectangleRounded(rect, 0.8, 0, buttonColor);
	int textX = position.x + (300 - MeasureText(text, 50)) / 2;
	DrawText(text, textX, position.y + 25, 50, BLACK);
}

bool Button::checkCollision(Vector2 mousePos, bool mousePressed)
{
	Rectangle rect = { position.x,position.y,300,100 };
	if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
	{
		return true;
	}
	return false;
}

void Button::update()
{
	if (goingUp)
		hoverOffset -= 0.1f;
	else
		hoverOffset += 0.1f;
	if (hoverOffset <= -2) {
		goingUp = false;
	}
	if (hoverOffset >= 2) {
		goingUp = true;
	}
}
