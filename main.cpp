#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define FILE_MENU_NEW 1
#define FILE_MENU_BACK 2
#define FILE_MENU_EXIT 3
#define FILE_MENU_CONTROLS 4

#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void AddMenu(HWND);
void NewGame();
void GamePaint(char, HWND);
void DrawBoard(HWND);
void North();
void South();
void East();
void West();
void Test(HWND);
void CreateNewSquare();
void RedrawBoard(HWND);
bool CheckForValidMoves();
int PickColor(int, int);

HMENU hMenu;
char g_input;
int g_possible_positions[4][4][4] =
{
    {{10, 10, 105, 105},     {105, 10, 210, 105},    {210, 10, 315, 105},    {315, 10, 420, 105}},
    //
    {{10, 105, 105, 210},    {105, 105, 210, 210},   {210, 105, 315, 210},   {315, 105, 420, 210}},
    //
    {{10, 210, 105, 315},    {105, 210, 210, 315},   {210, 210, 315, 315},   {315, 210, 420, 315}},
    //
    {{10, 315, 105, 420},    {105, 315, 210, 420},   {210, 315, 315, 420},   {315, 315, 420, 420}},
    //
};
int g_value_in_position[4][4] = {0};

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */


    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Dano App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW,          /* default window *changed to non-resizable */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               445,                 /* The programs width */
               490,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_COMMAND:
        switch(wParam)
        {
        case FILE_MENU_EXIT:
            DestroyWindow(hwnd);
            break;
        case FILE_MENU_NEW:
            NewGame();
            RedrawBoard(hwnd);
            //MessageBeep(MB_DEFBUTTON1);
            break;
        case FILE_MENU_BACK:
            MessageBeep(MB_DEFBUTTON2);
            break;
        case FILE_MENU_CONTROLS:
            MessageBox( nullptr, TEXT( "Controls: \n W A S D to Move the rectangles." ), TEXT( "Message" ), MB_OK );
            break;
        }

        break;
    case WM_PAINT:
    {
        HDC hDC;
        PAINTSTRUCT Ps;
        hDC = BeginPaint(hwnd, &Ps);
        Rectangle(hDC, 9, 9, 421, 421);
        EndPaint(hwnd, &Ps);
    }
    case WM_KEYDOWN:
    {
        switch(wParam)
        {
        case 'W':
            GamePaint('W', hwnd);
            break;
        case 'S':
            GamePaint('S', hwnd);
            break;
        case 'A':
            GamePaint('A', hwnd);
            break;
        case 'D':
            GamePaint('D', hwnd);
            break;
        case 'T':
            GamePaint('T', hwnd);
            break;
        }
    }
    case WM_CREATE:
        AddMenu(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void AddMenu(HWND hwnd)
{
    hMenu = CreateMenu();

    AppendMenu(hMenu, MF_STRING, FILE_MENU_NEW, "New Game");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_BACK, "Go Back");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_CONTROLS, "Controls");
    AppendMenu(hMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    SetMenu(hwnd, hMenu);
}
void NewGame()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            g_value_in_position[i][j] = 0;
        }
    }
}
void GamePaint(char g_input, HWND hwnd) //create animations, make buttons do actions
{
    HDC hDC = GetDC(hwnd);
    PAINTSTRUCT Ps;
    if(CheckForValidMoves() == false)
    {
        MessageBox( NULL, "You Lost", "", MB_OK );
    }
    else
    {
        if(g_input == 'W')
        {
            North();
            RedrawBoard(hwnd);
            UpdateWindow(hwnd);
        }
        if(g_input == 'S')
        {
            South();
            RedrawBoard(hwnd);
            UpdateWindow(hwnd);
        }
        if(g_input == 'D')
        {
            East();
            RedrawBoard(hwnd);
            UpdateWindow(hwnd);
        }
        if(g_input == 'A')
        {
            West();
            RedrawBoard(hwnd);
            UpdateWindow(hwnd);
        }
        if(g_input == 'T')
        {
            Test(hwnd);
            //RedrawBoard(hwnd);
            UpdateWindow(hwnd);
        }
    }
    ReleaseDC(hwnd, hDC);
    EndPaint(hwnd, &Ps);
}
void Test(HWND hwnd)
{
    /*HDC hDC = GetDC(hwnd);
    RoundRect(hDC, 10, 10, 105, 105, 5, 5);*/
}
//Ka daryt jeigu 0 ir nepasidaro i true
void North()
{
    bool legal_move = false;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            while(g_value_in_position[i][j]==0 && g_value_in_position[i+1][j]!=0)
            {
                legal_move = true;
                g_value_in_position[i][j]=g_value_in_position[i+1][j];
                g_value_in_position[i+1][j]=0;
                i=0;
            }
            //Combine if equal
            if(g_value_in_position[i][j] == g_value_in_position[i+1][j])
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i+1][j];
                g_value_in_position[i+1][j]=0;
            }
        }
    }
    if (legal_move == true && CheckForValidMoves() == true)
    {
        CreateNewSquare();
    }
}
void South()
{
    bool legal_move = false;
    for(int i=3; i>0; i--)
    {
        for(int j=0; j<4; j++)
        {
            while(g_value_in_position[i][j]==0 && g_value_in_position[i-1][j]!=0)
            {
                legal_move = true;
                g_value_in_position[i][j]=g_value_in_position[i-1][j];
                g_value_in_position[i-1][j]=0;
                i=3;
            }
            //Combine if equal
            if(g_value_in_position[i][j] == g_value_in_position[i-1][j])
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i-1][j];
                g_value_in_position[i-1][j]=0;
            }
        }
    }
    if (legal_move == true && CheckForValidMoves() == true)
    {
        CreateNewSquare();
    }
}
void East()
{
    bool legal_move = false;
    for(int i=0; i<4; i++)
    {
        for(int j=3; j>0; j--)
        {
            while(g_value_in_position[i][j]==0 && g_value_in_position[i][j-1]!=0) // i 0  j 2
            {
                legal_move = true;
                g_value_in_position[i][j]=g_value_in_position[i][j-1];
                g_value_in_position[i][j-1]=0;
                j=3;

            }
            //Combine if equal
            if(g_value_in_position[i][j] == g_value_in_position[i][j-1])
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i][j-1];
                g_value_in_position[i][j-1]=0;
            }
        }
    }
    if (legal_move == true && CheckForValidMoves() == true)
    {
        CreateNewSquare();
    }
}
void West()
{
    bool legal_move = false;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<3; j++)
        {
            while(g_value_in_position[i][j]==0 && g_value_in_position[i][j+1]!=0) // i 0  j 2
            {
                legal_move = true;
                g_value_in_position[i][j]=g_value_in_position[i][j+1];
                g_value_in_position[i][j+1]=0;
                j=0;

            }
            //Combine if equal
            if(g_value_in_position[i][j] == g_value_in_position[i][j+1])
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i][j+1];
                g_value_in_position[i][j+1]=0;
            }
        }
    }
    if (legal_move == true && CheckForValidMoves() == true)
    {
        CreateNewSquare();
    }
}
void RedrawBoard(HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH hBrush;
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            int r = PickColor(g_value_in_position[i][j], 0);
            int g = PickColor(g_value_in_position[i][j], 1);
            int b = PickColor(g_value_in_position[i][j], 2);
            hBrush = CreateSolidBrush (RGB(r, g, b));
            SelectObject (hDC, hBrush);
            RECT rect;
            rect.left=g_possible_positions[i][j][0];
            rect.top=g_possible_positions[i][j][1];
            rect.right=g_possible_positions[i][j][2];
            rect.bottom=g_possible_positions[i][j][3];
            RoundRect(hDC,g_possible_positions[i][j][0], g_possible_positions[i][j][1], g_possible_positions[i][j][2], g_possible_positions[i][j][3], 5, 5);
            string RectangleValue = to_string(g_value_in_position[i][j]);;
            //FillRect(hDC, &rect, nullptr);

            DrawText(hDC, RectangleValue.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
        }
    }
}
bool CheckForValidMoves()
{
    bool move_exists=false;
    for(int i = 0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(g_value_in_position[i][j]==0)
            {
                move_exists=true;
            }
        }
    }
    return move_exists;
}
void CreateNewSquare() //draw a new square after a button is pressed
{
    srand((unsigned int) time (NULL));
    int random_x = rand()%4;
    int random_y = rand()%4;
    int random_value = rand()%10;
    //Checking if there are empty squares
    while(g_value_in_position[random_x][random_y] != 0)
    {
        random_x = rand()%4;
        random_y = rand()%4;
    }
    if(random_value < 8)
    {
        random_value = 2;
    }
    else
    {
        random_value = 4;
    }
    g_value_in_position[random_x][random_y] = random_value;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            std::cout<<g_value_in_position[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;


}

int PickColor(int value, int value_to_return)
{
    int values[] = {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2}; //11
    int color_values[][3] = {{237, 194, 46}, {237, 197, 63}, {237, 200, 80}, {237, 204, 97}, {237, 207, 114}, {246, 94, 59}, {246, 124, 95}, {245, 149, 99}, {242, 177, 121},{237, 224, 200}, {238, 228, 218}};
    // iterate from the end of colorValues cuz it goes from dark to light
    int length_of_values = (sizeof(values)/sizeof(values[0]));
    for(int i=0; i < length_of_values; i++)
    {
        if(value == values[i])
        {
            if(value_to_return == 0)
            {
                int r = color_values[i][0];
                return r;
            }
            if(value_to_return == 1)
            {
                int g = color_values[i][1];
                return g;
            }
            if(value_to_return == 2)
            {
                int b = color_values[i][2];
                return b;
            }
        }
    }
    return 255;
}
