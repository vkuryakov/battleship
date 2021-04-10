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

// Режим компьютера: 
// 0 - тупой (стреляет по случайным клеткам);
// 1 - поумней (стреляет по алгоритму);
int computer_mode = 0;

void show_menu();

struct Player {
    // Координаты выстрелов:
    // true - был выстрел
    // false - не было выстрела
    bool enemy_field[FIELD_SIZE][FIELD_SIZE];

    // Координаты кораблей
    // true - есть корабль на клетке
    // false - нет корабля на клетке
    bool own_field[FIELD_SIZE][FIELD_SIZE];
};

struct Point {
    int x, y;
};

Player player1 = {};
const Player player2 = {};

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

void show_field() {
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

void show_computer_difficulty_dialog() {
    int input_var;
    do {
        system("cls");
        cout << "\t\tМорской бой." << endl;
        cout << "1. Легкий" << (computer_mode == 0 ? ".\t+" : ".") << endl;
        cout << "2. Сложный" << (computer_mode == 1 ? ".\t+" : ".") << endl;
        cout << "0. Назад." << endl;
        cin >> input_var;
        switch (input_var) {
            case 1:
                computer_mode = 0;
                break;
            case 2:
                computer_mode = 1;
                break;
            default:
                break;
        }
    } while (input_var != 0);
    show_menu();
}

void show_menu() {
    int input_var;
    do {
        system("cls");
        cout << "\tМорской бой." << endl;
        cout << "1. Человек против компьютера." << endl;
        cout << "2. Компьютер против компьютера." << endl;
        cout << "3. Установить сложность компьютера." << endl;
        cout << "0. Выход." << endl;
        cin >> input_var;
    } while (input_var < 0 || input_var > 3);
    switch (input_var) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            show_computer_difficulty_dialog();
            break;
        default:
            break;
    }
}

bool is_coords_valid(bool field[FIELD_SIZE][FIELD_SIZE], int x, int y, int direction, int deck) {
    if (field[x][y]) return false;
    int x_cur, y_cur;
    for (int i = -1; i < 2; i++) {
        x_cur = x + i;
        if (x_cur < 0 || x_cur > FIELD_SIZE) continue;
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;
            if (deck > 0 
                && ((direction == 0 && i == -1 && j == 0)
                    || (direction == 1 && i == 0 && j == 1)
                    || (direction == 2 && i == 1 && j == 0)
                    || (direction == 3 && i == 0 && j == -1))) {
                continue;
            }
            
            y_cur = y + i;
            if (field[x_cur][y_cur]) return false;
        }
    }
    return true;
}

bool is_ship_coords_valid(bool field[FIELD_SIZE][FIELD_SIZE], Point p1, Point p2) {
    if (p1.x < -1 || p1.x > FIELD_SIZE
        || p1.y < -1 || p1.y > FIELD_SIZE
        || p2.x < -1 || p2.x > FIELD_SIZE
        || p2.y < -1 || p2.y > FIELD_SIZE) return false;
    
    for (int i = p1.x; i <= p2.x && i > 0 && i < FIELD_SIZE - 1; i++) {
        for (int j = p1.y; j <= p2.y && j > 0 && j < FIELD_SIZE - 1; j++) {
            if (field[i][j]) return false;
        }
    }
    return true;
}

void clear_player(Player p) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            p.own_field[i][j] = false;
            p.enemy_field[i][j] = false;
        }
    }
}

void set_ships_auto(Player &p) {
    // служит для вычисления кол-ва кораблей (например, 1 единица 4-палубных кораблей = 4 + 1 = 5, 2 единицы 3-палубных = 2 + 3 = 5 и т.д.)
    const int cells_count_sum = 5;
    int ships_count, x, y, dir;
    Point p1, p2;
    // кол-во палуб корабля
    for (int i = 1; i < cells_count_sum; i++) {
        ships_count = cells_count_sum - i;
        // корабли
        for (int j = 0; j < ships_count; j++) {

            do {
                x = rand() % FIELD_SIZE;
                y = rand() % FIELD_SIZE;
                // Направление:
                // 0 - горизонтальное (слева направо);
                // 1 - вертикальное (сверху вниз);
                dir = rand() % 2;

                p1.x = x - 1;
                p1.y = y - 1;
                
                if (dir == 0) {
                    p2.x = x + i;
                    p2.y = y + 1;
                }
                else {
                    p2.x = x + 1;
                    p2.y = y + i;
                }
            } while (!is_ship_coords_valid(p.own_field, p1, p2));
            for (int k = 0; k < i; k++) {
                if (dir == 0) {
                    p.own_field[x + k][y] = true;
                }
                else {
                    p.own_field[x][y + k] = true;
                }
            }
            
            // рассчет координат палуб
            //for (int k = 0; k < i; k++) {
            //    do {
            //        if (k == 0) {
            //            x = rand() % FIELD_SIZE;
            //            y = rand() % FIELD_SIZE;
            //            if (i > 1) {
            //                if (x == 0) {
            //                    if (y == 0) dir = rand() % 2 + 1;
            //                    else if (y == FIELD_SIZE - 1) dir = rand() % 2;
            //                    else dir = rand() % 3
            //                }
            //                else if (x == FIELD_SIZE - 1) {
            //                    if (y == 0) dir = rand() % 2 + 1;
            //                    if (y == FIELD_SIZE - 1) dir = rand() % 2;
            //                }
            //            }
            //            else {
            //                dir = 0;
            //            }
            //            
            //            dir = i > 1 ? rand() % 4 : 0;
            //        }
            //        else {
            //            
            //        }
            //        
            //    } while (!is_coords_valid(p.own_field, x, y, dir, k));
            //    p.own_field[x][y] = true;
            //}
        }
    }
}

void run_human_vs_comp() {

}

void run_comp_vs_comp() {

}


int main()
{
    setlocale(LC_CTYPE, "Russian");
    srand(time(NULL));
    set_ships_auto(player1);
    show_menu();
    //show_field();
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
