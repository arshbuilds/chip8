#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>

using namespace std;

class Chip8
{
public:
    uint8_t memory[4096];
    uint16_t pc; // program counter
    Chip8()
    {
        for (int i = 0; i < 4096; i++)
        {
            memory[i] = 0;
        }
        pc = 0x200;
    }

    bool loadRom(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            std::cerr << "Failed to open ROM: " << filename << endl;
            return false;
        }
        std::streamsize size = file.tellg();

        if (size > 3584)
        { // 4096 - 512
            std::cerr << "ROM size exceeds memory limit: " << size << " bytes" << endl;
            return false;
        }

        std::vector<char> buffer(size);

        file.seekg(0, std::ios::beg);
        if (!file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read ROM: " << filename << endl;
            return false;
        }

        for (long i = 0; i < size; i++)
        {
            memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);
        }
        return true;
    }
};

int main()
{
    Chip8 chip8;
    chip8.loadRom("D:\\Projects\\chip8\\maze.ch8");
    for(int i = 0; i < 16; i++)
    {
        cout << hex << (int)chip8.memory[0x200 + i] << " ";
    }
    cout << endl;
return 0; }