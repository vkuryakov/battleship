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
    bool shoots[FIELD_SIZE][FIELD_SIZE];

    // Координаты кораблей
    // true - есть корабль на клетке
    // false - нет корабля на клетке
    bool own_field[FIELD_SIZE][FIELD_SIZE];
};

struct Point {
    int x, y;
};

Player player1 = {};
Player player2 = {};

void show_arr(Player p) {
    cout << endl;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            cout << '\t' << p.own_field[i][j];
        }
        cout << endl;
    }
}

bool is_need_divider_on_enemy_field(Player p, Player enemy, int row_num, int i) {
    return (i == FIELD_SIZE - 1)
        || ((enemy.own_field[row_num][i] && p.shoots[row_num][i]) && (!p.shoots[row_num][i + 1] || !enemy.own_field[row_num][i + 1]))
        || ((enemy.own_field[row_num][i + 1] && p.shoots[row_num][i + 1]) && (!p.shoots[row_num][i] || !enemy.own_field[row_num][i]));
}

void draw_field_header() {
    cout << EMPTY_HOR_CELL;
    for (int i = 0; i < FIELD_SIZE - 1; i++) {
        cout << TWO_SPACES << (char)(START_SYMBOL + i) << TWO_SPACES << " ";
    }
    cout << TWO_SPACES << "К" << TWO_SPACES;
}

void draw_top_border() {
    cout << EMPTY_HOR_CELL;
    for (int i = 0; i < FIELD_SIZE; i++) {
        if (i < FIELD_SIZE - 1) {
            cout << "______";
        }
        else {
            cout << "_____";
        }
    }
}

void draw_row_up_line(int row_num, Player p, Player enemy, bool is_own_field) {
    cout << "    |";
    if (is_own_field) {
        for (int i = 0; i < FIELD_SIZE; i++) {
            cout << "     ";
            if (i == FIELD_SIZE - 1 || p.own_field[row_num][i] != p.own_field[row_num][i + 1]) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
    }
    else {
        for (int i = 0; i < FIELD_SIZE; i++) {
            cout << EMPTY_HOR_CELL;
            if (is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
    }
}

void draw_row_bottom_line(int row_num, Player p, Player enemy, bool is_own_field) {
    bool is_first_deck, is_middle_deck, is_last_deck;
    cout << "    |";
    if (is_own_field) {

        // i - x - столбец
        // row_num - y - строка
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (row_num == FIELD_SIZE - 1) {
                if (i == FIELD_SIZE - 1 || p.own_field[row_num][i] != p.own_field[row_num][i + 1]) {
                    cout << "_____|";
                }
                else {
                    cout << "______";
                }
                continue;
            }
            if (p.own_field[row_num][i] != p.own_field[row_num + 1][i]) {
                cout << "_____";
            }
            else {
                cout << "     ";
            }

            if (i == FIELD_SIZE - 1 || p.own_field[row_num][i] != p.own_field[row_num][i + 1]) {
                cout << "|";
            }
            else if ((p.own_field[row_num][i] && p.own_field[row_num][i + 1]) || (p.own_field[row_num + 1][i] && p.own_field[row_num + 1][i + 1])) {
                cout << "_";
            }
            else {
                cout << " ";
            }
        }
    }
    else {
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (row_num == FIELD_SIZE - 1) {
                if (is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                    cout << "_____|";
                }
                else {
                    cout << "______";
                }
            }
            else {
                if (is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                    cout << "     |";
                }
                else {
                    cout << "      ";
                }
            }
            
        }
    }
}

void draw_row_middle_line(int row_num, Player p, Player enemy, bool is_own_field) {
    cout << " " << row_num + 1;
    if (row_num < FIELD_SIZE - 1) {
        cout << "  |";
    }
    else {
        cout << " |";
    }

    if (is_own_field) {
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (p.own_field[row_num][i]) {
                if (enemy.shoots[row_num][i]) {
                    cout << "  X  ";
                }
                else {
                    cout << "     ";
                }
            }
            else {
                if (enemy.shoots[row_num][i]) {
                    cout << "  *  ";
                }
                else {
                    cout << "     ";
                }
                
            }

            if (i == FIELD_SIZE - 1 || p.own_field[row_num][i] != p.own_field[row_num][i + 1]) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
    }
    else {
        for (int i = 0; i < FIELD_SIZE; i++) {
            if (!p.shoots[row_num][i]) {
                cout << EMPTY_HOR_CELL;
            }
            else {
                if (enemy.own_field[row_num][i]) {
                    cout << "  X  ";
                }
                else {
                    cout << "  *  ";
                }
            }
            
            if (is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
    }
}

void draw_row(int num, Player p, Player enemy) {
    // Первая системная строка
    draw_row_up_line(num, p, enemy, true);
    cout << EMPTY_HOR_CELL;
    draw_row_up_line(num, p, enemy, false);
    cout << endl;
    // Вторая системная строка
    draw_row_middle_line(num, p, enemy, true);
    cout << EMPTY_HOR_CELL;
    draw_row_middle_line(num, p, enemy, false);
    cout << endl;
    // Третья системная строка

    draw_row_bottom_line(num, p, enemy, true);
    cout << EMPTY_HOR_CELL;
    draw_row_bottom_line(num, p, enemy, false);
    cout << endl;
}

void show_field(Player p, Player enemy) {
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
    for (int i = 0; i < FIELD_SIZE; i++) {
        draw_row(i, p, enemy);
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
    if (p1.x < 0 || p1.x > FIELD_SIZE
        || p1.y < 0 || p1.y > FIELD_SIZE
        || p2.x < 0 || p2.x > FIELD_SIZE
        || p2.y < 0 || p2.y > FIELD_SIZE) return false;
    
    for (int i = p1.x; i <= p2.x && i < FIELD_SIZE; i++) {
        for (int j = p1.y; j <= p2.y && j < FIELD_SIZE; j++) {
            if (field[j][i]) return false;
        }
    }
    return true;
}

void clear_player(Player p) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            p.own_field[i][j] = false;
            p.shoots[i][j] = false;
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

                p1.x = x > 0 ? x - 1 : x;
                p1.y = y > 0 ? y - 1 : y;
                
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
                    p.own_field[y][x + k] = true;
                }
                else {
                    p.own_field[y + k][x] = true;
                }
            }
        }
    }
}

void test_data() {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (rand() % 2) {
                player2.shoots[i][j] = true;
            }
            else {
                player2.shoots[i][j] = false;
            }
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
    test_data();
    set_ships_auto(player1);
    //show_menu();
    show_field(player1, player2);
    show_arr(player1);
}