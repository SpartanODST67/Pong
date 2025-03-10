#include <stdio.h>
#include <Windows.h>

#include "block.h"

void drawScreen(HDC, Vector2, Block, Block, Block);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const char windowClassName[] = "PongWindowClass";

/*int main(int argc, char* argv[]) {    
}*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
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
    
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    
    //Registering window class.
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", 
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    //Creating Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        windowClassName,
        "Pong",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, gameBorder.x + 20, gameBorder.y + 45,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creating Failed!", "Error!", 
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HDC screen = GetDC(hwnd);
    if(screen == NULL) {
        fprintf(stderr, "Failed to get screen context.\n");
        return -1;
    }

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        ball.position = moveBall(&ball, leftPaddle, rightPaddle, gameBorder);
        drawScreen(screen, gameBorder, ball, leftPaddle, rightPaddle);
        //Sleep((1.0f/60.0f) * 1000.0f);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void drawScreen(HDC screen, Vector2 dimensions, Block ball, Block leftPaddle, Block rightPaddle) {
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