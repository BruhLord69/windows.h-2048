#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#define FILE_MENU_NEW 1
#define FILE_MENU_BACK 2
#define FILE_MENU_EXIT 3
#define FILE_MENU_CONTROLS 4
#define BTN_NEW 5

using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void AddMenu(HWND);
void NewGame(HWND);
void RestartGame();
void GamePaint(char, HWND);
void DrawBoard(HWND);
void North();
void South();
void East();
void West();
void Test(HWND);
void CreateNewSquare();
void RedrawBoard(HWND);
void DrawBoard(HWND);
void DrawScore(int, HWND);
void CheckForValidMoves();
void ReadHighScores();
void WriteHighScore(int);
void DrawHighScore(HWND);
int PickColor(int, int);

HBITMAP hBitmap = NULL;
HMENU hMenu;
HWND button_new;
char g_input;
int g_possible_positions[4][4][4] =
{
    {{16, 83, 112, 183},     {117, 83, 213, 183},    {218, 83, 314, 183},    {319, 83, 415, 183}},
    //
    {{16, 188, 112, 288},    {117, 188, 213, 288},   {218, 188, 314, 288},   {319, 188, 415, 288}},
    //
    {{16, 293, 112, 393},    {117, 293, 213, 393},   {218, 293, 314, 393},   {319, 293, 415, 393}},
    //
    {{16, 398, 112, 498},    {117, 398, 213, 498},   {218, 398, 314, 498},   {319, 398, 415, 498}},
    //
};
int g_high_score = 0;
int g_value_in_position[4][4] = {0};
bool g_new_game = true;
const string g_high_score_file = "High_scores.txt";

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
    HBRUSH window_background_color;
    window_background_color = CreateSolidBrush (RGB(240, 238, 229));
    wincl.hbrBackground = window_background_color;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Dano App"),       /* Title Text */
               WS_MINIMIZEBOX | WS_SYSMENU,          /* default window *changed to non-resizable */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               435,                 /* The programs width */
               560,                 /* and height in pixels */
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
            ReadHighScores();
            std::cout<<g_high_score<<std::endl;
            RestartGame();
            RedrawBoard(hwnd);
            //MessageBeep(MB_DEFBUTTON1);
            break;
        case FILE_MENU_BACK:
            MessageBeep(MB_DEFBUTTON2);
            break;
        case FILE_MENU_CONTROLS:
            MessageBox( nullptr, TEXT( "Controls: \n W A S D to Move the rectangles." ), TEXT( "Message" ), MB_OK );
            break;
        case BTN_NEW:
            DestroyWindow(button_new);
            RestartGame();
            RedrawBoard(hwnd);
            break;
        }

        break;
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
        HDC hDC;
        PAINTSTRUCT Ps;

        hDC = BeginPaint(hwnd, &Ps);

        if(g_new_game == true)
        {
            NewGame(hwnd);
        }
        AddMenu(hwnd);
        EndPaint(hwnd, &Ps);
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
void RestartGame()
{

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            g_value_in_position[i][j] = 0;
        }
    }
    CreateNewSquare();
}
void NewGame(HWND hwnd)
{
    ReadHighScores();
    g_new_game = false;
    HDC hDC = GetDC(hwnd);

    button_new = CreateWindow(
                     "BUTTON",  // Predefined class; Unicode assumed
                     "New Game",      // Button text
                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // Styles
                     160,         // x position
                     160,         // y position
                     100,        // Button width
                     50,        // Button height
                     hwnd,     // Parent window
                     (HMENU) BTN_NEW,
                     NULL,
                     NULL);      // Pointer not needed.


    HBRUSH brush = CreatePatternBrush((HBITMAP)LoadImage(NULL, "Background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    ReleaseDC(hwnd, hDC);
}
void GamePaint(char g_input, HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    PAINTSTRUCT Ps;
    CheckForValidMoves();
    if(g_input == 'W')
    {
        North();
        RedrawBoard(hwnd);
    }
    if(g_input == 'S')
    {
        South();
        RedrawBoard(hwnd);
    }
    if(g_input == 'D')
    {
        East();
        RedrawBoard(hwnd);
    }
    if(g_input == 'A')
    {
        West();
        RedrawBoard(hwnd);
    }
    ReleaseDC(hwnd, hDC);
    EndPaint(hwnd, &Ps);
}
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
            if(g_value_in_position[i][j] == g_value_in_position[i+1][j] && g_value_in_position[i][j] != 0)
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i+1][j];
                g_value_in_position[i+1][j]=0;
            }
        }
    }

    if (legal_move == true)
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
            if(g_value_in_position[i][j] == g_value_in_position[i-1][j] && g_value_in_position[i][j] != 0)
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i-1][j];
                g_value_in_position[i-1][j]=0;
            }
        }
    }
    if (legal_move == true)
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
            if(g_value_in_position[i][j] == g_value_in_position[i][j-1] && g_value_in_position[i][j] != 0)
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i][j-1];
                g_value_in_position[i][j-1]=0;
            }
        }
    }
    if (legal_move == true)
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
            if(g_value_in_position[i][j] == g_value_in_position[i][j+1] && g_value_in_position[i][j] != 0)
            {
                legal_move = true;
                g_value_in_position[i][j] += g_value_in_position[i][j+1];
                g_value_in_position[i][j+1]=0;
            }
        }
    }

    if (legal_move == true)
    {
        CreateNewSquare();
    }
}
void RedrawBoard(HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH hBrush;
    int sum = 0;
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    DrawBoard(hwnd);
    //Number drawing
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            sum += g_value_in_position[i][j];
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
            RoundRect(hDC,g_possible_positions[i][j][0], g_possible_positions[i][j][1], g_possible_positions[i][j][2], g_possible_positions[i][j][3], 10, 10);
            string RectangleValue = to_string(g_value_in_position[i][j]);
            SetBkColor(hDC, RGB(r, g, b));
            DrawText(hDC, RectangleValue.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
        }
    }
    DrawScore(sum, hwnd);
    DrawHighScore(hwnd);

}
void DrawBoard(HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH game_background_color;
    game_background_color = CreateSolidBrush (RGB(167, 153, 140));
    SelectObject (hDC, game_background_color);
    RECT game_background_rectangle;
    game_background_rectangle.left = 10;
    game_background_rectangle.top = 77;
    game_background_rectangle.right = 421;
    game_background_rectangle.bottom = 504;
    FillRect(hDC, &game_background_rectangle, game_background_color);
}
void DrawScore(int sum, HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH score_background_color;
    score_background_color = CreateSolidBrush (RGB(207, 193, 180));
    SelectObject (hDC, score_background_color);
    RoundRect(hDC,10,10,160,60,10,10);

    RECT game_score_rectangle;
    game_score_rectangle.left = 10;
    game_score_rectangle.top = 22;
    game_score_rectangle.right = 160;
    game_score_rectangle.bottom = 70;

    string game_score = to_string(sum);
    SetBkColor(hDC, RGB(207, 193, 180));
    DrawText(hDC, game_score.c_str(),-1, &game_score_rectangle, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);

    RECT score_string_rectangle;
    score_string_rectangle.left = 10;
    score_string_rectangle.top = 0;
    score_string_rectangle.right = 160;
    score_string_rectangle.bottom = 40;
    DrawText(hDC,"SCORE",5, &score_string_rectangle, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
}
void DrawHighScore(HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH score_background_color;
    score_background_color = CreateSolidBrush (RGB(207, 193, 180));
    SelectObject (hDC, score_background_color);
    RoundRect(hDC,271,10,421,60,10,10);

    RECT game_score_rectangle;
    game_score_rectangle.left = 271;
    game_score_rectangle.top = 22;
    game_score_rectangle.right = 421;
    game_score_rectangle.bottom = 70;

    string high_score = to_string(g_high_score);
    SetBkColor(hDC, RGB(207, 193, 180));
    DrawText(hDC, high_score.c_str(),-1, &game_score_rectangle, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);

    RECT score_string_rectangle;
    score_string_rectangle.left = 274;
    score_string_rectangle.top = 0;
    score_string_rectangle.right = 421;
    score_string_rectangle.bottom = 40;
    DrawText(hDC,"HIGH SCORE",10, &score_string_rectangle, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
}
void CheckForValidMoves()
{
    bool move_exists=false;
    //North
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(g_value_in_position[i][j]==0 && g_value_in_position[i+1][j]!=0)
            {
                move_exists = true;
            }
            if(g_value_in_position[i][j] == g_value_in_position[i+1][j] && g_value_in_position[i][j] != 0)
            {
                move_exists = true;
            }
        }
    }
    //South
    for(int i=3; i>0; i--)
    {
        for(int j=0; j<4; j++)
        {
            if(g_value_in_position[i][j]==0 && g_value_in_position[i-1][j]!=0)
            {
                move_exists = true;
            }
            if(g_value_in_position[i][j] == g_value_in_position[i-1][j])
            {
                move_exists = true;
            }
        }
    }
    //East
    for(int i=0; i<4; i++)
    {
        for(int j=3; j>0; j--)
        {
            if(g_value_in_position[i][j]==0 && g_value_in_position[i][j-1]!=0)
            {
                move_exists = true;
            }
            //Combine if equal
            if(g_value_in_position[i][j] == g_value_in_position[i][j-1])
            {
                move_exists = true;
            }
        }
    }
    //West
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(g_value_in_position[i][j]==0 && g_value_in_position[i][j+1]!=0)
            {
                move_exists = true;
            }
            if(g_value_in_position[i][j] == g_value_in_position[i][j+1])
            {
                move_exists = true;
            }
        }
    }

    if(move_exists == false)
    {
        int sum = 0;
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                sum+= g_value_in_position[i][j];
            }
        }
        WriteHighScore(sum);
        MessageBox( NULL, "You Lost", "", MB_OK );
    }
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
}

int PickColor(int value, int value_to_return)
{
    int values[] = {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 0}; //11
    int color_values[][3] = {{237, 194, 46}, {237, 197, 63}, {237, 200, 80}, {237, 204, 97}, {237, 207, 114}, {246, 94, 59}, {246, 124, 95}, {245, 149, 99}, {242, 177, 121},{237, 224, 200}, {238, 228, 218}, {205, 193, 180}};
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
void ReadHighScores()
{
    ifstream input(g_high_score_file);
    string line;
    if(input.is_open())
    {
        while(std::getline(input, line))
        {
            int score = stoi(line);
            if(score > g_high_score) g_high_score = score;
        }
    }
}
void WriteHighScore(int score)
{
    ofstream output;
    output.open(g_high_score_file, fstream::app);
    if(score > g_high_score)
    {
        output<<score;
        output<<"\r\n";
    }
    output.close();
}
