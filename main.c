#include <stdio.h>

struct Vector2 {
    float x;
    float y;
} typedef Vector2;

struct Block {
    Vector2 position;
    Vector2 dimensions;
    Vector2 velocity;
} typedef Block;

Vector2 moveBall(Block*, Block, Block, Vector2);

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

Vector2 moveBall(Block* ball, Block leftPaddle, Block rightPaddle, Vector2 gameBorder) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

    if(ball->position.x <= 0) {
        ball->position.x = 0;
        ball->velocity.x = -1 * ball->velocity.x;
    }
    else if(ball->position.x >= gameBorder.x) {
        ball->position.x = gameBorder.x;
        ball->velocity.x = -1 * ball->velocity.x;
    }

    if(ball->position.y <= 0) {
        ball->position.y = 0;
        ball->velocity.y = -1 * ball->velocity.y;
    }
    else if(ball->position.y >= gameBorder.y) {
        ball->position.y = gameBorder.y;
        ball->velocity.y = -1 * ball->velocity.y;
    }

    return ball->position;
}