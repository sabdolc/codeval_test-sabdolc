#include <iostream>
#include <fstream>
#include <array>
#include "save_game.h"
#include "binary_rendering.h"

// Global storage for the current game's boats.
std::array<boat, 5> boats_array{};

std::string current_save_file = "boats.txt";

bool using_binary_save = false;

static char next_turn_ascii = 'P';

extern std::array<std::array<std::string, 10>, 10> player;
extern std::array<std::array<std::string, 10>, 10> enemy;

std::string boat_to_string(const boat& b) {
    return std::to_string(b.size) + " " + std::string{b.direction} + " " + std::string{b.row} + " " + std::to_string(b.column) + " ";
}

void set_boatsizes()
{
    boats_array[0].size = 2;
    boats_array[1].size = 3;
    boats_array[2].size = 3;
    boats_array[3].size = 4;
    boats_array[4].size = 5;
}

void reset_boat_save_file(const std::string &filename)
{
    using_binary_save = true;
    binaryformat::write_binary_header(filename);
}

void save_boat(const boat &b, const std::string &filename)
{
    if (using_binary_save) {
        binaryformat::save_boat_binary(b, filename);
        return;
    }
    
    // Append a single boat each time it is created.
    std::ofstream file(filename, std::ios::app);
    if (!file)
    {
        std::cerr << "Error: could not open '" << filename << "' for appending.\n";
        return;
    }

    file << boat_to_string(b) << "\n";
}

bool load_boats_from_file(const std::string &filename)
{
    if (binaryformat::is_binary_save(filename)) {
        using_binary_save = true;
        return binaryformat::load_game_binary(filename);
    }
    
    using_binary_save = false;

    std::ifstream file{filename};

    // Check if the file opened successfully
    if (!file)
    {
        return false;
    }

    std::string line;
    std::getline(file, line);

    int i = 0;
    char dir, r;
    int col, siz;

    while (file >>siz >>dir >> r >> col && i < 5)
    {
        boats_array[i].size = siz;
        boats_array[i].direction = dir;
        boats_array[i].row = r;
        boats_array[i].column = col;
        i++;
    }

    for (int j = i; j < 5; j++) {
        boats_array[j].direction = 0;
        boats_array[j].row = 0;
        boats_array[j].column = 0;
    }

    std::cout << "Successfully loaded boats and shots from " << filename << std::endl;
    return true;
}

void save_shots(char shooter, char row, int col, char result, const std::string& filename) {
    if (using_binary_save) {
        binaryformat::save_shot_binary(shooter, row, col, result, filename);
        return;
    }
    
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error: could not open '" << filename << "' for appending." << std::endl;
        return;
    }

    file << "SHOT " << shooter << " " << row << " " << col << " " << result << std::endl;
    next_turn_ascii = (shooter == 'P') ? 'E' : 'P';
}

bool load_shots_from_file(const std::string& filename) {
    if (using_binary_save) return binaryformat::replay_shots_binary(filename);
    
    char last_shooter = '\0';
    std::ifstream file(filename);
    if (!file) return false;

    std::string first;
    file >> first;
    if (!file) return false;

    std::string temp;
    std::getline(file, temp);

    for (int i = 0; i < 5; i++) {
        int size; 
        char dir;
        char row;
        int col;
        file >> size >> dir >> row >> col;
        if (!file) return false;
    }

    while (true) {
        std::string tag;
        file >> tag;
        if (!file) break;

        if (tag != "SHOT") {
            std::getline(file, temp);
            continue;
        }

        char shooter, row, result;
        int col;
        file >> shooter >> row >> col >> result;
        if (!file) break;

        last_shooter = shooter;

        if (row >= 'a' && row <= 'j') row = row - 'a' + 'A';

        int r = row - 'A';
        int c = col - 1;

        if (r < 0 || r > 9 || c < 0 || c > 9) continue;

        if (shooter == 'E') {
            if (result == 'H') player[r][c] = "💥";
            else if (result == 'M') player[r][c] = "🚀";
            else if (result == 'S') player[r][c] = "🔥";
        }
        else if (shooter == 'P') {
            if (result == 'H') enemy[r][c] = "💥";
            else if (result == 'M') enemy[r][c] = "🚀";
            else if (result == 'S') enemy[r][c] = "🔥";
        }
    }

    if (last_shooter == 'P') {
        next_turn_ascii = 'E';
    }
    else if (last_shooter == 'E') {
        next_turn_ascii = 'P';
    }
    else {
        next_turn_ascii = 'P';
    }

    return true;
}

char get_next_turn() {
    if (using_binary_save) return binaryformat::get_next_turn_binary();
    return next_turn_ascii;
}

