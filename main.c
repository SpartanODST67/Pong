#include <stdio.h>

struct Vector2 {
    float x;
    float y;
} typedef Vector2;

int main(int argc, char* argv[]) {

    Vector2 gameBoarder;
    gameBoarder.x = 960;
    gameBoarder.y = 520;

    printf("%f %f", gameBoarder.x, gameBoarder.y);
    return 0;
}