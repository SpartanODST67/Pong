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

    Vector2 gameBoarder;
    gameBoarder.x = 960;
    gameBoarder.y = 520;

    Block ball;
    ball.dimensions.x = 10;
    ball.dimensions.y = 10;
    ball.position.x = (gameBoarder.x / 2);
    ball.position.y = (gameBoarder.y / 2);

    printf("Game dimensions: %fx%f, Ball at: (%f, %f)", gameBoarder.x, gameBoarder.y, ball.position.x, ball.position.y);
    return 0;
}