#include <stdio.h>
#include "block.h"

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

    for(int i = 0; i < 100; i++) {
        ball.position = moveBall(&ball, leftPaddle, rightPaddle, gameBorder);
        printf("Game dimensions: %fx%f, Ball at: (%f, %f)\n", gameBorder.x, gameBorder.y, ball.position.x, ball.position.y);
    }
    return 0;
}