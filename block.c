#include <stdlib.h>
#include "block.h"

Block createBlock(float width, float height, float xCord, float yCord, float horVelocity, float vertVelocity) {
    Block res;
    res.dimensions.x = width;
    res.dimensions.y = height;
    res.position.x = xCord;
    res.position.y = yCord;
    res.velocity.x = horVelocity;
    res.velocity.y = vertVelocity;

    return res;
}

Block createBall(float xCord, float yCord, int direction) {
    Block ball = createBlock(10, 10, xCord, yCord, 0, 0);

    ball.velocity.x = 5;
    if(direction == 0) { //Random direction
        if(rand() % 2 == 0) {
            ball.velocity.x = -ball.velocity.x;
        }
    }
    else if(direction < 0) {
        ball.velocity.x = -ball.velocity.x;
    }

    ball.velocity.y = 5;
    if(rand() % 2 == 0) {
        ball.velocity.y = -ball.velocity.y;
    }

    return ball;
}

Vector2 moveBall(Block* ball, Block leftPaddle, Block rightPaddle, Vector2 gameBorder) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

    if(hitPaddle(*ball, leftPaddle)) { //If ball collides with the left paddle.
        ball->velocity.x = -ball->velocity.x;
        ball->position.x = leftPaddle.position.x + (leftPaddle.dimensions.x / 2) + (ball->dimensions.x / 2);
    }
    if(hitPaddle(*ball, rightPaddle)) { //If ball collides with the right paddle.
        ball->velocity.x = -ball->velocity.x;
        ball->position.x = rightPaddle.position.x - (rightPaddle.dimensions.x / 2) - (ball->dimensions.x / 2);
    }

    if(ball->position.x - (ball->dimensions.x / 2) <= 0) { //If ball collides with the left wall.
        ball->position.x = ball->dimensions.x / 2;
        ball->velocity.x = -1 * ball->velocity.x;
    }
    else if(ball->position.x + (ball->dimensions.x / 2) >= gameBorder.x) { //If ball collides with the right wall.
        ball->position.x = gameBorder.x - (ball->dimensions.x / 2);
        ball->velocity.x = -1 * ball->velocity.x;
    }

    if(ball->position.y - (ball->dimensions.y / 2) <= 0) { //If ball collides with the bottom wall.
        ball->position.y = ball->dimensions.y / 2;
        ball->velocity.y = -1 * ball->velocity.y;
    }
    else if(ball->position.y + (ball->dimensions.y / 2) >= gameBorder.y) { // if ball collides with the top wall.
        ball->position.y = gameBorder.y - (ball->dimensions.y / 2);
        ball->velocity.y = -1 * ball->velocity.y;
    }

    return ball->position;
}

int hitPaddle(Block ball, Block paddle) {
    //If right side of the ball not in bounds of the left side of the paddle
    if(ball.position.x + (ball.dimensions.x / 2) < paddle.position.x - (paddle.dimensions.x / 2)) return 0;

    //If left side of the ball is not in bounds of the right side of the paddle.
    if(ball.position.x - (ball.dimensions.x / 2) > paddle.position.x + (paddle.dimensions.x / 2)) return 0;

    //If top of the ball is not in bounds of the top of the paddle.
    if(ball.position.y + (ball.dimensions.y / 2) > paddle.position.y + (paddle.dimensions.y / 2)) return 0;

    //If bottom of the ball is not in bounds of the bottom of the paddle.
    if(ball.position.y - (ball.dimensions.y / 2) < paddle.position.y - (paddle.dimensions.y / 2)) return 0;

    //The ball is in bounds of the paddle.
    return 1;
}

Vector2 movePaddle(char key, Block paddle, Vector2 gameBorders) {
    if(key == 'w' || key == 'i') {
        paddle.position.y -= paddle.velocity.y;
    }
    else if(key == 's' || key == 'k') {
        paddle.position.y += paddle.velocity.y;
    }

    if(paddle.position.y - (paddle.dimensions.y / 2) < 0) {
        paddle.position.y = paddle.dimensions.y / 2;
    }
    else if(paddle.position.y + (paddle.dimensions.y / 2) > gameBorders.y) {
        paddle.position.y = gameBorders.y - (paddle.dimensions.y / 2);
    }

    return paddle.position;
}