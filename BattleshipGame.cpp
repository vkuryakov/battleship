// BattleshipGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;

const int FIELD_SIZE = 10;
const char START_SYMBOL = 'А';
const string EMPTY_HOR_CELL = "     ";
const string TWO_SPACES = "  ";

struct APlayer {
    string name;
    // Координаты выстрелов:
    // true - был выстрел
    // false - не было выстрела
    bool shoots[10][10];

    // Координаты кораблей
    // true - есть корабль на клетке
    // false - нет корабля на клетке
    bool shipCells[10][10];
};

void draw_field_header() {
    cout << EMPTY_HOR_CELL;
    for (int i = 0; i < FIELD_SIZE - 1; i++) {
        cout << TWO_SPACES << (char)(START_SYMBOL + i) << TWO_SPACES;
    }
    cout << TWO_SPACES << "К" << TWO_SPACES;
}

void draw_top_border() {
    cout << EMPTY_HOR_CELL;
    for (int i = 1; i <= FIELD_SIZE; i++) {
        cout << "_____";
    }
}

void draw_row_first_line(string str) {
    cout << "    |";
    for (int i = 0; i < FIELD_SIZE; i++) {
        cout << str;
    }
    cout << "|";
}

void draw_row_second_line(int num) {
    cout << " " << num;
    if (num < 10) {
        cout << "  |";
    }
    else {
        cout << " |";
    }

    for (int j = 0; j < FIELD_SIZE; j++) {
        cout << TWO_SPACES << " " << TWO_SPACES;
    }
    cout << "|";
}

void draw_row(int num) {
    // Первая системная строка
    draw_row_first_line(EMPTY_HOR_CELL);
    cout << EMPTY_HOR_CELL;
    draw_row_first_line(EMPTY_HOR_CELL);
    cout << endl;
    // Вторая системная строка
    draw_row_second_line(num);
    cout << EMPTY_HOR_CELL;
    draw_row_second_line(num);
    cout << endl;
    // Третья системная строка

    if (num < FIELD_SIZE) {
        draw_row_first_line(EMPTY_HOR_CELL);
    }
    else {
        draw_row_first_line("_____");
    }
    cout << EMPTY_HOR_CELL;
    if (num < FIELD_SIZE) {
        draw_row_first_line(EMPTY_HOR_CELL);
    }
    else {
        draw_row_first_line("_____");
    }
    cout << endl;
}

void show_field(APlayer player) {
    system("cls");
    // Выводим буквы столбцов первого поля
    draw_field_header();

    // Выводим буквы столбцов второго поля
    cout << EMPTY_HOR_CELL << " ";
    draw_field_header();
    cout << endl;
    draw_top_border();
    cout << EMPTY_HOR_CELL << " ";
    draw_top_border();
    cout << endl;


    // Выводим поле
    // Каждая строка поля состоит из трех системных строк:
    // 1. Пустая строка - для визуального отделения символа от верхнего разделителя.
    // 2. Строка с символами.
    // 3. Строка - вертикальный разделитель: в ней содержатся символы подчеркивания для вертикального разделения клеток.
    for (int i = 1; i <= FIELD_SIZE; i++) {
        draw_row(i);
    }
}

void show_menu() {
    int input_var;
    do {
        system("cls");
        cout << "\t\tBattleship The Game." << endl;
        cout << "1. Player vs Computer." << endl;
        cout << "2. Computer vs Computer." << endl;
        cout << "0. Exit." << endl;
        cin >> input_var;
    } while (input_var < 0 || input_var > 2);

}

int main()
{
    setlocale(LC_CTYPE, "Russian");
    srand(time(NULL));
    //HWND console = GetConsoleWindow();
    //RECT r;
    //GetWindowRect(console, &r);
    //MoveWindow(console, r.left, r.top, 1500, 600, TRUE);
    show_field({});
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
