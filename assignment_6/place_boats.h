#pragma once

#include <array>
#include <string>

/*
 * Global flags used by the game
 * --------------------------------
 * loading_game:
 *   true  -> boats and shots are being loaded from a save file
 *   false -> normal gameplay
 *
 * lazy_mode:
 *   true  -> boats are placed automatically (randomly)
 *   false -> user places boats manually
 */
extern bool loading_game;
extern bool lazy_mode;

/*
 * 10x10 game boards
 * --------------------------------
 * player:
 *   Your own fleet (ships, hits, misses)
 *
 * enemy:
 *   Your view of the enemy's board
 *
 * Each cell contains a string such as:
 *   "  "  (water)
 *   "🚢"  (ship)
 *   "💥"  (hit)
 *   "🚀"  (miss)
 *   "🔥"  (sunk)
 */
extern std::array<std::array<std::string, 10>, 10> player;
extern std::array<std::array<std::string, 10>, 10> enemy;

/*
 * Checks whether a boat placement is valid
 *
 * Parameters:
 *   way    - 'H' for horizontal or 'V' for vertical
 *   row    - row letter ('A' through 'J')
 *   column - starting column (0–9)
 *   size   - boat length
 *
 * Returns:
 *   true  if the placement fits on the board
 *   false otherwise
 */
bool validBoatLocation(char way, char row, int column, int size);

/*
 * Handles the full boat placement phase.
 *
 * - Loads boats from save file if needed
 * - Allows manual placement or lazy placement
 * - Updates the player board
 */
void place_boats();

/*
 * Loads boats from the save file and draws them on the board.
 *
 * Returns:
 *   number of boats successfully loaded
 */
int load_board();

/*
 * Prints the player's fleet board to the terminal.
 */
void print_yourboard();

/*
 * Prints the enemy board as seen by the player.
 */
void print_enemyboard();