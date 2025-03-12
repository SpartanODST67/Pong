#include <stdlib.h>
#include "block.h"

/// @brief Creates a block.
/// @param width Width of the block.
/// @param height Height of the block.
/// @param xCord Starting x cordinate of the block.
/// @param yCord Starting y cordinate of the block.
/// @param horVelocity Horizontal velocity of the block.
/// @param vertVelocity Vertical velocity of the block.
/// @return A Block that is initalized with the provided values.
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

/// @brief Creates a ball.
/// @param xCord The starting x cord.
/// @param yCord The starting y cord.
/// @param direction The direction for initial velocity.
/// @return A Block with its values initalized for a ball.
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

/// @brief Creates a paddle.
/// @param gameBorder Dimensions of the game.
/// @param xOffset Horizonal offset.
/// @param side Side of the paddle.
/// @return A Block with its values initalized for a paddle.
Block createPaddle(Vector2 gameBorder, float xOffset, int side) {
    float xCord;
    if(side < 0) {
        xCord = xOffset; 
    }
    else {
        xCord = gameBorder.x - xOffset;
    }
    
    Block paddle = createBlock(20, 100, xCord, gameBorder.y / 2, 0, 10);

    return paddle;
}

/// @brief Moves a ball.
/// @param ball Reference to the ball.
/// @param leftPaddle Struct storing the values of the left paddle.
/// @param rightPaddle Struct storing the values of the right paddle.
/// @param gameBorder Struct storing the values of the game border.
/// @return The new position of the ball.
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

/// @brief Checks if the ball collides with the paddle.
/// @param ball The ball.
/// @param paddle Paddle to check collision with.
/// @return 0 if the ball does not collide with the ball, 1 otherwise.
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

/// @brief Moves a paddle.
/// @param key Key input.
/// @param paddle Paddle to move.
/// @param gameBorder Struct storing the values of the game border.
/// @return new position of the paddle.
Vector2 movePaddle(char key, Block paddle, Vector2 gameBorder) {
    if(key == 'w' || key == 'i') {
        paddle.position.y -= paddle.velocity.y;
    }
    else if(key == 's' || key == 'k') {
        paddle.position.y += paddle.velocity.y;
    }

    if(paddle.position.y - (paddle.dimensions.y / 2) < 0) {
        paddle.position.y = paddle.dimensions.y / 2;
    }
    else if(paddle.position.y + (paddle.dimensions.y / 2) > gameBorder.y) {
        paddle.position.y = gameBorder.y - (paddle.dimensions.y / 2);
    }

    return paddle.position;
}