#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <thread>

#define WIDTH 31
#define HEIGHT 21
#define RENDER_SPEED 100
#define MOVE_INTERVAL 3000

using namespace std;


char menu[HEIGHT][WIDTH] = {
    "                              ",
    "##############################",
    "#                            #",
    "#            MENU            #",
    "#                            #",
    "#    ********************    #",
    "#    |                  |    #",
    "#    |  > Start         |    #",
    "#    |                  |    #",
    "#    |    Exit          |    #",
    "#    |                  |    #",
    "#    |                  |    #",
    "#    ********************    #",
    "#                            #",
    "#                            #",
    "#    ####################    #",
    "#                            #",
    "#                            #",
    "##############################",
    "##############################"
};

bool run = true, isExit = false, moving = false;
unsigned int menuSelectY = 7;
POINT mousePointer;

void moveMouse() {
    while (run) {
        Sleep(MOVE_INTERVAL);

        if (moving && GetCursorPos(&mousePointer)) {
            SetCursorPos(mousePointer.x, mousePointer.y + 50);
            Sleep(100);
            
            if (moving && GetCursorPos(&mousePointer)) {
                SetCursorPos(mousePointer.x, mousePointer.y - 50);
                Sleep(100);
            }
        }
    }
}

void handleOnSelect() {
    if (isExit) {
        run = false;
    } else {
        moving = !moving;
        if (moving) {
            strcpy(menu[7], "#    |  > Stop          |    #");
        } else {
            strcpy(menu[7], "#    |  > Start         |    #");
        }
    }
}

void renderMenu() {
    for (int i = 0; i < HEIGHT; i++) {
        cout << menu[i] << endl;
    }
    cout << endl;
}

int main()
{
    SetConsoleTitleA("Move Mouse");

    auto moveThread = thread{
        []() {
            moveMouse();
        }
    };
    moveThread.detach();

    while (run) {
        renderMenu();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
        Sleep(200);

        if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_RETURN)) {
            handleOnSelect();
        }
        if (GetAsyncKeyState(VK_DOWN) && !isExit) {
            menuSelectY += 2;
        }
        if (GetAsyncKeyState(VK_UP) && isExit) {
            menuSelectY -= 2;
        }
        isExit = menuSelectY == 9;
        menu[menuSelectY][8] = '>';

        switch (menuSelectY) {
            case 7:
                menu[menuSelectY + 2][8] = ' ';
                break;
            case 9:
                menu[menuSelectY - 2][8] = ' ';
            default:
                break;
        }
    }

    return 0;
}
