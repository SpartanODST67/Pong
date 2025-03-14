#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
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

/// @brief Handles the key input from the user.
/// @param pushedKey Key that was inputted.
/// @param leftPaddle reference to the left paddle.
/// @param rightPaddle reference to the right paddle.
/// @param gameBorder dimensions of the game space.
void handleInput(char, Block*, Block*, Vector2);

#endif