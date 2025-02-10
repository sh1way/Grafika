// l1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
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

Point afinovTransform(const Point& p, double transpositionMatrix[3][3]) {
    return {
        p.x * transpositionMatrix[0][0] + p.y * transpositionMatrix[0][1] + p.w * transpositionMatrix[0][2],
        p.x* transpositionMatrix[1][0] + p.y * transpositionMatrix[1][1] + p.w * transpositionMatrix[1][2],
        1.0
    };
}

void DrawFigures(HDC hdc, Figure figures[], int figureCount) {
    for (int i = 0; i < figureCount; i++) {
        if (figures[i].count < 2) continue;

        MoveToEx(hdc, figures[i].points[0].x, figures[i].points[0].y, NULL);

        for (int j = 1; j < figures[i].count; j++) {
            LineTo(hdc, figures[i].points[j].x, figures[i].points[j].y);
        }
    }
}

void DrawSegment(HDC hdc) {
    MoveToEx(hdc, 100, 210, NULL);
    LineTo(hdc, 1350, 210);
    for (int i = 0; i < 6; i++) {
        MoveToEx(hdc, 100 + i * 250, 205, NULL);
        LineTo(hdc, 100 + i * 250, 215);
    }
    for (int i = 0; i < 6; i++) {
        MoveToEx(hdc, 225 + + i * 250, 208, NULL);
        LineTo(hdc, 225 + i * 250, 213);
    }
}

void DrawFiguresTrasposition(HDC hdc, Figure figures[], int figureCount, double transpositionMatrix[3][3]) {
    for (int i = 0; i < figureCount; i++) {
        if (figures[i].count < 2) continue;

        Point p = afinovTransform({(double)figures[i].points[0].x, (double)figures[i].points[0].y }, transpositionMatrix);
        MoveToEx(hdc, (int)p.x, (int)p.y, NULL);

        for (int j = 1; j < figures[i].count; j++) {
            p = afinovTransform({(double)figures[i].points[j].x, (double)figures[i].points[j].y }, transpositionMatrix);
            LineTo(hdc, (int)p.x, (int)p.y);
        }
    }
}

Point scaling(const Point& p, double scalingMatrix[3][3]) {
    return {
        p.x * scalingMatrix[0][0] + p.y * scalingMatrix[0][1] + p.w * scalingMatrix[0][2],
        p.x * scalingMatrix[1][0] + p.y * scalingMatrix[1][1] + p.w * scalingMatrix[1][2],
        1.0
    };
}

void DrawFiguresScaling(HDC hdc, Figure figures[], int figureCount, double scalingMatrix[3][3]) {
    for (int i = 0; i < figureCount; i++) {
        if (figures[i].count < 2) continue;

        Point p = scaling({ (double)figures[i].points[0].x, (double)figures[i].points[0].y }, scalingMatrix);
        MoveToEx(hdc, (int)p.x + 50, (int)p.y + 150, NULL);

        for (int j = 1; j < figures[i].count; j++) {
            p = scaling({ (double)figures[i].points[j].x, (double)figures[i].points[j].y }, scalingMatrix);
            LineTo(hdc, (int)p.x + 50, (int)p.y + 150);
        }
    }
}

Point rotation(const Point& p, double rotationMatrix[3][3]) {
    return {
        p.x * rotationMatrix[0][0] + p.y * rotationMatrix[0][1] + p.w * rotationMatrix[0][2],
        p.x * rotationMatrix[1][0] + p.y * rotationMatrix[1][1] + p.w * rotationMatrix[1][2],
        1.0
    };
}

void DrawRotation(HDC hdc, Figure figures[], int figureCount, double rotationMatrix[3][3]) {
    for (int i = 0; i < figureCount; i++) {
        if (figures[i].count < 2) continue;

        Point p = scaling({ (double)figures[i].points[0].x, (double)figures[i].points[0].y }, rotationMatrix);
        MoveToEx(hdc, (int)p.x+825, (int)p.y+250, NULL);

        for (int j = 1; j < figures[i].count; j++) {
            p = scaling({ (double)figures[i].points[j].x, (double)figures[i].points[j].y }, rotationMatrix);
            LineTo(hdc, (int)p.x+825, (int)p.y+250);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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

            DrawFigures(hdc, figures, 2);

            int deltaX = 250 * 3.5;

            double transpositionMatrix[3][3] = {
                {1, 0, deltaX},
                {0, 1, 0},
                {0, 0, 1}
            };

            DrawFiguresTrasposition(hdc, figures, 2, transpositionMatrix);

            DrawSegment(hdc);

            double scalingMatrix[3][3] = {
                {0.5, 0 , 0},
                {0, 1, 0},
                {0, 0, 1}
            };

            DrawFiguresScaling(hdc, figures, 2, scalingMatrix);

            double rotationMatrix[3][3]{
                {0.939693, -0.34202, 0},
                {0.34202, 0.939693, 0},
                {0, 0, 1}
            };

            DrawRotation(hdc, figures, 2, rotationMatrix);

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
