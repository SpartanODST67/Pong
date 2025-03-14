#include "vector2.h"

#define PLAYER_ONE_UP 'w'
#define PLAYER_TWO_UP 'i'
#define PLAYER_ONE_DOWN 's'
#define PLAYER_TWO_DOWN 'k'

struct Block {
    Vector2 position;
    Vector2 dimensions;
    Vector2 velocity;
} typedef Block;

Block createBlock(float, float, float, float, float, float);
Block createBall(float, float, int);
Block createPaddle(Vector2, float, int);

Vector2 moveBall(Block*, Block, Block, Vector2);
int hitPaddle(Block, Block);
Vector2 movePaddle(char, Block, Vector2);