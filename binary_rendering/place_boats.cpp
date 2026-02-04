#include <iostream>
#include <array>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "save_game.h"
#include "place_boats.h"

// ANSI escape codes
const std::string RESET = "\x1b[0m";
const std::string BOLD = "\x1b[1m";
const std::string BLUE = "\x1b[34m";
const std::string CYAN = "\x1b[36m";
const std::string YELLOW = "\x1b[33m";
const std::string GREEN = "\x1b[32m";
const std::string RED = "\x1b[31m";
const std::string BG_BLUE = "\x1b[44m";

static const std::string SHIP = "🚢"; // ship

bool loading_game = false; 
bool lazy_mode = false;

// global array storage
std::array<std::array<std::string, 10>, 10> player =
    {{{"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}}};

std::array<std::array<std::string, 10>, 10> enemy =
    {{{"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
      {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}}};

static int row_to_index(char row)
{
    return static_cast<int>(std::toupper(static_cast<unsigned char>(row)) - 'A');
}

static bool parse_A1_token(const std::string& tok, int& r, int& c)
{
    if (tok.size() < 2) return false;

    char row = tok[0];
    row = static_cast<char>(std::toupper(static_cast<unsigned char>(row)));
    if (row < 'A' || row > 'J') return false;

    int col = 0;
    for (size_t i = 1; i < tok.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(tok[i]))) return false;
        col = col * 10 + (tok[i] - '0');
    }
    if (col < 1 || col > 10) return false;

    r = row - 'A';
    c = col - 1;
    return true;
}

static bool read_pos_A1_or_A_1(int& r, int& c)
{
    std::string tok;
    if (!(std::cin >> tok)) return false;

    if (tok.size() == 1 && std::isalpha(static_cast<unsigned char>(tok[0]))) {
        int col;
        if (!(std::cin >> col)) return false;

        std::string merged;
        merged.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(tok[0]))));
        merged += std::to_string(col);
        return parse_A1_token(merged, r, c);
    }

    return parse_A1_token(tok, r, c);
}

bool validBoatLocation(char way, char row, int column, int size)
{
    if ((way != 'V' && way != 'H') || (row < 'A' || row > 'J') || (column < 0 || column >= 10))
    {

        return false;
    }
    else
    {
        // Convert the row letter to the equivalent letter value
        int rowIndex = row - 'A';
        if ((column + size > 10) && (way == 'H'))
        {
            if (!lazy_mode && !loading_game) {
                std::cout << "Not enough space horizontally for boat" << std::endl;
            }
            return false;
        }
        else if ((rowIndex + size > 10) && (way == 'V'))
        {
            if (!lazy_mode && !loading_game) {
                std::cout << "Not enough space vertically for boat" << std::endl;
            }
            return false;
        }
    }
    return true;
}

int load_board()
{
    int loaded_count = 0;
    for (const auto &b : boats_array)
    {
        // We assume a boat is valid if its direction is set
        if (b.direction == 'H' || b.direction == 'V')
        {
            int rowIndex = b.row - 'A';
            for (int s = 0; s < b.size; ++s)
            {
                if (b.direction == 'H')
                {
                    player[rowIndex][b.column + s] = SHIP;
                    boats_array[loaded_count].positions[s] = {rowIndex, b.column + s};
                }
                else
                {
                    player[rowIndex + s][b.column] = SHIP;
                    boats_array[loaded_count].positions[s] = {rowIndex + s, b.column};
                }
            }
            loaded_count++;
        }
    }
    return loaded_count;
}
void print_yourboard()
{
    std::cout << "Your Fleet" << std::endl;
    std::cout << BOLD << YELLOW
              << "0 1 2 3 4 5 6 7 8 9 10"
              << RESET << '\n';
    for (int i = 0; i < 10; i++)
    {
        std::cout << BOLD << YELLOW << static_cast<char>(i + 'A') << " " << RESET;
        for (int j = 0; j < 10; j++)
        {
            std::cout << BG_BLUE << player[i][j] << BG_BLUE;
        }
        std::cout << RESET << std::endl;
    }
}
void print_enemyboard()
{
    std::cout << "Enemy Waters" << std::endl;
    std::cout << BOLD << RED
              << "0 1 2 3 4 5 6 7 8 9 10"
              << RESET << '\n';
    for (int i = 0; i < 10; i++)
    {
        std::cout << BOLD << RED << static_cast<char>(i + 'A') << " " << RESET;
        for (int j = 0; j < 10; j++)
        {
            if (enemy[i][j] == "  ") {
                std::cout << BG_BLUE << "  " << RESET;
            } else {
                std::cout << BG_BLUE << enemy[i][j] << RESET;
            }
        }
        std::cout << std::endl;
    }
}

static bool can_place(char way, int rowIndex, int column, int size) {
    if (way == 'H') {
        for (int f = -1; f <= 1; f++) {
            for (int x = -1; x <= size; x++) {
                int rr = rowIndex + f;
                int cc = column + x;
                if (rr >= 0 && rr < 10 && cc >= 0 && cc < 10) {
                    if (player[rr][cc] == SHIP) return false;
                }
            }
        }
        return true;
    }
    else {
        for (int f = -1; f <= 1; f++) {
            for (int x = -1; x <= size; x++) {
                int rr = rowIndex + x;
                int cc = column + f;
                if (rr >= 0 && rr < 10 && cc >= 0 && cc < 10) {
                    if (player[rr][cc] == SHIP) return false;
                }
            }
        }
        return true;
    }
}

static void do_place_boat(char way, char row, int col0, int size, int i) {
    int rowIndex = row - 'A';

    boat b{size, way, row, col0};
    boats_array[i] = b;
    save_boat(b, current_save_file);

    if (way == 'H') {
        for (int x = 0; x < size; x++) {
            player[rowIndex][col0+x] = SHIP;
            boats_array[i].positions[x] = {rowIndex, col0 + x};
        }
    }
    else {
        for (int x = 0; x < size; x++) {
            player[rowIndex+x][col0] = SHIP;
            boats_array[i].positions[x] = {rowIndex + x, col0};
        }
    }
}

void place_boats()
{
    // The maximum boat size is 5 and the minimum size is 3. Boats cannot be overlapped or placed next to each other.
    // Format => V/H row col ex: V A 9
    // Rows: A - J, Columns: 0 - 9

    // If boats were loaded from a file, draw them onto the 'player' grid now.
    int i = load_board();

    if (i < 5)
    {
        std::cout << "Start by placing your remaining " << (5 - i) << " boats on the board." << std::endl;
    }
    else
    {
        std::cout << "All boats and shots loaded from file! Displaying your fleet:" << std::endl;
    }

    if (lazy_mode && i < 5 && !loading_game) {
        std::srand((unsigned)std::time(nullptr));

        while (i < 5) {
            int size = boats_array[i].size;
            char way = (std::rand() % 2 == 0) ? 'H' : 'V';
            int rowIndex = std::rand() % 10;
            int col0 = std::rand() % 10;

            char rowChar = char('A' + rowIndex);

            if (!validBoatLocation(way, rowChar, col0, size)) continue;
            if (!can_place(way, rowIndex, col0, size)) continue;

            do_place_boat(way, rowChar, col0, size, i);
            i++;
        }

        std::cout << "Lazy placement complete!\n";
        print_yourboard();
        print_enemyboard();
        std::cout << "Let the Games Begin!\n";
        return;
    }

    while (i < 5)
    {
        print_yourboard();

        char way;
        int rowIndex = -1;
        int col0 = -1;
        int size = boats_array[i].size;

        std::cout << "Boat " << i + 1 << "/5 Pick a location to place a boat of size " << size << ": ";

        // replacement input block for assignment_6/place_boats.cpp
        std::string pos;
        if (!(std::cin >> way >> pos)) {
            std::cout << "Invalid input. Expected format: H A1\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        way = static_cast<char>(std::toupper(static_cast<unsigned char>(way)));

        int r = -1, c = -1;
        int col_num = 0;
        char rowChar = 0;

        // Accept "H A 3" -> pos == "A"
        if (pos.size() == 1 && std::isalpha(static_cast<unsigned char>(pos[0]))) {
            if (!(std::cin >> col_num)) {
                std::cout << "Invalid input. Expected format: H A1\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            rowChar = static_cast<char>(std::toupper(static_cast<unsigned char>(pos[0])));
            r = row_to_index(rowChar);
            c = col_num - 1;
        }
        // Accept "H A3" -> pos == "A3"
        else {
            if (!parse_A1_token(pos, r, c)) {
                std::cout << "Invalid input. Expected format: H A1\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            rowChar = static_cast<char>('A' + r);
        }

        // map into the variables the rest of the code expects
        rowIndex = r;
        col0 = c;

        // Validate placement using the helpers that expect (char row, int column)
        if (!validBoatLocation(way, rowChar, col0, size)) {
            std::cout << "Invalid location. Try again.\n";
            continue;
        }

        if (!can_place(way, rowIndex, col0, size)) {
            std::cout << "Spot taken or too close to another ship, try again\n";
            continue;
        }

        do_place_boat(way, rowChar, col0, size, i);
        i++;
    }

    if (!loading_game) {
        std::cout << "Done with board setup" << std::endl;
        print_yourboard();
        print_enemyboard();
        std::cout << "Let the Games Begin!" << std::endl;
    }
}
