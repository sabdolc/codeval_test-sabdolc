#pragma once

#include <cstdint>
#include <string>
#include "save_game.h"

/*
 * Functions for reading and writing the binary save format.
 *
 * This is an advanced feature introduced in Assignment 6.
 * Students do NOT need to modify this unless instructed.
 */
namespace binaryformat {

    /*
     * Magic bytes used to identify a valid binary save file.
     */
    static constexpr char MAGIC[4] = {'B', 'S', 'H', '6'};

    /*
     * Binary file version number.
     */
    static constexpr uint8_t VERSION = 1;

    /*
     * Packed representation of a boat in the binary file.
     * Uses bit fields to minimize file size.
     */
    #pragma pack(push, 1)
    struct boat_bits {
        uint16_t size: 3;
        uint16_t direction: 1;
        uint16_t row: 4;
        uint16_t column: 4;
        uint16_t unused: 4;
    };

    /*
     * Packed representation of a shot location.
     */
    struct shot_bits {
        uint8_t row: 4;
        uint8_t column: 4;
    };

    /*
     * Packed representation of a shot with result flags.
     */
    struct my_shot_bits {
        uint8_t hit: 1;
        uint8_t sunk: 1;
        uint8_t unused: 6;
        shot_bits loc;
    };
    #pragma pack(pop)

    /*
     * Checks whether a file is a valid binary save.
     */
    bool is_binary_save(const std::string& filename);

    /*
     * Writes the binary header to a new save file.
     */
    void write_binary_header(const std::string& filename);

    /*
     * Loads boats from a binary save file.
     */
    bool load_game_binary(const std::string& filename);

    /*
     * Saves one boat in binary format.
     */
    void save_boat_binary(const boat& b, const std::string& filename);

    /*
     * Saves one shot in binary format.
     */
    void save_shot_binary(char shooter, char row, int col, char result,
                          const std::string& filename);

    /*
     * Replays all shots from a binary save file.
     */
    bool replay_shots_binary(const std::string& filename);

    /*
     * Returns whose turn is next (binary save).
     */
    char get_next_turn_binary();
}
