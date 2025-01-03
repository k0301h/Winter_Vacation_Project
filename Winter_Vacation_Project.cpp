﻿// Winter_Vacation_Project.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "Winter_Vacation_Project.h"
#include <array>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(500, 1300);
std::uniform_int_distribution<int> dis2(500, 800);


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINTERVACATIONPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINTERVACATIONPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINTERVACATIONPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINTERVACATIONPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

constexpr int MAP_SIZE = 500;
constexpr int MAP_SIZE2 = 1300;
constexpr int MAP_SIZE3 = 800;

struct COLOR {
    BYTE r, g, b;
};

struct POSITION {
    int x, y;
};

class UNIT {
protected: 
    POSITION position;
    int size;
    COLOR color;
};

enum direction {
    RIGHT = 0,
    LEFT,
    UP,
    DOWN,
};

class MONSTER : UNIT {
    enum direction d;
public:
    MONSTER(int mx, int my, int msize, COLOR mcolor) {
        position.x = dis(gen);
        position.y = dis2(gen);
        size = msize;
        color = mcolor;
    }
    
    void Move(int mx, int my) {
       
    }

    POSITION GetPosition() {
        return position;
    }

    int GetSize() {
        return size;
    }
};


class PLAYER : UNIT {
    enum direction d;
public:
    PLAYER(int dx, int dy, int dsize, COLOR dcolor) {
        position.x = dx;
        position.y = dy;
        size = dsize;
        color = dcolor;
        d = RIGHT;
    }

    void Move(int dx, int dy) {
        if(position.x + dx >= 0 && position.x + size + dx <= MAP_SIZE)
            position.x += dx;
        if(position.y + dy >= 0 && position.y + size + dy <= MAP_SIZE)
            position.y += dy;

        if (dx > 0) d = RIGHT;
        else if (dx < 0) d = LEFT;
        else if (dy > 0) d = DOWN;
        else d = UP;
    }

    COLOR GetColor() {
        return color;
    }

    POSITION GetPosition() {
        return position;
    }

    int GetSize() {
        return size;
    }
   
    enum direction GetDirection() {
        return d;
    }
};

class BULLET : public UNIT {
    enum direction d;
public:
    bool isActive;

    BULLET() {
        isActive = false;   
    }

    BULLET(PLAYER player) {
        d = player.GetDirection();

        if (d == RIGHT) {
            position.x = player.GetPosition().x + player.GetSize();
            position.y = player.GetPosition().y + player.GetSize() / 2;
        }

        else if (d == LEFT) {
            position.x = player.GetPosition().x;
            position.y = player.GetPosition().y + player.GetSize() / 2;
        }

        else if (d == UP) {
            position.x = player.GetPosition().x + player.GetSize() / 2;
            position.y = player.GetPosition().y;
        }

        else if (d == DOWN) {
            position.x = player.GetPosition().x + player.GetSize() / 2;
            position.y = player.GetPosition().y + player.GetSize();
        }
        size = 10;
        color = { 0, 0, 0 };
    }

    void Move(int amount) {
        if (d == RIGHT) {
            if (position.x + amount <= MAP_SIZE)
                position.x += amount;
            else 
                isActive = false;
        }
        if (d == LEFT) {
            if( position.x >= -MAP_SIZE)
                position.x -= amount;
            else
                isActive = false;
        }
        if (d == UP) {
            if( position.y - amount >= -MAP_SIZE)
                position.y -= amount;
            else
                isActive = false;
        }
        if (d == DOWN) {
            if( position.y + amount <= MAP_SIZE)
                position.y += amount;
            else
                isActive = false;
        }
        
    }

    COLOR GetColor() {
        return color;
    }

    POSITION GetPosition() const {
        return position;
    }

    int GetSize() const {
        return size;
    }
};

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static PLAYER player{ 50, 50, 50, COLOR{255, 0, 0} };
    //static BULLET bullets[10];
    static MONSTER monster{ dis(gen), dis2(gen), 50, COLOR{125, 125, 0 }};

    //static std::array<BULLET, 10> bullets;
    static std::vector<MONSTER> monster;
    static std::vector<BULLET> bullets;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        SetTimer(hWnd, 2, 10, NULL);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RIGHT:
            player.Move(10, 0);
            break;
        case VK_UP:
            player.Move(0, -10);
            break;
        case VK_DOWN:
            player.Move(0, 10);
            break;
        case VK_LEFT:
            player.Move(-10, 0);
            break;
        case VK_SPACE:
            BULLET bullet{player};
            bullets.emplace_back(bullet);
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case 1: {
            for (int index = 0; index < bullets.size(); ++index) {
                if (!bullets[index].isActive) {
                    bullets.erase(bullets.begin() + index);
                    index--;
                }
            }
            break;
        }
        case 2:
            for (BULLET& bullet : bullets) {
                bullet.Move(10);
            }
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HPEN NewPen = CreatePen(PS_SOLID, 2, RGB(player.GetColor().r, player.GetColor().g, player.GetColor().b));
            HPEN OldPen = (HPEN)SelectObject(hdc, NewPen);

            Rectangle(hdc, player.GetPosition().x, player.GetPosition().y, player.GetPosition().x + player.GetSize(), player.GetPosition().y + player.GetSize());
           
            Rectangle(hdc, monster.GetPosition().x, monster.GetPosition().y, monster.GetPosition().x + monster.GetSize(), monster.GetPosition().y + monster.GetSize());
            
            SelectObject(hdc, OldPen);
            DeleteObject(NewPen);

            if (!bullets.empty()) {
                NewPen = CreatePen(PS_SOLID, 2, RGB(bullets[0].GetColor().r, bullets[0].GetColor().g, bullets[0].GetColor().b));
                OldPen = (HPEN)SelectObject(hdc, NewPen);

                for (const BULLET& bullet : bullets) {
                    if(bullet.isActive)
                       Ellipse(hdc, bullet.GetPosition().x, bullet.GetPosition().y, bullet.GetPosition().x + bullet.GetSize(), bullet.GetPosition().y + bullet.GetSize());    
                }

                SelectObject(hdc, OldPen);
                DeleteObject(NewPen);
            }
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

// 정보 대화 상자의 메시지 처리기입니다.
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
