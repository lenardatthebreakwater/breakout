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
	bool isLaunched;
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
		.speed = 7,
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
		.y = playerRec.y - 15,
		.radius= 10,
		.speedX = 0,
		.speedY = -5,
		.isMoving = false,
		.color = RAYWHITE
	};

	// I inputted each brick's coordinates manually which is bad
	// Gotta figure out how to improve it in the future
	Brick bricks[12] = {
		{ .x = 70, .y = 70, .width = 150, .height = 50, .isHit = false },
		{ .x = 240, .y = 70, .width = 150, .height = 50, .isHit = false },
		{ .x = 410, .y = 70, .width = 150, .height = 50, .isHit = false },
		{ .x = 580, .y = 70, .width = 150, .height = 50, .isHit = false },
		{ .x = 70, .y = 140, .width = 150, .height = 50, .isHit = false },
		{ .x = 240, .y = 140, .width = 150, .height = 50, .isHit = false },
		{ .x = 410, .y = 140, .width = 150, .height = 50, .isHit = false },
		{ .x = 580, .y = 140, .width = 150, .height = 50, .isHit = false },
		{ .x = 70, .y = 210, .width = 150, .height = 50, .isHit = false },
		{ .x = 240, .y = 210, .width = 150, .height = 50, .isHit = false },
		{ .x = 410, .y = 210, .width = 150, .height = 50, .isHit = false },
		{ .x = 580, .y = 210, .width = 150, .height = 50, .isHit = false },
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
		if (CheckCollisionCircleRec((Vector2){ .x = ball.x, .y = ball.y }, ball.radius, playerRec)) {
			// this if guard is here because this code reruns per frame
			// the ball and paddle don't uncollide immediately
			// the code doesn't give a Fook about this and reruns immediately as a new frame starts
			// this allows multiple collision checks to run even if the ball hasn't separated from the paddle yet
			// leading to ball.speedY getting flipped many times
			// the guard is there to make sure ball.speedY only gets flipped once, only when the ball is incoming and its speedY value is still positive 
			if (ball.speedY > 0) {
				ball.speedY *= -1;
			}
			if (ball.speedX == 0) {
				ball.speedX = 5;
			}
		}

		for (size_t i = 0; i <sizeof(bricks) / sizeof(bricks[0]); i++) {
			if (bricks[i].isHit == false && CheckCollisionCircleRec((Vector2){ .x = ball.x, .y = ball.y }, ball.radius, (Rectangle){ .x = bricks[i].x, .y = bricks[i].y, .width = bricks[i].width, .height = bricks[i].height })) {
				if (ball.speedX == 0) {
					ball.speedX = 5;
				}
				bricks[i].isHit = true;
				ball.speedY *= -1;
			}
		}
	
		BeginDrawing();
		ClearBackground(BLACK);
		drawBricks(bricks, sizeof(bricks));
		DrawRectangleRec(playerRec, player.color);
		DrawCircle(ball.x, ball.y, ball.radius, ball.color);
		EndDrawing();

	}

	CloseWindow();
	return 0;
}

void drawBricks(Brick bricks[], size_t bricksSize) {
	for (size_t i = 0; i < bricksSize / sizeof(*bricks); i++) {
		// This can also be written as DrawRectangle(bricks[i]->.x, etc) which is the cleaner and more prefferd way. 
		// I decided to write it the olden way for learning purposes. 
		// Olden way is more explicit and shows you whats happening.
		if ((*(bricks + i)).isHit == false) {
			DrawRectangle((*(bricks + i)).x, (*(bricks + i)).y, (*(bricks + i)).width, (*(bricks + i)).height, RAYWHITE);
		}
	}
}
