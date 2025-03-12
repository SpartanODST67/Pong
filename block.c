#include "block.h"

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

Vector2 movePaddle(char key, Block paddle) {
    paddle.position.y += paddle.velocity.y;
    return paddle.position;
}