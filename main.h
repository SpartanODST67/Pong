#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ctype.h>
#include <Windows.h>

#include "main.h"
#include "block.h"

/// @brief Stores the state of the game.
struct GameStateInfo {
    Vector2 gameBorder;
    Block ball;
    Block leftPaddle;
    Block rightPaddle;
    int player1Score;
    int player2Score;
} typedef GameStateInfo;

/// @brief Windows thread that controls the gameloop.
/// @param lpParam Thread data that is passed during thread creation.
/// @return Completion status of the thread.
DWORD WINAPI gameLoop(LPVOID);

/// @brief Draws the screen of the game.
/// @param gameState State of the game to reflect.
void drawScreen(HDC, GameStateInfo);

/// @brief Draws the scores of the players.
/// @param screen screen to draw on.
/// @param color color of the text.
/// @param player1Score Player 1's score.
/// @param player2Score player 2's score.
/// @param p1Pos of screen to draw player 1's score.
/// @param p2Pos Position of screen to draw player 2's score.
void drawScores(HDC, HBRUSH, int, int, Vector2, Vector2);

/// @brief Draws a score to the screen.
/// @param screen screen to draw on.
/// @param color Color of text.
/// @param score Score to draw.
/// @param pos Position to draw score.
void drawScore(HDC, HBRUSH, int, Vector2);

/// @brief Draws a 0 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberZero(HDC, HBRUSH, Vector2);

/// @brief Draws a 1 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberOne(HDC, HBRUSH, Vector2);

/// @brief Draws a 2 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberTwo(HDC, HBRUSH, Vector2);

/// @brief Draws a 3 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberThree(HDC, HBRUSH, Vector2);

/// @brief Draws a 4 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberFour(HDC, HBRUSH, Vector2);

/// @brief Draws a 5 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberFive(HDC, HBRUSH, Vector2);

/// @brief Draws a 6 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberSix(HDC, HBRUSH, Vector2);

/// @brief Draws a 7 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberSeven(HDC, HBRUSH, Vector2);

/// @brief Draws a 8 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberEight(HDC, HBRUSH, Vector2);

/// @brief Draws a 9 on the screen.
/// @param screen screen to draw on.
/// @param color Color of number.
/// @param pos Position to draw on screen.
void drawNumberNine(HDC, HBRUSH, Vector2);

/// @brief Handles the message that targeted the window. Typically a large switch statement that defines the behaviour of each response.
/// @param hwnd Window Handle. 
/// @param msg Message code to handle.
/// @param wParam Additional data to the message. Int vals of size of pointer width. Look up message to cast to proper data type.
/// @param lParam Additional data to the message. Int vals of size of pointer width. Look up message to cast to proper data type.
/// @return Int val that represents response to message.
//  CALLBACK: calling convention.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/// @brief Obtains the game state.
/// @param hwnd Window Handle.
/// @return the game state.
GameStateInfo* GetAppState(HWND);

/// @brief Reads key user key presses.
/// @param leftPaddle reference to the left paddle.
/// @param rightPaddle reference to the right paddle.
/// @param gameBorder dimensions of the game space.
void inputHandler(Block*, Block*, Vector2);

/// @brief Handles the key input from the user.
/// @param pushedKey Key that was inputted.
/// @param leftPaddle reference to the left paddle.
/// @param rightPaddle reference to the right paddle.
/// @param gameBorder dimensions of the game space.
void handleInput(char, Block*, Block*, Vector2);

#endif