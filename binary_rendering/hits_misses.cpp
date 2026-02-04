#include <iostream>
#include <array>
#include <string>
#include <limits>

#include "place_boats.h"
#include "hits_misses.h"
#include "save_game.h"

static const std::string SHIP = "🚢"; 
static const std::string HIT = "💥";
static const std::string MISS = "🚀";
static const std::string SUNK = "🔥";

extern std::array<std::array<std::string, 10>, 10> player;
extern std::array<std::array<std::string, 10>, 10> enemy;

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


void check_and_sunk_boat(const boat& b, bool is_enemy) {
    bool all_hit = true;

    for (int i = 0; i < b.size; ++i) {
        int r = b.positions[i].first;
        int c = b.positions[i].second;

        const auto& cell = is_enemy ? enemy[r][c] : player[r][c];
        if (cell != HIT && cell != SUNK) {
            all_hit = false;
            break;
        }
    }

    if (!all_hit) return;

    bool changed = false;

    for (int i = 0; i < b.size; ++i) {
        int r = b.positions[i].first;
        int c = b.positions[i].second;

        if (is_enemy) {
            if (enemy[r][c] != SUNK) {
                enemy[r][c] = SUNK;
                save_shots('P', char('A' + r), c + 1, 'S', current_save_file);
                changed = true;
            }
        } else {
            if (player[r][c] != SUNK) {
                player[r][c] = SUNK;
                save_shots('E', char('A' + r), c + 1, 'S', current_save_file);
                changed = true;
            }
        }
    }

    if (changed) std::cout << "Boat has sunk!" << std::endl;
}

void sink_enemy_ship(int r, int c, char rowChar)
{
    enemy[r][c] = SUNK;
    save_shots('P', rowChar, c + 1, 'S', current_save_file);

    // left
    int cc = c - 1;
    while (cc >= 0 && enemy[r][cc] == HIT) {
        enemy[r][cc] = SUNK;
        save_shots('P', rowChar, cc + 1, 'S', current_save_file);
        cc--;
    }

    // right
    cc = c + 1;
    while (cc < 10 && enemy[r][cc] == HIT) {
        enemy[r][cc] = SUNK;
        save_shots('P', rowChar, cc + 1, 'S', current_save_file);
        cc++;
    }

    // up
    int rr = r - 1;
    while (rr >= 0 && enemy[rr][c] == HIT) {
        enemy[rr][c] = SUNK;
        save_shots('P', char('A' + rr), c + 1, 'S', current_save_file);
        rr--;
    }

    // down
    rr = r + 1;
    while (rr < 10 && enemy[rr][c] == HIT) {
        enemy[rr][c] = SUNK;
        save_shots('P', char('A' + rr), c + 1, 'S', current_save_file);
        rr++;
    }
}

static bool enemy_tracking()
{
    while (true)
    {
        std::cout << "Enemy's turn! Enter enemy shot (expected: A1): " << std::endl;

        int r, c;
        if (!read_pos_A1_or_A_1(r, c))
        {
            if (std::cin.eof()) return false;
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        char rowChar = static_cast<char>('A' + r);
        int colHuman = c + 1;

        if (r < 0 || r > 9 || c < 0 || c > 9) {
            std::cout << "Invalid input. Format: H A1" << std::endl;
            continue;
        }

        if (player[r][c] == HIT || player[r][c] == MISS || player[r][c] == SUNK) {
            std::cout << "Enemy already fired at this location" << std::endl;
            continue;
        }

        if (player[r][c] == SHIP) {
            player[r][c] = HIT;
            std::cout << "Enemy shot at " << rowChar << colHuman << ": HIT!" << std::endl;
            save_shots('E', rowChar, colHuman, 'H', current_save_file);
            // check sunk boats
            for (auto &b : boats_array) check_and_sunk_boat(b, false);
        } else {
            player[r][c] = MISS;
            std::cout << "Enemy shot at " << rowChar << colHuman << ": MISS!" << std::endl;
            save_shots('E', rowChar, colHuman, 'M', current_save_file);
        }
        break;
    }

    print_yourboard();
    return true;
}

static bool player_tracking()
{
    while (true)
    {
        print_enemyboard(); 
        std::cout << "Your turn! Enter your shot (expected: A1 H/M/S): " << std::endl;

        int r, c;
        char result;

        if (!read_pos_A1_or_A_1(r, c) || !(std::cin >> result))
        {
            if (std::cin.eof()) return false;
            std::cout << "Invalid input. Format: H A1" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        result = static_cast<char>(std::tolower(static_cast<unsigned char>(result)));
        char rowChar = static_cast<char>('A' + r);
        int colHuman = c + 1;

        if (r < 0 || r > 9 || c < 0 || c > 9) {
            std::cout << "Invalid coordinates" << std::endl;
            continue;
        }

        if (enemy[r][c] == HIT || enemy[r][c] == MISS || enemy[r][c] == SUNK) {
            std::cout << "You already fired at this location" << std::endl;
            std::cin.clear();
            continue;
        }

        if (result == 'h') {
            enemy[r][c] = HIT;
            std::cout << "You shot at " << rowChar << colHuman << ": HIT!" << std::endl;
            save_shots('P', rowChar, colHuman, 'H', current_save_file);
            break;
        }
        else if (result == 'm') {
            enemy[r][c] = MISS;
            std::cout << "You shot at " << rowChar << colHuman << ": MISS!" << std::endl;
            save_shots('P', rowChar, colHuman, 'M', current_save_file);
            break;
        }
        else if (result == 's') {
            sink_enemy_ship(r, c, rowChar);
            std::cout << "You shot at " << rowChar << colHuman << ": SUNK!" << std::endl;
            save_shots('P', rowChar, colHuman, 'S', current_save_file);
            break;
        }
        else {
            std::cout << "Invalid input. Format: H A1" << std::endl;
        }
    }

    print_enemyboard();
    return true;
}


bool all_sunk(const std::array<std::array<std::string, 10>, 10>& board, bool is_enemy_board) {
    int count = 0;
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            if (is_enemy_board) {
                // For the enemy board, we win if we have recorded 17 hits/sunks
                if (board[r][c] == HIT || board[r][c] == SUNK) {
                    count++;
                }
            } else {
                // For your own board, you lose if 0 SHIP icons remain
                if (board[r][c] == SHIP) {
                    return false; 
                }
            }
        }
    }

    if (is_enemy_board) {
        return (count >= 17); // Returns true if you've hit all 17 spots
    }
    
    return true; // If no ships were found on player board
}

void play_game() {
    bool player_turn = (get_next_turn() == 'P');
    while (true) {
        if (player_turn) {
            player_tracking();
        }
        else {
            enemy_tracking();
        }
        player_turn = !player_turn;
        if (all_sunk(player, false)) {
            std::cout << "All your ships have been sunk. Game over!" << std::endl;
            break;
        }
        if (all_sunk(enemy, true)) {
            std::cout << "You sunk all enemy ships. You win!" << std::endl;
            break;
        }
    }
}
