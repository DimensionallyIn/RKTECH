#include <windows.h>
#include <stdio.h>

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 350
#define CELL_SIZE (WINDOW_WIDTH / 3)

#define ID_RESET   109

HWND g_hWndMain;
char g_board[3][3] = {0};
int g_turn = 1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;


    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "TicTacToe";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "TicTacToe", "Tic Tac Toe", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_hWndMain = hWnd;


    CreateWindow("BUTTON", "Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 300, 100, 30, hWnd, (HMENU)ID_RESET, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void DrawBoard(HWND hWnd, HDC hdc) {

    for (int i = 1; i < 3; ++i) {
        MoveToEx(hdc, i * CELL_SIZE, 0, NULL);
        LineTo(hdc, i * CELL_SIZE, WINDOW_HEIGHT);
        MoveToEx(hdc, 0, i * CELL_SIZE, NULL);
        LineTo(hdc, WINDOW_WIDTH, i * CELL_SIZE);
    }


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (g_board[i][j] == 1) { // X
                MoveToEx(hdc, j * CELL_SIZE, i * CELL_SIZE, NULL);
                LineTo(hdc, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
                MoveToEx(hdc, j * CELL_SIZE, (i + 1) * CELL_SIZE, NULL);
                LineTo(hdc, (j + 1) * CELL_SIZE, i * CELL_SIZE);
            } else if (g_board[i][j] == 2) { // O
                Ellipse(hdc, j * CELL_SIZE, i * CELL_SIZE, (j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);
            }
        }
    }
}

char CheckWinner() {

    for (int i = 0; i < 3; ++i) {
        if (g_board[i][0] != 0 && g_board[i][0] == g_board[i][1] && g_board[i][0] == g_board[i][2]) {
            return g_board[i][0];
        }
    }


    for (int i = 0; i < 3; ++i) {
        if (g_board[0][i] != 0 && g_board[0][i] == g_board[1][i] && g_board[0][i] == g_board[2][i]) {
            return g_board[0][i];
        }
    }


    if (g_board[0][0] != 0 && g_board[0][0] == g_board[1][1] && g_board[0][0] == g_board[2][2]) {
        return g_board[0][0];
    } else if (g_board[0][2] != 0 && g_board[0][2] == g_board[1][1] && g_board[0][2] == g_board[2][0]) {
        return g_board[0][2];
    }

    return 0;
}

void MakeMove(int row, int col) {
    if (g_board[row][col] == 0) {
        g_board[row][col] = g_turn;
        g_turn = (g_turn == 1) ? 2 : 1;
        InvalidateRect(g_hWndMain, NULL, TRUE);

        char winner = CheckWinner();
        if (winner != 0) {
            char message[50];
            sprintf(message, "Player %c wins!", (winner == 1) ? 'X' : 'O');
            MessageBox(g_hWndMain, message, "Game Over", MB_OK);
            SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawBoard(hWnd, hdc);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            switch (id) {
                case ID_RESET: {

                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            g_board[i][j] = 0;
                        }
                    }
                    g_turn = 1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                }
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            int row = yPos / CELL_SIZE;
            int col = xPos / CELL_SIZE;
            if (row < 3 && col < 3 && g_board[row][col] == 0) {
                MakeMove(row, col);
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

