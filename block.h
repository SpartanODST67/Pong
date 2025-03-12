#include "vector2.h"

struct Block {
    Vector2 position;
    Vector2 dimensions;
    Vector2 velocity;
} typedef Block;

Vector2 moveBall(Block*, Block, Block, Vector2);
int hitPaddle(Block, Block);