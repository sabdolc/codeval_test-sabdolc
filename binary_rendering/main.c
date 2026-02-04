#include <iostream>
#include <string>
#include <filesystem>

#include "place_boats.h"
#include "save_game.h"
#include "hits_misses.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "USAGE: ./binary_saving <name> [save_file] [-l]" << std::endl;
        return 1;
    }

    std::string filename;

    for (int i = 2; i < argc; i++) {
        std::string a = argv[i];
        if (a == "-l") lazy_mode = true;
        else filename = a;
    }

    if (filename.empty()) {
    std::cout << "Enter save file name: " << std::endl;
    if (!(std::cin >> filename)) return 0;
    }

    current_save_file = filename;

    bool file_exists = std::filesystem::exists(current_save_file);

    set_boatsizes();

    if(file_exists) {
        if (!load_boats_from_file(current_save_file)) {
            std::cerr << "Failed to load game" << std::endl;
            return 3;
        }
        loading_game = true;
        lazy_mode = false;
        place_boats();
        if (!load_shots_from_file(current_save_file)) {
            std::cerr << "Failed to load shots" << std::endl;
            return 4;
        }
        loading_game = false;
        print_yourboard();
        print_enemyboard();
        play_game();
    }
    else {
        reset_boat_save_file(current_save_file);
        place_boats();
        play_game();
    }

    return 0;
}