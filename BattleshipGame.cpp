// BattleshipGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <time.h>
#include <windows.h>
#include <conio.h>

using namespace std;

const int FIELD_SIZE = 10;
const char START_SYMBOL = 'А';
const string EMPTY_HOR_CELL = "     ";
const string TWO_SPACES = "  ";
bool exit_game = false;

// По какой-то пока не ясной мне причине коды символов при выводе и при вводе различаются.
// По этой причине введен этот костыль.
// Первый элемент - код символа для вывода на консоль.
// Второй элемент - код символа для ввода пользователем.

int SYMBOLS_CODES[FIELD_SIZE][2] = { 
    {-64, -128}, // А
    {-63, -127}, // Б
    {-62, -126}, // В
    {-61, -125}, // Г
    {-60, -124}, // Д
    {-59, -123}, // Е
    {-58, -122}, // Ж
    {-57, -121}, // З
    {-56, -120}, // И
    {-54, -118}  // К
};

// Режим компьютера: 
// 0 - тупой (стреляет по случайным клеткам);
// 1 - поумней (стреляет по алгоритму);
int computer_mode = 0;

// Режим игры:
// 0 - игрок против компьютера;
// 1 - компьютер против компьютера;
int game_mode = 0;

struct Player {
    string name;
    bool is_comp = false;
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

void show_menu();
void set_ships_auto(Player& p);
bool is_ship_coords_valid(bool field[FIELD_SIZE][FIELD_SIZE], Point p1, Point p2);
bool is_ship_destroyed(Player player, Player enemy, Point point);

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
    for (int i = 0; i < FIELD_SIZE; i++) {
        cout << TWO_SPACES << (char)SYMBOLS_CODES[i][0] << TWO_SPACES << " ";
    }
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
            cout << EMPTY_HOR_CELL;
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
    cout << "    |";
    if (is_own_field) {

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
                cout << EMPTY_HOR_CELL;
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
                cout << "_____";
                if ((i == FIELD_SIZE - 1) || is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                    cout << "|";
                }
                else {
                    cout << "_";
                }

                continue;
            }
            if ((p.shoots[row_num][i] && enemy.own_field[row_num][i] && (!p.shoots[row_num + 1][i] || !enemy.own_field[row_num + 1][i]))
                || (p.shoots[row_num + 1][i] && enemy.own_field[row_num + 1][i] && (!p.shoots[row_num][i] || !enemy.own_field[row_num][i]))) {
                cout << "_____";
                if (is_need_divider_on_enemy_field(p, enemy, row_num, i)) {
                    cout << "|";
                }
                else if ((p.shoots[row_num][i] && p.shoots[row_num][i + 1] && enemy.own_field[row_num][i] && enemy.own_field[row_num][i + 1])
                    || (p.shoots[row_num + 1][i] && p.shoots[row_num + 1][i + 1] && enemy.own_field[row_num + 1][i] && enemy.own_field[row_num + 1][i + 1])) {
                    cout << "_";
                }
                else {
                    cout << " ";
                }
            }
            else {
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
                    cout << EMPTY_HOR_CELL;
                }
            }
            else {
                if (enemy.shoots[row_num][i]) {
                    cout << "  *  ";
                }
                else {
                    cout << EMPTY_HOR_CELL;
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
    cout << EMPTY_HOR_CELL;
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
    char input_var;
    while (true) {
        system("cls");
        cout << "\t\tМорской бой." << endl;
        cout << "1. Легкий" << (computer_mode == 0 ? ".\t+" : ".") << endl;
        cout << "2. Сложный" << (computer_mode == 1 ? ".\t+" : ".") << endl;
        cout << "0. Назад." << endl;
        input_var = _getch();
        if (input_var == '0') break;
        switch (input_var) {
            case '1':
                computer_mode = 0;
                break;
            case '2':
                computer_mode = 1;
                break;
            default:
                break;
        }
    }

    show_menu();
}

bool is_player_has_ships(Player p) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (p.own_field[i][j] && !p.shoots[i][j]) return true;
        }
    }
    return false;
}

// Определение конца игры можно было бы реализовать с помощью функции is_player_has_ships,
// но эта функция вызывается очень часто (каждый ход) и она должна быть быстрой: за один 
// проход проверяются поля обоих игроков.
bool is_game_over(Player p1, Player p2) {
    bool p1_has_ships = false, p2_has_ships = false;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (p1.own_field[i][j] && !p2.shoots[i][j]) {
                p1_has_ships = true;
            }
            if (p2.own_field[i][j] && !p1.shoots[i][j]) {
                p2_has_ships = true;
            }
            if (p1_has_ships && p2_has_ships) return false;
        }
    }
    return true;
}

Point input_coords_to_point(char* input) {
    Point point = { -1, -1 };
    int y;
    for (int i = 0; i < FIELD_SIZE; i++) {
        if (SYMBOLS_CODES[i][1] == (int)input[0]) point.x = i;
    }
    if (point.x == -1) return { -1, -1 };
    y = input[1] - '0';
    if (y == 1 && input[2] == '0') y = 10;
    if (y < 1 || y > FIELD_SIZE || (y < 10 && input[2] != '\0') || (y == 10 && input[3] != '\0')) return { -1, -1 };
    point.y = y - 1;
    return point;
}

Point get_random_shoot(Player p) {
    Point point;
    do {
        point.x = rand() % 10;
        point.y = rand() % 10;
    } while (p.shoots[point.y][point.x]);
    return point;
}

// Анализ полей и поиск координат, где вероятней всего располагается палуба какого-либо вражеского корабля.
Point smart_shoot(Player comp, Player enemy) {
    int multiplier_direction, counter;
    // Этап 1. Поиск уже раненого корабля.
    Point deck_of_injured_ship;
    bool injured_ship_found = false;
    for (int i = 0; i < FIELD_SIZE && !injured_ship_found; i++) {
        for (int j = 0; j < FIELD_SIZE && !injured_ship_found; j++) {
            if (enemy.own_field[i][j] && comp.shoots[i][j] && !is_ship_destroyed(enemy, comp, { j,i })) {
                deck_of_injured_ship.x = j;
                deck_of_injured_ship.y = i;
                injured_ship_found = true;
            }
        }
    }
    // Если найден подбитый корабль
    if (injured_ship_found) {
        // Попытка найти вторую подбитую палубу для определения направления корабля.
        // Если нестреляных полей больше одного - выбираем случайным образом.
        if ((deck_of_injured_ship.x > 0 && comp.shoots[deck_of_injured_ship.y][deck_of_injured_ship.x - 1] && enemy.own_field[deck_of_injured_ship.y][deck_of_injured_ship.x - 1])
            || (deck_of_injured_ship.x < FIELD_SIZE - 1 && comp.shoots[deck_of_injured_ship.y][deck_of_injured_ship.x + 1] && enemy.own_field[deck_of_injured_ship.y][deck_of_injured_ship.x + 1])) {
            // Горизонтальное расположение корабля
            multiplier_direction = rand() % 2 == 0 ? -1 : 1;
            counter = deck_of_injured_ship.x + (1 * multiplier_direction);
            while(true) {
                if (counter < 0
                    || counter == FIELD_SIZE 
                    || (comp.shoots[deck_of_injured_ship.y][counter] && !enemy.own_field[deck_of_injured_ship.y][counter])) {
                    // Если дошли до края поля или в это поле уже стреляли и промахнулись - меняем направление
                    multiplier_direction *= -1;
                    counter = deck_of_injured_ship.x + (1 * multiplier_direction);
                }
                if (!comp.shoots[deck_of_injured_ship.y][counter]) {
                    return { counter, deck_of_injured_ship.y };
                }
                else {
                    counter += (1 * multiplier_direction);
                }
            };
        }
        else if ((deck_of_injured_ship.y > 0 && comp.shoots[deck_of_injured_ship.y - 1][deck_of_injured_ship.x] && enemy.own_field[deck_of_injured_ship.y - 1][deck_of_injured_ship.x])
            || (deck_of_injured_ship.y < FIELD_SIZE - 1 && comp.shoots[deck_of_injured_ship.y + 1][deck_of_injured_ship.x] && enemy.own_field[deck_of_injured_ship.y + 1][deck_of_injured_ship.x])) {
            // Вертикальное расположение корабля
            multiplier_direction = rand() % 2 == 0 ? -1 : 1;
            counter = deck_of_injured_ship.y + (1 * multiplier_direction);
            while (true) {
                if (counter < 0
                    || counter == FIELD_SIZE
                    || (comp.shoots[counter][deck_of_injured_ship.x] && !enemy.own_field[counter][deck_of_injured_ship.x])) {
                    // Если дошли до края поля или в это поле уже стреляли и промахнулись - меняем направление
                    multiplier_direction *= -1;
                    counter = deck_of_injured_ship.y + (1 * multiplier_direction);
                }
                if (!comp.shoots[counter][deck_of_injured_ship.x]) {
                    return { deck_of_injured_ship.x, counter };
                }
                else {
                    counter += (1 * multiplier_direction);
                }
            };
        }
        else {
            // Подбита только одна палуба
            // 0 - верх, 1 - право, 2 - низ, 3 - лево
            multiplier_direction = rand() % 4;
            while (true) {
                if (multiplier_direction == 0)
                {
                    if (deck_of_injured_ship.y > 0 && !comp.shoots[deck_of_injured_ship.y - 1][deck_of_injured_ship.x]) {
                        // Если направление вверх и в этом направлении есть нестреляная клетка - возвращаем ее
                        return { deck_of_injured_ship.x, deck_of_injured_ship.y - 1 };
                    }
                    // иначе - меняем направление по часовой стрелке и продолжаем искать
                    multiplier_direction++;
                }

                if (multiplier_direction == 1)
                {
                    if (deck_of_injured_ship.x < FIELD_SIZE - 1 && !comp.shoots[deck_of_injured_ship.y][deck_of_injured_ship.x + 1]) {
                        // Если направление вправо и в этом направлении есть нестреляная клетка - возвращаем ее
                        return { deck_of_injured_ship.x + 1, deck_of_injured_ship.y };
                    }
                    // иначе - меняем направление по часовой стрелке и продолжаем искать
                    multiplier_direction++;
                }

                if (multiplier_direction == 2)
                {
                    if (deck_of_injured_ship.y < FIELD_SIZE - 1 && !comp.shoots[deck_of_injured_ship.y + 1][deck_of_injured_ship.x]) {
                        // Если направление вниз и в этом направлении есть нестреляная клетка - возвращаем ее
                        return { deck_of_injured_ship.x, deck_of_injured_ship.y + 1 };
                    }
                    // иначе - меняем направление по часовой стрелке и продолжаем искать
                    multiplier_direction++;
                }

                if (multiplier_direction == 3)
                {
                    if (deck_of_injured_ship.x > 0 && !comp.shoots[deck_of_injured_ship.y][deck_of_injured_ship.x - 1]) {
                        // Если направление вправо и в этом направлении есть нестреляная клетка - возвращаем ее
                        return { deck_of_injured_ship.x - 1, deck_of_injured_ship.y };
                    }
                    // иначе - меняем направление по часовой стрелке и продолжаем искать
                    multiplier_direction = 0;
                }
            }
        }
    }

    return get_random_shoot(comp);
}

Point get_shoot_point(Player p, Player enemy) {
    Point point;
    char input_point[4];
    
    if (p.is_comp) {
        if (computer_mode == 0) {
            point = get_random_shoot(p);
        }
        else {
            point = smart_shoot(p, enemy);
        }
    }
    else {
        while (true) {
            cout << "Введите координаты выстрела: ";
            cin >> input_point;
            point = input_coords_to_point(input_point);
            if (point.x == -1) cout << "Недопустимые координаты. Попробуйте еще раз." << endl;
            else break;
        }
    }
    return point;
}

bool is_ship_destroyed(Player player, Player enemy, Point point) {
    // Чтобы выяснить уничтожен ли корабль, ищем в 4 направлениях живые палубы
    // Разработаны 2 алгоритма

    // Первый алгоритм: смотрим сразу в 4 направления и ищем живые палубы.
    // Негативный момент: приходится проверять много лишних пустых клеток.
    //int x_west, x_east, y_north, y_south;

    //for (int i = 1; i < 4; i++) {
    //    x_east = point.x + i;
    //    x_west = point.x - i;
    //    y_north = point.y + i;
    //    y_south = point.y - i;
    //    if ((x_east < FIELD_SIZE && player.own_field[x_east][point.y] && !enemy.shoots[x_east][point.y])
    //        || (x_west > 0 && player.own_field[point.x - i][point.y] && !enemy.shoots[point.x - i][point.y])
    //        || (y_north > 0 && player.own_field[point.x][point.y + i] && !enemy.shoots[point.x][point.y + i])
    //        || (y_south < FIELD_SIZE && player.own_field[point.x][point.y - i] && !enemy.shoots[point.x][point.y - i])) return false;
    //}

    // Второй алгоритм: поочереди смотрим в разные стороны
    // По сравнению с первым алгоритмом каждый цикл будет заканчиваться на пустой клетке или на границе поля.
    int counter = point.x - 1;
    while (counter >= 0 && counter < FIELD_SIZE && player.own_field[point.y][counter]) {
        if (!enemy.shoots[point.y][counter--]) return false;
    }
    counter = point.x + 1;
    while (counter >= 0 && counter < FIELD_SIZE && player.own_field[point.y][counter]) {
        if (!enemy.shoots[point.y][counter++]) return false;
    }
    counter = point.y + 1;
    while (counter >= 0 && counter < FIELD_SIZE && player.own_field[counter][point.x]) {
        if (!enemy.shoots[counter++][point.x]) return false;
    }
    counter = point.y - 1;
    while (counter >= 0 && counter < FIELD_SIZE && player.own_field[counter][point.x]) {
        if (!enemy.shoots[counter--][point.x]) return false;
    }
    return true;
}

// Функция делает "залп воинских почестей", снаряды которого падают в каждую клетку вокруг
// погибшего корабля. Подразумевается, что в клетке point находится последняя подбитая палуба
// корабля.
void military_honors(Player &player, Player enemy, Point point) {
    Point first_deck = point, last_deck = point;
    int counter = point.x - 1;
    while (counter >= 0 && counter < FIELD_SIZE && enemy.own_field[point.y][counter]) {
        counter--;
        first_deck.x--;
    }
    counter = point.x + 1;
    while (counter >= 0 && counter < FIELD_SIZE && enemy.own_field[point.y][counter]) {
        counter++;
        last_deck.x++;
    }
    counter = point.y + 1;
    while (counter >= 0 && counter < FIELD_SIZE && enemy.own_field[counter][point.x]) {
        counter++;
        last_deck.y++;
    }
    counter = point.y - 1;
    while (counter >= 0 && counter < FIELD_SIZE && enemy.own_field[counter][point.x]) {
        counter--;
        first_deck.y--;
    }

    // Корректировка координат залпа для более удобного и безопасного цикла.
    if (first_deck.x > 0) first_deck.x--;
    if (first_deck.y > 0) first_deck.y--;
    if (last_deck.x < FIELD_SIZE) last_deck.x++;
    if (last_deck.y < FIELD_SIZE) last_deck.y++;

    for (int i = first_deck.y; i <= last_deck.y; i++) {
        for (int j = first_deck.x; j <= last_deck.x; j++) {
            if (i < 0 || i >= FIELD_SIZE || j < 0 || j >= FIELD_SIZE) {
                continue;
            }
            player.shoots[i][j] = true;
        }
    }
}

void run_game(Player p1, Player p2) {
    char input;
    Player* current_p = &p1, *enemy = &p2, *tmp;
    bool is_destroyed;
    while (!is_game_over(p1, p2)) {
        // Если играет человек, мы должны показать ему поле перед ходом
        if (game_mode == 0 && !(*current_p).is_comp) {
            show_field(p1, p2);
        }
        Point shoot = get_shoot_point(*current_p, *enemy);
        (*current_p).shoots[shoot.y][shoot.x] = true;
        // Показываем поле после выстрела
        // Если играют 2 компьютера, чередуем отображение полей, чтобы видно было поля обоих компьютеров.
        // Если играет человек, показываем только поля игрока.
        if (game_mode == 0) {
            show_field(p1, p2);
        }
        else {
            show_field(*current_p, *enemy);
        }
        cout << "Ходит " << (*current_p).name << endl;
        cout << "Выстрел по координатам: " << (char)SYMBOLS_CODES[shoot.x][0] << shoot.y + 1 << endl;
        if ((*enemy).own_field[shoot.y][shoot.x]) {
            is_destroyed = is_ship_destroyed(*enemy, *current_p, shoot);
            cout << "Попадание: корабль " << (is_destroyed ? "уничтожен" : "ранен");
            // Сложный компьютер окружает уничтоженные корабли полями с состоянием "мимо",
            // чтобы в будущем не тратить ход на гарантированный промах
            if (is_destroyed && (*current_p).is_comp && computer_mode == 1) military_honors(*current_p, *enemy, shoot);
        }
        else {
            cout << "Промах";
            tmp = current_p;
            current_p = enemy;
            enemy = tmp;
        }

        if (!is_game_over(p1, p2)) {
            cout << endl << "Нажмите любую клавишу чтобы продолжить или Q для возврата в главное меню...";
        }
        else break;
        input = _getch();
        if (input == 'Q' || input == 'q') return;
    }

    cout << endl << "Игра окончена. Победил " << (is_player_has_ships(*current_p) ? (*current_p).name : (*enemy).name)
        << endl
        << "Нажмите любую клавишу для возврата в главное меню...";
    _getch();
}

void run_human_vs_comp() {
    char is_automatic_fill, tmp_input;
    Player p1 = { "Игрок" };
    Player p2 = { "Компьютер", true };
    game_mode = 0;
    char input_point[3];
    Point point;
    bool is_horizontal_available, is_vertical_available;
    int ships;

    do {
        cout << endl << "Заполнить поле автоматически (Y/N): ";
        is_automatic_fill = _getch();
    } while (is_automatic_fill != 'Y' && is_automatic_fill != 'y' && is_automatic_fill != 'N' && is_automatic_fill != 'n');
    if (is_automatic_fill == 'Y' || is_automatic_fill == 'y') {
        set_ships_auto(p1);
    }
    else {

        for (int i = 0; i < 4; i++) {
            ships = 4 - i;
            do {
                show_field(p1, p2);
                cout << "*Необходимо ввести координаты \"главной\" палубы, а затем выбрать куда будут направлены остальные палубы: вправо или вниз." << endl; 
                cout << "Введите координаты " << i + 1 << "-палубного корабля: ";
                cin >> input_point;
                point = input_coords_to_point(input_point);
                if (point.x == -1) {
                    cout << "Недопустимые координаты. Попробуйте еще раз." << endl;
                    continue;
                }

                // Для однопалубных кораблей нет необходимости выбирать горизонтальное или вертикальное направление.
                if (i > 0) {
                    // Проверяем возможность горизонтального расположения:
                    is_horizontal_available = is_ship_coords_valid(p1.own_field, point, { point.x + i, point.y });
                    // Проверяем возможность вертикального расположения:
                    is_vertical_available = is_ship_coords_valid(p1.own_field, point, { point.x, point.y + i });
                    // Если возможны оба варианта - предоставляем право выбора пользователю
                    if (is_horizontal_available && is_vertical_available) {
                        
                            cout << "В какую сторону расположить корабль?" << endl
                                << "1. Вправо." << endl
                                << "2. Вниз." << endl;
                        do {
                            tmp_input = _getch();
                        } while (tmp_input != '1' && tmp_input != '2');
                        if (tmp_input == '1') {
                            for (int j = 0; j <= i; j++) p1.own_field[point.y][point.x + j] = true;
                        }
                        else {
                            for (int j = 0; j <= i; j++) p1.own_field[point.y + j][point.x] = true;
                        }
                    }
                    else if (is_horizontal_available) {
                        for (int j = 0; j <= i; j++) p1.own_field[point.y][point.x + j] = true;
                    }
                    else if (is_vertical_available) {
                        for (int j = 0; j <= i; j++) p1.own_field[point.y + j][point.x] = true;
                    }
                    else {
                        cout << "По этим координатам невозможно поставить корабль. Попробуйте еще раз." << endl;
                        _getch();
                        continue;
                    }
                    
                }
                else {
                    if (!is_ship_coords_valid(p1.own_field, point, point)) {
                        cout << "По этим координатам невозможно поставить корабль. Попробуйте еще раз." << endl;
                        _getch();
                        continue;
                    }
                    p1.own_field[point.y][point.x] = true;
                }

                ships--;
            } while (ships > 0);
        }
    }
    
    set_ships_auto(p2);
    run_game(p1, p2);
}

void run_comp_vs_comp() {
    Player p1 = { "Компьютер1", true };
    Player p2 = { "Компьютер2", true };
    game_mode = 1;
    set_ships_auto(p1);
    set_ships_auto(p2);
    run_game(p1, p2);
}

void show_menu() {
    char input_var;
    while (true) {
        system("cls");
        cout << "\tМорской бой." << endl;
        cout << "1. Человек против компьютера." << endl;
        cout << "2. Компьютер против компьютера." << endl;
        cout << "3. Установить сложность компьютера." << endl;
        cout << "0. Выход." << endl;
        input_var = _getch();
        if (input_var == '0') {
            exit_game = true;
            return;
        }
        switch (input_var) {
            case '1':
                run_human_vs_comp();
                break;
            case '2':
                run_comp_vs_comp();
                break;
            case '3':
                show_computer_difficulty_dialog();
                break;
            default:
                break;
        }
    }
}

bool is_ship_coords_valid(bool field[FIELD_SIZE][FIELD_SIZE], Point p1, Point p2) {
    if (p2.x < 0 || p2.x >= FIELD_SIZE
        || p2.y < 0 || p2.y >= FIELD_SIZE) return false;
    
    for (int i = p1.x - 1; i <= p2.x + 1; i++) {
        if (i < 0 || i >= FIELD_SIZE) continue;
        for (int j = p1.y - 1; j <= p2.y + 1; j++) {
            if (j < 0 || j >= FIELD_SIZE) continue;
            if (field[j][i]) return false;
        }
    }
    return true;
}

void set_ships_auto(Player &p) {
    // cells_count_sum служит для вычисления кол-ва кораблей (например, 
    // 1 единица 4-палубных кораблей = 4 + 1 = 5, 2 единицы 3-палубных = 2 + 3 = 5 и т.д.)
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

                p1.x = x;
                p1.y = y;
                if (dir == 0) {
                    p2.x = x + i - 1;
                    p2.y = y;
                }
                else {
                    p2.x = x;
                    p2.y = y + i - 1;
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

//void test_data() {
//    for (int i = 0; i < FIELD_SIZE; i++) {
//        for (int j = 0; j < FIELD_SIZE; j++) {
//            if (rand() % 2) {
//                player2.shoots[i][j] = true;
//            }
//            else {
//                player2.shoots[i][j] = false;
//            }
//
//            if (rand() % 2) {
//                player1.shoots[i][j] = true;
//            }
//            else {
//                player1.shoots[i][j] = false;
//            }
//        }
//    }
//}




int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1200, 900, TRUE);

    do {
        show_menu();
    } while (!exit_game);
    


    //test_data();
    //set_ships_auto(player1);
    //set_ships_auto(player2);
    
    //show_field(player1, player2);
    //show_arr(player1);
}