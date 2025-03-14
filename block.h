#ifndef BLOCK_H
#define BLOCK_H

#include "vector2.h"

#define PLAYER_ONE_UP 'w'
#define PLAYER_TWO_UP 'i'
#define PLAYER_ONE_DOWN 's'
#define PLAYER_TWO_DOWN 'k'

/// @brief Stores the position, dimensions, and velocity of a block.
struct Block {
    Vector2 position;
    Vector2 dimensions;
    Vector2 velocity;
} typedef Block;

/// @brief Creates a block.
/// @param width Width of the block.
/// @param height Height of the block.
/// @param xCord Starting x cordinate of the block.
/// @param yCord Starting y cordinate of the block.
/// @param horVelocity Horizontal velocity of the block.
/// @param vertVelocity Vertical velocity of the block.
/// @return A Block that is initalized with the provided values.
Block createBlock(float, float, float, float, float, float);

/// @brief Creates a ball.
/// @param xCord The starting x cord.
/// @param yCord The starting y cord.
/// @param direction The direction for initial velocity.
/// @return A Block with its values initalized for a ball.
Block createBall(float, float, int);

/// @brief Creates a paddle.
/// @param gameBorder Dimensions of the game.
/// @param xOffset Horizonal offset.
/// @param side Side of the paddle.
/// @return A Block with its values initalized for a paddle.
Block createPaddle(Vector2, float, int);

/// @brief Moves a ball.
/// @param ball Reference to the ball.
/// @param leftPaddle Struct storing the values of the left paddle.
/// @param rightPaddle Struct storing the values of the right paddle.
/// @param gameBorder Struct storing the values of the game border.
/// @return The new position of the ball.
Vector2 moveBall(Block*, Block, Block, Vector2);

/// @brief Checks if the ball collides with the paddle.
/// @param ball The ball.
/// @param paddle Paddle to check collision with.
/// @return 0 if the ball does not collide with the ball, 1 otherwise.
int hitPaddle(Block, Block);

/// @brief Moves a paddle.
/// @param key Key input.
/// @param paddle Paddle to move.
/// @param gameBorder Struct storing the values of the game border.
/// @return new position of the paddle.
Vector2 movePaddle(char, Block, Vector2);

#endif