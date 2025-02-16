// l1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include <cstring>
#include "l1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_L1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_L1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_L1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_L1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

struct Figure {
    POINT* points;
    int count;
};

struct Point {
    double x, y, w = 1.0;
};

int indexTransformMatrix = 0;

Point transform(const Point& p, const double Matrix[3][3]) {
    return {
        p.x * Matrix[0][0] + p.y * Matrix[1][0] + p.w * Matrix[2][0],
        p.x * Matrix[0][1] + p.y * Matrix[1][1] + p.w * Matrix[2][1],
        p.x * Matrix[0][2] + p.y * Matrix[1][2] + p.w * Matrix[2][2]
    };
}

void DrawFiguresTrasform(HDC hdc, Figure figures[], int figureCount, double Matrix[3][3]) {
    for (int i = 0; i < figureCount; i++) {
        if (figures[i].count < 2) continue;

        Point p = transform({(double)figures[i].points[0].x, (double)figures[i].points[0].y }, Matrix);
        MoveToEx(hdc, (int)p.x, (int)p.y, NULL);

        for (int j = 1; j < figures[i].count; j++) {
            p = transform({(double)figures[i].points[j].x, (double)figures[i].points[j].y }, Matrix);
            LineTo(hdc, (int)p.x, (int)p.y);
        }
    }
}

RECT FindingLength(Figure figures[], int figureCount) {
    int minX = figures[0].points[0].x;
    int maxX = figures[0].points[0].x;
    int minY = figures[0].points[0].y;
    int maxY = figures[0].points[0].y;
    for (int i = 0; i < figureCount; i++) {
        if (figureCount < 2) continue;

        for (int j = 0; j < figures[i].count; j++) {
            if (figures[i].points[j].x < minX) minX = figures[i].points[j].x;
            if (figures[i].points[j].x > maxX) maxX = figures[i].points[j].x;
            if (figures[i].points[j].y < minY) minY = figures[i].points[j].y;
            if (figures[i].points[j].y > maxY) maxY = figures[i].points[j].y;
        }
    }
    return {minX, minY, maxX, maxY};
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            indexTransformMatrix = (indexTransformMatrix - 1 + 4) % 4;
            InvalidateRect(hWnd, NULL, true);
            break;
        case VK_RIGHT:
            indexTransformMatrix = (indexTransformMatrix + 1) % 4;
            InvalidateRect(hWnd, NULL, true);
            break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            POINT body[] = {
                {150, 125},
                {150, 175},
                {200, 200},
                {350, 125},
                {335, 150},
                {350, 175},
                {200, 100},
                {100, 150},
                {150, 175}
            };

            POINT eye[] = {
                {135, 140},
                {135, 150},
                {120, 150},
                {135, 140}
            };
            
            Figure figures[] = {
                {body, 9},
                {eye, 4}
            };

            double zeroTrasformMatrix[3][3] = {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
            };

            RECT boundingBox = FindingLength(figures, 2);
            int deltaX = boundingBox.right - boundingBox.left;
            int deltaY = boundingBox.bottom - boundingBox.top;

            deltaX = deltaX * 3.5;

            double transpositionMatrix[3][3] = {
                {1, 0, 0},
                {0, 1, 0},
                {(double)deltaX, 0, 1}
            };

            double scalingMatrix[3][3] = {
                {0.5, 0 , 0},
                {0, 1, 0},
                {0, 0, 1}
            };

            double rotationMatrix[3][3]{
                {0.939693, -0.34202, 0},
                {0.34202, 0.939693, 0},
                {0, 0, 1}
            };

            double transformMatrix[3][3];

            switch (indexTransformMatrix)
            {
            case 0:
                memcpy(transformMatrix, zeroTrasformMatrix, sizeof(transformMatrix));
                break;
            case 1:
                memcpy(transformMatrix, transpositionMatrix, sizeof(transformMatrix));
                break;
            case 2:
                memcpy(transformMatrix, scalingMatrix, sizeof(transformMatrix));
                break;
            case 3:
                memcpy(transformMatrix, rotationMatrix, sizeof(transformMatrix));
                break;
            }

            DrawFiguresTrasform(hdc, figures, 2, transformMatrix);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
