#pragma once

#include <array>
#include <string>
#include <utility>

/*
 * Represents a single boat in the game.
 *
 * size:
 *   Length of the boat (2–5)
 *
 * direction:
 *   'H' = horizontal
 *   'V' = vertical
 *
 * row:
 *   Starting row ('A'–'J')
 *
 * column:
 *   Starting column (0–9)
 *
 * positions:
 *   All board cells occupied by this boat
 *   (used to detect when a boat is sunk)
 */
struct boat
{
    int size{};
    char direction{};
    char row{};
    int column{};
    std::pair<int, int> positions[5];
};

/*
 * Name of the save file currently in use.
 */
extern std::string current_save_file;

/*
 * All boats in the current game (exactly 5).
 */
extern std::array<boat, 5> boats_array;

/*
 * True if the current save file is binary,
 * false if it is plain text.
 */
extern bool using_binary_save;

/*
 * Initializes the standard Battleship boat sizes.
 */
void set_boatsizes();

/*
 * Starts a new save file.
 * - Clears old contents
 * - Writes binary header if needed
 */
void reset_boat_save_file(const std::string& filename = current_save_file);

/*
 * Saves one boat to the current save file.
 */
void save_boat(const boat& b, const std::string& filename = current_save_file);

/*
 * Loads boats from a save file into boats_array.
 *
 * Returns:
 *   true  if successful
 *   false on error
 */
bool load_boats_from_file(const std::string& filename);

/*
 * Records a single shot in the save file.
 *
 * shooter:
 *   'P' = player
 *   'E' = enemy
 *
 * row, col:
 *   Board location of the shot
 *
 * result:
 *   'H' = hit
 *   'M' = miss
 *   'S' = sunk
 */
void save_shots(char shooter, char row, int col, char result,
                const std::string& filename = current_save_file);

/*
 * Replays all saved shots and updates the boards.
 *
 * Returns:
 *   true  if successful
 *   false on error
 */
bool load_shots_from_file(const std::string& filename = current_save_file);

/*
 * Returns whose turn is next:
 *   'P' = player
 *   'E' = enemy
 */
char get_next_turn();
