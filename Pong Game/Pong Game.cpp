#include <iostream>
#include <raylib.h>
#include "Button.h"

const int screenWidth = 1280;
const int screenHeight = 800;

int fadeAlpha = 255;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };
Color MenuBackground = Color{ 27, 79, 114, 255 };

int playerScore = 0;
int cpuScore = 0;

Button start({screenWidth/2-150,screenHeight/2 - 100});
Button end({ screenWidth / 2 - 150,screenHeight / 2 + 50 });

typedef enum GameScreen { LOADING = 0, MENU, GAMEPLAY, ENDING} GameScreen;

GameScreen currentScreen = LOADING;

class Ball
{
public:
	float x = screenWidth/2;
	float y = screenHeight / 2;
	float r = 20;
	int speedX = 7;
	int speedY = 7;
	int speedChoice[2] = { -1,1 };
	void draw()
	{
		DrawCircle(x, y, r, Yellow);
	}

	void update()
	{
		x += speedX;
		y += speedY;

		if (y + r >= GetScreenHeight() || y - r <= 0)
		{
			speedY *= -1;
		}
		if (x + r >= GetScreenWidth() || x - r <= 0)
		{
			speedX *= -1;
		}
		if (x + r >= GetScreenWidth()) {
			playerScore++;
			resetBall();
		}
		if (x - r <= 0)
		{
			cpuScore++;
			resetBall();
		}
	}
	void resetBall() 
	{
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		speedX *= speedChoice[GetRandomValue(0, 1)];
		speedY *= speedChoice[GetRandomValue(0, 1)];
	}
 };

class Paddle
{
public:
	float width = 25;
	float height = 120;
	float x = 5;
	float y = screenHeight / 2 - height / 2;
	int speed = 6;

	void draw() {
		DrawRectangleRounded(Rectangle{x, y, width, height },0.8,0, WHITE);
	}
	void correct() {
		if (y + height > screenHeight)
		{
			y = screenHeight - height;
		}
		if (y <= 0)
		{
			y = 0;
		}
	}
	void update() {
		if(IsKeyDown(KEY_UP))
		{
			y -= speed;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y += speed;
		}
		correct();
	}
};

class Cpu : public Paddle {
public:
	Cpu()
	{
		x = screenWidth - 5 - width;
		y = screenHeight / 2 - height / 2;
	}

	void update(int ballY) {
		if (y + height / 2 > ballY)
		{
			y -= speed;
		}
		if (y + height / 2 < ballY)
		{
			y += speed;
		}
		correct();
	}
};

Ball b;
Paddle p;
Cpu c;
int main()
{
	InitWindow(screenWidth, screenHeight, "Pong Game");
	SetTargetFPS(60);


	int textWidth = MeasureText("Pong Game", 100);
	int centerX = (screenWidth - textWidth) / 2;
	int centerY = screenHeight / 2;


	while (!WindowShouldClose()) {
		BeginDrawing();

		float time = GetTime();
		unsigned char flickerAlpha = (sin(time * 10) > 0) ? 255 : 0;
#pragma region update
		switch (currentScreen)
		{
		case LOADING:
			if (fadeAlpha > 0)
				fadeAlpha -= 2;
			else
				currentScreen = MENU;
			break;
		case MENU :
			start.update();
			end.update();
			if (start.checkCollision(GetMousePosition(), IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) 
			{
				currentScreen = GAMEPLAY;
				fadeAlpha = 0;
			}
			if (end.checkCollision(GetMousePosition(), IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) 
			{
				CloseWindow();
				return 0;
			}break;
		case GAMEPLAY : 
			b.update();
			c.update(b.y);
			p.update();

			if (CheckCollisionCircleRec(Vector2{ b.x,b.y }, b.r, Rectangle{ p.x, p.y, p.width, p.height }))
			{
				b.speedX *= -1;
			}
			if (CheckCollisionCircleRec(Vector2{ b.x,b.y }, b.r, Rectangle{ c.x, c.y, c.width, c.height }))
			{
				b.speedX *= -1;
			}
			if (IsKeyPressed(KEY_SPACE))
			{
				currentScreen = MENU;
			}break;
		}
#pragma endregion
		

#pragma region draw
		switch (currentScreen)
		{
		case LOADING:
			ClearBackground(MenuBackground);
			DrawText("PONG GAME", centerX , centerY - 10, 100, Color{ 255, 255, 255, (unsigned char)fadeAlpha });
			break;
		case MENU :
			ClearBackground(MenuBackground);
			DrawText("PONG GAME", screenWidth / 2 - 150, 100, 60, Color{ 255, 255, 255, flickerAlpha });
			start.draw("START");
			end.draw("EXIT");
			break;
		case GAMEPLAY :
			ClearBackground(Dark_Green);
			DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Green);

			DrawCircle(screenWidth / 2, screenHeight / 2, 150, Light_Green);
			DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

			DrawText(TextFormat("%i", playerScore), screenWidth / 2 - 75, screenHeight / 2 - 20, 60, WHITE);
			DrawText(TextFormat("%i", cpuScore), screenWidth / 2 + 37, screenHeight / 2 - 20, 60, WHITE);


			b.draw();
			p.draw();
			c.draw();
			break;
#pragma endregion	
		}
		EndDrawing();
	}
	CloseWindow();
}
