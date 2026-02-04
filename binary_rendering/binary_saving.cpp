#include <fstream>
#include <iostream>
#include <array>
#include "binary_saving.h"
#include "save_game.h"

static const std::string SHIP = "🚢"; 
static const std::string HIT = "💥";
static const std::string MISS = "🚀";
static const std::string SUNK = "🔥";

extern std::array<std::array<std::string, 10>, 10> player;
extern std::array<std::array<std::string, 10>, 10> enemy;

static char next_turn_bin = 'P';

namespace binaryformat {
    bool is_binary_save(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) return false;

        char m[4];
        in.read(m, 4);
        if (!in) return false;

        return (m[0]==MAGIC[0] && m[1]==MAGIC[1] && m[2]==MAGIC[2] && m[3]==MAGIC[3]);
    }
    void write_binary_header(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out) {
            std::cerr << "Error: could not open '" << filename << "' for writing" << std::endl;
            return;
        }
        out.write(MAGIC, 4);
        out.put(static_cast<char>(VERSION));
    }
    bool load_game_binary(const std::string& filename) {
        std::ifstream in (filename, std::ios::binary);
        if (!in) return false;

        char m[4];
        in.read(m, 4);
        if (!in) return false;
        char ver = 0;
        in.get(ver);
        if (!in) return false;

        for (int i = 0; i < 5; i++) {
            boat_bits bb{};
            
            in.read(reinterpret_cast<char*>(&bb), sizeof(bb));

            if (!in) {
                in.clear();
                for (int j = i; j < 5; j++) {
                    boats_array[j].direction = 0;
                    boats_array[j].row = 0;
                    boats_array[j].column = 0;
                }
                return true;
            }

            boats_array[i].size = int(bb.size);
            boats_array[i].direction = bb.direction ? 'H' : 'V';
            boats_array[i].row = char('A' + bb.row);
            boats_array[i].column = int(bb.column);
        }
        return true;
    }

    void save_boat_binary(const boat& b, const std::string& filename) {
        std::ofstream out(filename, std::ios::binary | std::ios::app);
        if (!out) {
            std::cerr << "Errpr: could not open '" << filename << "' for appending" << std::endl;
            return;
        }

        boat_bits bb{};
        bb.row= uint8_t(b.row - 'A');
        bb.column = uint8_t(b.column);
        bb.direction = (b.direction == 'H') ? 1 : 0;
        bb.size = uint8_t(b.size);
        bb.unused = 0;

        out.write(reinterpret_cast<const char*>(&bb), sizeof(bb));
    }

    void save_shot_binary(char shooter, char rowChar, int colBased, char result, const std::string& filename) {
        std::ofstream out(filename, std::ios::binary | std::ios::app);
        if (!out) {
            std::cerr << "Error: could not open '" << filename << "' for appending" << std::endl;
            return;
        }

        char row = rowChar;
        if (row >= 'a' && row <= 'j') row = row - 'a' + 'A';

        uint8_t r = uint8_t(row - 'A');
        uint8_t c = uint8_t(colBased - 1);

        shot_bits s{};
        s.row = r;
        s.column = c;

        out.write(&shooter, 1);
        out.write(reinterpret_cast<const char*>(&s), sizeof(s));
        out.write(&result, 1);

        next_turn_bin = (shooter == 'P') ? 'E' : 'P';
    }

    bool replay_shots_binary(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) return false;

        char m[4];
        in.read(m, 4);
        char ver = 0;
        in.get(ver);
        if (!in) return false;

        for (int i = 0; i < 5; i++) {
            boat_bits bb{};
            in.read(reinterpret_cast<char*>(&bb), sizeof(bb));
            if (!in) return true;
        }

        char last_shooter = '\0';

        while (true) {
            char shooter = 0;
            shot_bits s{};
            char result = 0;

            in.read(&shooter, 1);
            if (!in) break;

            in.read(reinterpret_cast<char*>(&s), sizeof(s));
            if (!in) break;

            in.read(&result, 1);
            if (!in) break;

            last_shooter = shooter;

            uint8_t r = s.row;
            uint8_t c = s.column;
            if (r > 9 || c > 9) continue;

            if (shooter == 'E') {
                if (result == 'H') player[r][c] = "💥";
                else if (result == 'M') player[r][c] = "🚀";
                else if (result == 'S') player[r][c] = "🔥";
            } else if (shooter == 'P') {
                if (result == 'H') enemy[r][c] = "💥";
                else if (result == 'M') enemy[r][c] = "🚀";
                else if (result == 'S') enemy[r][c] = "🔥";
            }
        }

        if (last_shooter == 'P') next_turn_bin = 'E';
        else if (last_shooter == 'E') next_turn_bin = 'P';
        else next_turn_bin = 'P';

        return true;
    }

    char get_next_turn_binary() {
        return next_turn_bin;
    }
}
