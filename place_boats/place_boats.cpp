#include <iostream>
#include <array>
#include <limits>

#include "boats.hpp"

void print_grid(const std::array<std::array<std::string, 10>, 10>& grid, const std::string& title, const std::string& color)
{
    std::cout << title << std::endl;
    std::cout << BOLD << color
              << "  1 2 3 4 5 6 7 8 9 10"
              << RESET << '\n';
    for (int k = 0; k < 10; k++)
    {
        std::cout << BOLD << color << static_cast<char>(k + 'A') << " " << RESET;
        for (int j = 0; j < 10; j++)
        {
            std::cout << BG_BLUE << grid[k][j] << BG_BLUE;
        }
        std::cout << RESET << std::endl;
    }
}

void place_boats()
{
    std::array<std::array<std::string, 10>, 10> player;
    for (auto& row : player)
        for (auto& cell : row)
            cell = "  ";

    const std::string names[] = {"Carrier", "Battleship", "Destroyer", "Submarine", "Patrol"};
    const int sizes[] = {5, 4, 3, 3, 2};

    std::cout << "Start by placing your 5 boats on the board." << std::endl;
    print_grid(player, "Your Fleet", YELLOW);

    int i = 0;
    while (i < 5)
    {
        std::cout << names[i] << "(" << sizes[i] << ") Pick a location to place a boat: ";
        char way;
        std::string position;

        // ---- Direction ----
        std::cin >> way;
        if (way != 'H' && way != 'V') {
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // ---- Position ----
        std::cin >> position;
        if (position.size() < 2 || !std::isalpha(position[0])) {
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        char row = std::toupper(position[0]);
        int column = 0;
        for (size_t k = 1; k < position.size(); k++) {
            if (!std::isdigit(position[k])) {
                column = -1;
                break;
            }
            column = column * 10 + (position[k] - '0');
        }

        if (column < 1 || column > 10) {
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        column--; // convert to 0-based index

        // ---- Row bounds ----
        if (row < 'A' || row > 'J') {
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        int rowIndex = row - 'A';
        int size = sizes[i];

        // ---- Board bounds ----
        if (way == 'H' && column + size > 10) {
            std::cout << "Not enough space horizontally for boat" << std::endl;
            continue;
        }
        if (way == 'V' && rowIndex + size > 10) {
            std::cout << "Not enough space vertically for boat" << std::endl;
            continue;
        }

        // ---- Overlap / adjacency check ----
        bool canPlace = true;
        if (way == 'H') {
            for (int f = -1; f <= 1 && canPlace; f++)
            {
                for (int x = -1; x <= size && canPlace; x++)
                {
                    int checkRow = rowIndex + f;
                    int checkCol = column + x;
                    if (checkRow >= 0 && checkRow < 10 && checkCol >= 0 && checkCol < 10)
                    {
                        if (player[checkRow][checkCol] == SHIP)
                        {
                            std::cout << "Spot taken or too close to another ship, try again" << std::endl;
                            canPlace = false;
                        }
                    }
                }
            }
        } else {
            for (int f = -1; f <= 1 && canPlace; f++)
            {
                for (int x = -1; x <= size && canPlace; x++)
                {
                    int checkRow = rowIndex + x;
                    int checkCol = column + f;
                    if (checkRow >= 0 && checkRow < 10 && checkCol >= 0 && checkCol < 10)
                    {
                        if (player[checkRow][checkCol] == SHIP)
                        {
                            std::cout << "Spot taken or too close to another ship, try again" << std::endl;
                            canPlace = false;
                        }
                    }
                }
            }
        }

        if (canPlace)
        {
            std::cout << "A boat of size " << size << " will be placed at " << row << (column + 1)
                      << (way == 'H' ? " horizontally" : " vertically") << std::endl;
            if (way == 'H') {
                for (int x = 0; x < size; x++)
                    player[rowIndex][column + x] = SHIP;
            } else {
                for (int x = 0; x < size; x++)
                    player[rowIndex + x][column] = SHIP;
            }
            i++;
            print_grid(player, "Your Fleet", YELLOW);
        }
    }

    std::cout << "Done with board setup" << std::endl;
    std::cout << "Let the Games Begin!" << std::endl;
    print_grid(player, "Your Fleet", YELLOW);

    std::array<std::array<std::string, 10>, 10> enemy;
    for (auto& row : enemy)
        for (auto& cell : row)
            cell = "  ";
    print_grid(enemy, "Enemy Waters", RED);
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "USAGE: ./place_boats <name>" << std::endl;
        return 1;
    }

    std::cout << "Hello " << argv[1] << ". Do you want to play a game?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if (answer == "yes" || answer == "Yes")
    {
        place_boats();
    }
    else if (answer == "no" || answer == "No")
    {
        std::cout << "Run me when you are ready!" << std::endl;
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
        return 2;
    }

    return 0;
}