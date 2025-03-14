#include <stdio.h>
#include <Windows.h>

#include "block.h"

DWORD WINAPI gameLoop(LPVOID);
void drawScreen(HDC, Block, Block, Block);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void handleInput(char);

const char windowClassName[] = "PongWindowClass";
Vector2 gameBorder;
Block ball;
Block leftPaddle;
Block rightPaddle;
HWND hwnd;

/// @brief Entry point of Windows Program.
/// @param hInstance Handle to an instance. Used to ID exe in mem.
/// @param hPrevInstance Unused. Relic of 16-bit Windows. Always 0.
/// @param lpCmdLine Command line args.
/// @param nCmdShow Dictates if window is minimized, maximized, or shown normally.
/// @return int (unused by OS but can be used by other processes).
//WINAPI - Calling Convention - defines the order that parameters appear on the stack.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    gameBorder.x = 960;
    gameBorder.y = 520;

    ball = createBall(gameBorder.x / 2, gameBorder.y / 2, 0);
    leftPaddle = createPaddle(gameBorder, 40, -1);
    rightPaddle = createPaddle(gameBorder, 40, 1);

    WNDCLASSEX wc;
    MSG msg;
    
    //Registering window class.
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc; // The window procedure.
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance; // Handle to application instance.
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowClassName; // String to ID window class.
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //Registers window class with OS.
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", 
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Creating Window
    hwnd = CreateWindowEx(
        0,                   // Optional window styles
        windowClassName,     // Window class (Identical to the one used for registration)
        "Pong",              // Window Text (Used in different ways depending on style. Name presented on the title bar.)
        WS_OVERLAPPEDWINDOW, // Window Style (WS_OVERLAPPEDWINDOW is a constant that is an bitwise OR of multiple styles).

        //Size and position
        CW_USEDEFAULT,     
        CW_USEDEFAULT,     
        gameBorder.x + 20, // Width
        gameBorder.y + 45, // Height
        
        NULL,       // Parent Window (Null for top level windows)
        NULL,       // Menu
        hInstance,  // Instance Handle
        NULL        // Additional application data (void*) for wind proc.
    );

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", 
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    //Create gameloop thread.
    DWORD gameLoopThreadID;
    HANDLE gameLoopThread = CreateThread(
        NULL, // Security attributes (Default)
        0, // Stack size (Default)
        gameLoop, // Thread function
        NULL, // Argument to thread func
        0, // Creation flags (default)
        &gameLoopThreadID // Returns Thread ID.
    );

    //Message loop.
    /*
        GetMessage gets the first message from the message queue.
        Params:
            msg = address of MSG struct to place message info into.
            Other params allows filtering of messages and are commonly set to NULL and 0.

        Posting a message enqueues a message.
        Sending a message skips the queue.
    */
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg); //Translates keystrokes to chars. Remeber to call it before DispatchMessage();
        DispatchMessage(&msg); // Tells OS to call wind proc.
    }

    CloseHandle(gameLoopThread); // Close thread.

    return msg.wParam;
}

/// @brief Handles the message that targeted the window. Typically a large switch statement that defines the behaviour of each response.
/// @param hwnd Window Handle. 
/// @param msg Message code to handle.
/// @param wParam Additional data to the message. Int vals of size of pointer width. Look up message to cast to proper data type.
/// @param lParam Additional data to the message. Int vals of size of pointer width. Look up message to cast to proper data type.
/// @return Int val that represents response to message.
//  CALLBACK: calling convention. 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE: // When user clicks the X or CTRL + F4.
            //Default wind proc behaviour already does this.
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY: // When window is no longer visible but is about to be destroyed (before any child windows are destroyed).
            PostQuitMessage(0); //Used to break out of message loop.
            break;
        case WM_CHAR:
            handleInput((char) wParam);
            break;
        case WM_PAINT: //When a portion of the window needs to be repainted.
        {
            PAINTSTRUCT ps;
            //Fills ps with info on repaint request.
            HDC screen = BeginPaint(hwnd, &ps); 

            //All painting here.
            drawScreen(screen, ball, leftPaddle, rightPaddle);

            //Clears update region.
            EndPaint(hwnd, &ps);
        }
        break;
        
        default:
        return DefWindowProc(hwnd, msg, wParam, lParam); // Performs default action to message.
    }

    return 0;
}

/// @brief Windows thread that controls the gameloop.
/// @param lpParam Thread data that is passed during thread creation.
/// @return Completion status of the thread.
DWORD WINAPI gameLoop(LPVOID lpParam) {
    int player1Score = 0;
    int player2Score = 0;
    int scored = 0;
    
    while(player1Score < 10 && player2Score < 10) {
        ball.position = moveBall(&ball, leftPaddle, rightPaddle, gameBorder);
    
        if(ball.position.x - (ball.dimensions.x / 2) <= 0) {
            player2Score++;
            scored = 2;
        }
        else if(ball.position.x + (ball.dimensions.x / 2) >= gameBorder.x) {
            player1Score++;
            scored = 1;
        }

        if(scored != 0) {
            if(scored == 1) ball = createBall(gameBorder.x / 2, gameBorder.y / 2, -1);
            else ball = createBall(gameBorder.x / 2, gameBorder.y / 2, 1);
            scored = 0;
        }
        
        InvalidateRect(hwnd, NULL, TRUE); //Invalidates entire window.
        UpdateWindow(hwnd); //Forces repaint.
        Sleep((1.0f/60.0f) * 1000.0f);
    }

    char* victoryText;
    if(player1Score > player2Score) victoryText = "Player 1 Wins!";
    else victoryText = "Player 2 Wins!";

    while(MessageBox(hwnd, victoryText, "Game Over!", MB_ICONEXCLAMATION | MB_OK) != IDOK);
    PostMessage(hwnd, WM_CLOSE, 0, 0);

    return 0;
}

void handleInput(char pushedKey) {
    if(pushedKey == PLAYER_ONE_UP || pushedKey == PLAYER_ONE_DOWN) leftPaddle.position = movePaddle(pushedKey, leftPaddle, gameBorder);
    else if(pushedKey == PLAYER_TWO_UP || pushedKey == PLAYER_TWO_DOWN) rightPaddle.position = movePaddle(pushedKey, rightPaddle, gameBorder);
}

/// @brief Draws the screen of the game.
/// @param screen Canvas to draw on.
/// @param ball Struct that stores the ball's information.
/// @param leftPaddle Struct that stores the left paddle's information.
/// @param rightPaddle Struct that stores the right paddle's information.
void drawScreen(HDC screen, Block ball, Block leftPaddle, Block rightPaddle) {
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(screen, brush);

    //Draw Ball
    Rectangle(screen, ball.position.x - (ball.dimensions.x / 2), (ball.position.y + ball.dimensions.y / 2),
                ball.position.x + (ball.dimensions.x / 2), ball.position.y - (ball.dimensions.y / 2));

    //Draw Left Paddle
    Rectangle(screen, leftPaddle.position.x - (leftPaddle.dimensions.x / 2), (leftPaddle.position.y + leftPaddle.dimensions.y / 2),
                leftPaddle.position.x + (leftPaddle.dimensions.x / 2), leftPaddle.position.y - (leftPaddle.dimensions.y / 2));

    //Draw Right Paddle
    Rectangle(screen, rightPaddle.position.x - (rightPaddle.dimensions.x / 2), (rightPaddle.position.y + rightPaddle.dimensions.y / 2),
        rightPaddle.position.x + (rightPaddle.dimensions.x / 2), rightPaddle.position.y - (rightPaddle.dimensions.y / 2));
}