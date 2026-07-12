#include <string.h>
#include "raylib.h"

typedef struct Player {
	int x; 
	int y;
	int speed;
	int width;
	int height;
	Color color;
} Player;

typedef struct Ball {
	int x;
	int y;
	float radius;
	int speedX;
	int speedY;
	bool isMoving;
	Color color;
} Ball;

typedef struct Brick {
	int x;
	int y;
	int width;
	int height;
	bool isHit;
} Brick;

void drawBricks(Brick bricks[], size_t bricksSize);

int main(void) {
	int screenWidth = 800;
	int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Breakout");
	SetTargetFPS(60);

	Player player = {
		.x = ((screenWidth / 2) - ( 150 / 2)),
		.y = (screenHeight - (14 + 10)),
		.speed = 6,
		.width = 150,
		.height = 14,
		.color = RAYWHITE
	};

	Rectangle playerRec = {
		.x = player.x,
		.y = player.y,
		.width = player.width,
		.height = player.height
	};

	Ball ball = {
		.x = (playerRec.x + (playerRec.width / 2)),
		.y = playerRec.y,
		.radius= 10,
		.speedX = 5,
		.speedY = 5,
		.isMoving = false,
		.color = GREEN
	};

	Brick rowOfBricks[1] = {
		{ .x = 100, .y = 100, .width = 50, .height = 50, .isHit = false },
	};

	char winnerText[30] = { 0 };

	while (!WindowShouldClose()) {

		if (IsKeyDown(KEY_RIGHT)) {
			playerRec.x += player.speed;
			if (ball.isMoving == false) {
				ball.x += player.speed;
			}
		}
		else if (IsKeyDown(KEY_LEFT)){
			playerRec.x -= player.speed;
			if (ball.isMoving == false) {
				ball.x -= player.speed;
			}
		}
		if (playerRec.x < 0) {
			playerRec.x = 0;
			if (ball.isMoving == false) {
				ball.x = (playerRec.x + (playerRec.width / 2));
			}
		}
		if (playerRec.x > (screenWidth - playerRec.width)) {
			playerRec.x = (screenWidth - playerRec.width);
			if (ball.isMoving == false) {
				ball.x = (playerRec.x + (playerRec.width / 2));
			}
		}

		if (ball.isMoving == true) {
			ball.x += ball.speedX;
			ball.y += ball.speedY;
		}

		if (IsKeyPressed(KEY_SPACE)) {
			ball.isMoving = true;
		}

		// if the ball hits left
		if (ball.x < 0) {
			ball.x = 0;
			ball.speedX *= -1;
		}

		// if the ball hits right
		if (ball.x > (screenWidth - ball.radius)) {
			ball.x = (screenWidth - ball.radius);
			ball.speedX *= -1;
		}

		// if the ball hits the top
		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}

		// if the ball hits the bottom
		if (ball.y > (screenHeight - ball.radius)) {
			memcpy(winnerText, "You lose", 9);
		}
		
		// if the ball hits the paddle
		if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, playerRec)) {
			if (ball.speedY > 0) {
				ball.speedY *= -1;
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);
		drawBricks(rowOfBricks, sizeof(rowOfBricks));
		DrawRectangleRec(playerRec, player.color);
		DrawCircle(ball.x, ball.y, ball.radius, ball.color);
		EndDrawing();

	}

	CloseWindow();
	return 0;
}

void drawBricks(Brick bricks[], size_t bricksSize) {
	for (int i = 0; i < (int)(bricksSize / sizeof(*bricks)); i++) {
		// This can also be written as DrawRectangle(bricks[i]->.x, etc) which is the cleaner and more prefferd way. 
		// Old me decided to write it the olden way for learning purposes. 
		// Olden way is more explicit and shows you whats happening.
		DrawRectangle((*(bricks + i)).x, (*(bricks + i)).y, (*(bricks + i)).width, (*(bricks + i)).height, RAYWHITE);
	}
}