#include "block.h"

Vector2 moveBall(Block* ball, Block leftPaddle, Block rightPaddle, Vector2 gameBorder) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

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