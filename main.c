#include <stdio.h>

struct Vector2 {
    float x;
    float y;
} typedef Vector2;

struct Block {
    Vector2 position;
    Vector2 dimensions;
} typedef Block;

int main(int argc, char* argv[]) {

    Vector2 gameBorder;
    gameBorder.x = 960;
    gameBorder.y = 520;

    Block ball;
    ball.dimensions.x = 10;
    ball.dimensions.y = 10;
    ball.position.x = (gameBorder.x / 2);
    ball.position.y = (gameBorder.y / 2);

    printf("Game dimensions: %fx%f, Ball at: (%f, %f)", gameBoarder.x, gameBoarder.y, ball.position.x, ball.position.y);
    return 0;
}