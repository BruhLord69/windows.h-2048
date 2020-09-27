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

//using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void AddMenu(HWND);
void GamePaint(char, HWND);
void GameArea(HWND);
int pickColor(int, int);
int normalizeWindowSize(int);

HMENU hMenu;
char input;
int rectPositions[16][4];
int textPositions[16][4];
int valueInPosition[16];
int curPos;
int right;
int bot;

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
               WS_OVERLAPPEDWINDOW  ,          /* default window *changed to non-resizable */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               435,                 /* The programs width */
               480,                 /* and height in pixels */
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
            //WM_PAINT;
            MessageBeep(MB_DEFBUTTON1);
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
        //Rectangle(hDC, 10, 10, 420, 420);
        GameArea(hwnd);
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
        }
    }
    case WM_CREATE:
        AddMenu(hwnd);
        GamePaint('W', hwnd);
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
void GameArea(HWND hwnd)
{
    HDC hDC = GetDC(hwnd);
    HBRUSH hBrush;
    hBrush = CreateSolidBrush (RGB(212, 156, 180));
    RECT rect;
    GetClientRect(hwnd,&rect);

    int width = rect.right;
    int height = rect.bottom;
    //std::cout<<"Original right: "<<width<<" OG BOT: "<<height<<" ";
    //width: 429 height: 431
    //428 / 2 = 214
    // 408 / 2 = 202
    // left = middle - half
    // right = middle + half
    //

    width = normalizeWindowSize(width);
    height = normalizeWindowSize(height);
    //cout<<"After change right: "<<width<<" After change BOT: "<<height<<" ";
    if(width == height)
    {
        right = width - 10;
        bot = height - 10;
    }
    else if (width > height)
    {
        right = height - 10;
        bot = height - 10;
    }
    else
    {
        right = width - 10;
        bot = width - 10;
    }

    RECT window;
    window.left = 10;
    window.top = 10;
    window.right = right;
    window.bottom = bot;
    FillRect(hDC, &window, hBrush);

    ReleaseDC(hwnd,hDC);
    std::cout<<"right: "<< right<<" Client window bot: "<<height<<" bot: "<<bot<<std::endl;
}
int normalizeWindowSize(int size)
{
    while(size % 2 != 0)
    {
        size--;
    }
    return size;
}
void calculatePositions();
void GamePaint(char input, HWND hwnd) //create animations, make buttons do actions
{
    HDC hDC = GetDC(hwnd);
    PAINTSTRUCT Ps;
    //int positionDim1 = rand() % 16;
    /*if(input == 'W')
    {
        if(valueInPosition[positionDim1] * 2 == 2048)
        {
            valueInPosition[positionDim1] *= 2;
            MessageBox( NULL, "You Won!", "", MB_OK );
        }
        else if (valueInPosition[positionDim1]!= 0) valueInPosition[positionDim1] *= 2; // game should end at 2048
        else valueInPosition[positionDim1] = 2;
        cout<<" Pos: "<<positionDim1<<" Val: "<<valueInPosition[positionDim1]<<endl;
        string RectangleValue = to_string(valueInPosition[positionDim1]);;
        HBRUSH hBrush;
        int R = pickColor(valueInPosition[positionDim1], 0);
        int G = pickColor(valueInPosition[positionDim1], 1);
        int B = pickColor(valueInPosition[positionDim1], 2);
        hBrush = CreateSolidBrush (RGB(R, G, B));
        SelectObject (hDC, hBrush);
        RECT rect;
        rect.left=possiblePositions[positionDim1][0];
        rect.top=possiblePositions[positionDim1][1];
        rect.right=possiblePositions[positionDim1][2];
        rect.bottom=possiblePositions[positionDim1][3];

        FillRect(hDC, &rect, hBrush);

        RECT textRect;
        textRect.left=possiblePositions[positionDim1][0]+47.5;
        textRect.top=possiblePositions[positionDim1][1]+47.5;
        textRect.right=possiblePositions[positionDim1][2]+47.5;
        textRect.bottom=possiblePositions[positionDim1][3]+47.5;

        DrawText( hDC, RectangleValue.c_str(), -1, &textRect, DT_SINGLELINE | DT_NOCLIP  );

        UpdateWindow(hwnd);
    }
// RedrawWindow would work really well to redraw the window
    if(input == 'D')
    {
        //InvalidateRect(hwnd, NULL, TRUE);
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        UpdateWindow(hwnd);
    }
    ReleaseDC(hwnd, hDC);
    EndPaint(hwnd, &Ps);*/
    if(input == 'A')
    {
        GameArea(hwnd);
    }
}
void calculateNewPositions() //function to recalculate new positions
{

}
void setPosition() //function to generate a new number
{
    curPos = rand() % 16;
}
int getPosition() {
    return curPos;
}
void updateSquareValue()
{

}
void createNewSquare() //draw a new square after a button is pressed
{

}
int pickColor(int value, int valueToReturn)
{
    int values[] = {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2}; //11
    int colorValues[][3] = {{53, 0, 51}, {70, 0, 60}, {85, 0, 64}, {99, 2, 66}, {112, 7, 65}, {123, 14, 62}, {139, 39, 102}, {155, 65, 138}, {171, 91, 170}, {174, 117, 186}, {181, 144, 201}, {192, 172, 215}};
    // iterate from the end of colorValues cuz it goes from dark to light
    int lengthOfValues = (sizeof(values)/sizeof(values[0]));
    for(int i=0; i < lengthOfValues; i++)
    {
        if(value == values[i])
        {
            if(valueToReturn == 0)
            {
                int R = colorValues[i][0];
                return R;
            }
            if(valueToReturn == 1)
            {
                int G = colorValues[i][1];
                return G;
            }
            if(valueToReturn == 2)
            {
                int B = colorValues[i][2];
                return B;
            }
        }
    }
    return 0;
}
