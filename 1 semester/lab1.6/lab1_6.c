#include <stdio.h>
#include <windows.h>

int main() {
    // отримання розмірів консолі
    int columns, rows;
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hout, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    short left_col = 0;  // номер стовбця для лівої стрілки
    short right_col = columns - 1;  // номер стовбця для правої стрілки
    COORD Pos;

    for (int i = 0; i < columns / 4; i++) {
        short down_row = rows - 1; //номер рядка для руху стрілок знизу вгору
        short up_row = 0;  // номер рядка для руху стрілок згори вниз

        for (int j = 0; j < rows; j++, down_row--) {
            Pos.X = left_col;
            Pos.Y = down_row;
            SetConsoleCursorPosition(hout, Pos);
            printf("*");
            Sleep(100);
            Pos.X = right_col;
            SetConsoleCursorPosition(hout, Pos);
            printf("*");
            Sleep(100);
        }
        left_col++;
        right_col--;

        for (int k = 0; k < rows; k++, up_row++) {
            Pos.X = left_col;
            Pos.Y = up_row;
            SetConsoleCursorPosition(hout, Pos);
            printf("*");
            Sleep(100);
            Pos.X = right_col;
            SetConsoleCursorPosition(hout, Pos);
            printf("*");
            Sleep(100);
        }
        left_col++;
        right_col--;
    };
    getchar();
}