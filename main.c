#include <stdio.h>
#include <Windows.h>
#include "block.h"

void drawScreen(HDC, Vector2, Block, Block, Block);

int main(int argc, char* argv[]) {
    Vector2 gameBorder;
    gameBorder.x = 960;
    gameBorder.y = 520;

    Block ball;
    ball.dimensions.x = 10;
    ball.dimensions.y = 10;
    ball.position.x = (gameBorder.x / 2);
    ball.position.y = (gameBorder.y / 2);
    ball.velocity.x = 5;
    ball.velocity.y = 5;

    Block leftPaddle;
    leftPaddle.dimensions.x = 20;
    leftPaddle.dimensions.y = ball.dimensions.y * 4;
    leftPaddle.position.x = 40;
    leftPaddle.position.y = gameBorder.y / 2;

    Block rightPaddle;
    rightPaddle.dimensions.x = 20;
    rightPaddle.dimensions.y = ball.dimensions.y * 4;
    rightPaddle.position.x = gameBorder.x - 40;
    rightPaddle.position.y = gameBorder.y / 2;

    HDC screen = GetDC(NULL);
    if(screen == NULL) {
        fprintf(stderr, "Failed to get screen context.\n");
        return -1;
    }

    for(int i = 0; i < 100; i++) {
        ball.position = moveBall(&ball, leftPaddle, rightPaddle, gameBorder);
        printf("Game dimensions: %fx%f, Ball at: (%f, %f)\n", gameBorder.x, gameBorder.y, ball.position.x, ball.position.y);
        drawScreen(screen, gameBorder, ball, leftPaddle, rightPaddle);
        //Sleep((1.0f/60.0f) * 1000.0f);
    }
    return 0;
}

void drawScreen(HDC screen, Vector2 dimensions, Block ball, Block leftPaddle, Block rightPaddle) {
    //Draw background
    HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
    SelectObject(screen, brush);
    Rectangle(screen, 0, 0, dimensions.x, dimensions.y);

    brush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(screen, brush);
    //Draw Ball
    Rectangle(screen, ball.position.x - (ball.dimensions.x / 2), (ball.position.y + ball.dimensions.y / 2),
                ball.position.x + (ball.dimensions.x / 2), ball.position.y - (ball.dimensions.y / 2));

    //Draw Left Paddle
    Rectangle(screen, leftPaddle.position.x - (leftPaddle.dimensions.x / 2), (leftPaddle.position.y + leftPaddle.dimensions.y / 2),
                leftPaddle.position.x + (leftPaddle.dimensions.x / 2), leftPaddle.position.y - (leftPaddle.dimensions.y / 2));

    //Draw Right Paddle
    Rectangle(screen, rightPaddle.position.x - (rightPaddle.dimensions.x / 2), (rightPaddle.position.y + rightPaddle.dimensions.y / 2),
        rightPaddle.position.x + (rightPaddle.dimensions.x / 2), rightPaddle.position.y - (rightPaddle.dimensions.y / 2));
}