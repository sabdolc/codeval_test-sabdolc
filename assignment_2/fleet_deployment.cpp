#include <iostream>

// ANSI escape codes

const std::string RESET = "\x1b[0m";
const std::string BOLD = "\x1b[1m";
const std::string BLUE = "\x1b[34m";
const std::string CYAN = "\x1b[36m";
const std::string YELLOW = "\x1b[33m";
const std::string GREEN = "\x1b[32m";
const std::string RED = "\x1b[31m";
const std::string BG_BLUE = "\x1b[44m";

void validateBoatLocation(char way, char row, int column, int size)
{
    if ((way != 'V' && way != 'H') || (size > 5 || size < 2) || (row < 'A' || row > 'J') || (column <= 0 || column > 10))
    {
        {
            std::cout << "Invalid input" << std::endl;
            exit(2);
        }
    }
    else
    {
        // Convert the row letter to the equivalent letter value
        int rowIndex = row - 'A';
        if ((column + size > 10) && (way == 'H'))
        {
            std::cout << "Not enough space horizontally." << std::endl;
            exit(1);
        }
        else if ((rowIndex + size > 10) && (way == 'V'))
        {
            std::cout << "Not enough space vertically." << std::endl;
            exit(1);
        }
        else
        {
            std::cout << "A boat of size ";
            std::cout << size << " will be placed at " << row << column;
            if (way == 'V')
            {
                std::cout << " vertically." << std::endl;
            }
            else if (way == 'H')
            {
                std::cout << " horizontally." << std::endl;
            }
        }
    }
}
void startgame()
{
    // The maximum boat size is 5 and the minimum size is 2. Boats cannot be overlapped or placed next to each other. 
    std::cout << "Your Fleet" << std::endl;
    std::cout << BOLD << YELLOW
              << "0 1 2 3 4 5 6 7 8 9 10"
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "A " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "B " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "C " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "D " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "E " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "F " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "G " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "H " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "I " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';
    std::cout << BOLD << YELLOW << "J " << RESET << BG_BLUE
              << "                    "
              << RESET << '\n';

    std::cout << "Enemy Waters" << std::endl;
    std::cout << BOLD << RED << "0 1 2 3 4 5 6 7 8 9 10" << RESET << '\n';
    std::cout << BOLD << RED << "A " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "B " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "C " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "D " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "E " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "F " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "G " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "H " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "I " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    std::cout << BOLD << RED << "J " << RESET << BG_BLUE
              << "                    " << RESET << '\n';
    char way;
    char row;
    int column;
    int size;
    std::cout << "Pick a location to place a boat:" << std::endl;
    std::cin >> way;
    std::cin >> row;
    std::cin >> column;
    std::cin >> size;

    validateBoatLocation(way, row, column, size);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "USAGE: ./fleet_deployment.exe <name>" << std::endl;
        return 1;
    }

    std::cout << "Hello " << argv[1] << ". Do you want to play a game?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if (answer == "yes" || answer == "Yes")
    {
        startgame();
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
