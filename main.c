#include "main.h"

const char windowClassName[] = "PongWindowClass";
HWND hwnd;

/// @brief Entry point of Windows Program.
/// @param hInstance Handle to an instance. Used to ID exe in mem.
/// @param hPrevInstance Unused. Relic of 16-bit Windows. Always 0.
/// @param lpCmdLine Command line args.
/// @param nCmdShow Dictates if window is minimized, maximized, or shown normally.
/// @return int (unused by OS but can be used by other processes).
//WINAPI - Calling Convention - defines the order that parameters appear on the stack.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HANDLE heap = GetProcessHeap();

    GameStateInfo* gameState = HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(GameStateInfo)); //Allocate Memory
    gameState->gameBorder.x = 960;
    gameState->gameBorder.y = 520;

    gameState->ball = createBall(gameState->gameBorder.x / 2, gameState->gameBorder.y / 2, 0);
    gameState->leftPaddle = createPaddle(gameState->gameBorder, 40, -1);
    gameState->rightPaddle = createPaddle(gameState->gameBorder, 40, 1);

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
        gameState->gameBorder.x + 20, // Width
        gameState->gameBorder.y + 45, // Height
        
        NULL,       // Parent Window (Null for top level windows)
        NULL,       // Menu
        hInstance,  // Instance Handle
        gameState   // Additional application data (void*) for wind proc.
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
        gameState, // Argument to thread func
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
    HeapFree(heap, 0, gameState);

    return msg.wParam;
}
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    GameStateInfo* gameState;
    if(msg == WM_CREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        gameState = (GameStateInfo*) pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)gameState);    
    }
    else {
        gameState = GetAppState(hwnd);
    }

    switch(msg) {
        
        case WM_CLOSE: // When user clicks the X or CTRL + F4.
            //Default wind proc behaviour already does this.
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY: // When window is no longer visible but is about to be destroyed (before any child windows are destroyed).
            PostQuitMessage(0); //Used to break out of message loop.
            break;
        case WM_PAINT: //When a portion of the window needs to be repainted.
        {
            PAINTSTRUCT ps;
            //Fills ps with info on repaint request.
            HDC screen = BeginPaint(hwnd, &ps); 

            //All painting here.
            drawScreen(screen, *gameState);

            //Clears update region.
            EndPaint(hwnd, &ps);
        }
        break;
        
        default:
        return DefWindowProc(hwnd, msg, wParam, lParam); // Performs default action to message.
    }

    return 0;
}

inline GameStateInfo* GetAppState(HWND hwnd) {
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    GameStateInfo* pState = (GameStateInfo*) ptr;
    return pState;
}

DWORD WINAPI gameLoop(LPVOID lpParam) {
    GameStateInfo* gameState = (GameStateInfo*)lpParam;
    int scored = 0;
    int aIActive = 1;

    while(gameState->player1Score < 10 && gameState->player2Score < 10) {
        aIActive = inputHandler(&(gameState->leftPaddle), &(gameState->rightPaddle), gameState->gameBorder, aIActive);
        
        if(aIActive == 1) aIMove(gameState);

        gameState->ball.position = moveBall(&(gameState->ball), gameState->leftPaddle, gameState->rightPaddle, gameState->gameBorder);
    
        if(gameState->ball.position.x - (gameState->ball.dimensions.x / 2) <= 0) {
            gameState->player2Score++;
            scored = 2;
        }
        else if(gameState->ball.position.x + (gameState->ball.dimensions.x / 2) >= gameState->gameBorder.x) {
            gameState->player1Score++;
            scored = 1;
        }

        if(scored != 0) {
            if(scored == 1) gameState->ball = createBall(gameState->gameBorder.x / 2, gameState->gameBorder.y / 2, -1);
            else gameState->ball = createBall(gameState->gameBorder.x / 2, gameState->gameBorder.y / 2, 1);
            scored = 0;
        }
        
        InvalidateRect(hwnd, NULL, TRUE); //Invalidates entire window.
        UpdateWindow(hwnd); //Forces repaint.
        Sleep((1.0f/60.0f) * 1000.0f);
    }

    char* victoryText;
    if(gameState->player1Score > gameState->player2Score) victoryText = "Player 1 Wins!";
    else victoryText = "Player 2 Wins!";

    while(MessageBox(hwnd, victoryText, "Game Over!", MB_ICONEXCLAMATION | MB_OK) != IDOK);
    PostMessage(hwnd, WM_CLOSE, 0, 0);

    return 0;
}

int inputHandler(Block* leftPaddle, Block* rightPaddle, Vector2 gameBorder, int aIActive) {
    const char p1UpConst = toupper(PLAYER_ONE_UP);
    const char p1DownConst = toupper(PLAYER_ONE_DOWN);
    const char p2UpConst = toupper(PLAYER_TWO_UP);
    const char p2DownConst = toupper(PLAYER_TWO_DOWN);

    if((GetAsyncKeyState(p1UpConst) & 0x8000) != 0) {
        handleInput(PLAYER_ONE_UP, leftPaddle, rightPaddle, gameBorder);
    }
    if((GetAsyncKeyState(p1DownConst) & 0x8000) != 0) {
        handleInput(PLAYER_ONE_DOWN, leftPaddle, rightPaddle, gameBorder);
    }
    if((GetAsyncKeyState(p2UpConst) & 0x8000) != 0) {
        if(aIActive == 1) aIActive = 0;
        handleInput(PLAYER_TWO_UP, leftPaddle, rightPaddle, gameBorder);
    }
    if((GetAsyncKeyState(p2DownConst) & 0x8000) != 0) {
        if(aIActive == 1) aIActive = 0;
        handleInput(PLAYER_TWO_DOWN, leftPaddle, rightPaddle, gameBorder);
    }

    return aIActive;
}

void handleInput(char pushedKey, Block* leftPaddle, Block* rightPaddle, Vector2 gameBorder) {
    if(pushedKey == PLAYER_ONE_UP || pushedKey == PLAYER_ONE_DOWN) leftPaddle->position = movePaddle(pushedKey, *leftPaddle, gameBorder);
    else if(pushedKey == PLAYER_TWO_UP || pushedKey == PLAYER_TWO_DOWN) rightPaddle->position = movePaddle(pushedKey, *rightPaddle, gameBorder);
}

void aIMove(GameStateInfo* gameStateInfo) {
    static LONG activationDelay = 0; // So the second player has a chance to input key before AI kicks in.
    if(activationDelay++ < 60) return;
    
    static LONG calls = 0;
    int moveDecision = 0;

    if(calls % 2 == 0) {
        if(gameStateInfo->ball.velocity.y < 0 &&
            gameStateInfo->ball.position.y < gameStateInfo->rightPaddle.position.y - gameStateInfo->rightPaddle.dimensions.y / 2) moveDecision = 1;
        else if(gameStateInfo->ball.velocity.y > 0 &&
            gameStateInfo->ball.position.y > gameStateInfo->rightPaddle.position.y + gameStateInfo->rightPaddle.dimensions.y / 2) moveDecision = -1;
        else moveDecision = 0;
    }

    switch(moveDecision) {
        case 1:
            handleInput(PLAYER_TWO_UP, &(gameStateInfo->leftPaddle), &(gameStateInfo->rightPaddle), gameStateInfo->gameBorder);
            break;
        case -1:
            handleInput(PLAYER_TWO_DOWN, &(gameStateInfo->leftPaddle), &(gameStateInfo->rightPaddle), gameStateInfo->gameBorder);
            break;
    }

    calls++;
}

void drawScreen(HDC screen, GameStateInfo gameState) {
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(screen, brush);

    //Draw Ball
    Rectangle(screen, gameState.ball.position.x - (gameState.ball.dimensions.x / 2), (gameState.ball.position.y + gameState.ball.dimensions.y / 2),
                gameState.ball.position.x + (gameState.ball.dimensions.x / 2), gameState.ball.position.y - (gameState.ball.dimensions.y / 2));

    //Draw Left Paddle
    Rectangle(screen, gameState.leftPaddle.position.x - (gameState.leftPaddle.dimensions.x / 2), 
                gameState.leftPaddle.position.y + (gameState.leftPaddle.dimensions.y / 2),
                gameState.leftPaddle.position.x + (gameState.leftPaddle.dimensions.x / 2), 
                gameState.leftPaddle.position.y - (gameState.leftPaddle.dimensions.y / 2));

    //Draw Right Paddle
    Rectangle(screen, gameState.rightPaddle.position.x - (gameState.rightPaddle.dimensions.x / 2), 
                gameState.rightPaddle.position.y + (gameState.rightPaddle.dimensions.y / 2),
                gameState.rightPaddle.position.x + (gameState.rightPaddle.dimensions.x / 2), 
                gameState.rightPaddle.position.y - (gameState.rightPaddle.dimensions.y / 2));

    Vector2 p1ScorePos = gameState.gameBorder;
    p1ScorePos.x = p1ScorePos.x / 4;
    p1ScorePos.y = p1ScorePos.y / 6;

    Vector2 p2ScorePos = gameState.gameBorder;
    p2ScorePos.x = (p2ScorePos.x * 3.0f / 4.0f);
    p2ScorePos.y = p2ScorePos.y / 6;

    drawScores(screen, brush, gameState.player1Score, gameState.player2Score, p1ScorePos, p2ScorePos);
}

void drawScores(HDC screen, HBRUSH color, int player1Score, int player2Score, Vector2 p1Pos, Vector2 p2Pos) {
    drawScore(screen, color, player1Score, p1Pos);
    drawScore(screen, color, player2Score, p2Pos);
}

void drawScore(HDC screen, HBRUSH color, int score, Vector2 pos) {
    switch(score) {
        case 1:
            drawNumberOne(screen, color, pos);
            break;
        case 2:
            drawNumberTwo(screen, color, pos);
            break;
        case 3:
            drawNumberThree(screen, color, pos);
            break;
        case 4:
            drawNumberFour(screen, color, pos);
            break;
        case 5:
            drawNumberFive(screen, color, pos);
            break;
        case 6:
            drawNumberSix(screen, color, pos);
            break;
        case 7:
            drawNumberSeven(screen, color, pos);
            break;
        case 8:
            drawNumberEight(screen, color, pos);
            break;
        case 9:
            drawNumberNine(screen, color, pos);
            break;
        default:
            drawNumberZero(screen, color, pos);
            break;
    }
} 

void drawNumberZero(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y + 5, pos.x - 20, pos.y + 30); //Bottom left
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberOne(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x - 30, pos.y + 5, pos.x - 20, pos.y + 30); //Bottom left
}

void drawNumberTwo(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x - 30, pos.y + 5, pos.x - 20, pos.y + 30); //Bottom left
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberThree(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberFour(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
}

void drawNumberFive(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberSix(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x - 30, pos.y + 5, pos.x - 20, pos.y + 30); //Bottom left
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberSeven(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
}

void drawNumberEight(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x - 30, pos.y + 5, pos.x - 20, pos.y + 30); //Bottom left
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
    Rectangle(screen, pos.x - 30, pos.y + 30, pos.x + 30, pos.y + 40); //Bottom
}

void drawNumberNine(HDC screen, HBRUSH color, Vector2 pos) {
    Rectangle(screen, pos.x - 30, pos.y - 40, pos.x + 30, pos.y - 30); //Top
    Rectangle(screen, pos.x - 30, pos.y - 30, pos.x - 20, pos.y - 5); //Top left
    Rectangle(screen, pos.x + 20, pos.y - 30, pos.x + 30, pos.y - 5); //Top right
    Rectangle(screen, pos.x - 30, pos.y - 5, pos.x + 30, pos.y + 5); //Middle
    Rectangle(screen, pos.x + 20, pos.y + 5, pos.x + 30, pos.y + 30); //Bottom right
}