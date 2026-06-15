#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <random>

class Chip8
{

private:
    uint8_t V[16];   // registers
    uint16_t opcode; // opcode
    uint16_t I;      // index register

public:
    uint8_t memory[4096];
    bool display[64][32];

    uint16_t pc; // program counter
    Chip8()
    {
        for (int i = 0; i < 64, i < 32; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                display[j][i] = false;
            }
        }

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
            std::cerr << "Failed to open ROM: " << filename << std::endl;
            return false;
        }
        std::streamsize size = file.tellg();

        if (size > 3584)
        { // 4096 - 512
            std::cerr << "ROM size exceeds memory limit: " << size << " bytes" << std::endl;
            return false;
        }

        std::vector<char> buffer(size);

        file.seekg(0, std::ios::beg);
        if (!file.read(buffer.data(), size))
        {
            std::cerr << "Failed to read ROM: " << filename << std::endl;
            return false;
        }

        for (long i = 0; i < size; i++)
        {
            memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);
        }
        return true;
    }

    void fetchInstruction()
    {
        uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
        pc += 2;
        std::cout << "Fetched opcode: " << std::hex << opcode << std::endl;
        this->opcode = opcode;
    }

    void executeInstruction()
    {
        uint8_t family = (opcode) >> 12;
        switch (family)
        {
        case 0x6:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);
            V[x] = nn;
            std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
            break;
        }
        case 0xA:
        {
            uint16_t nnn = (opcode & 0x0FFF);
            I = nnn;
            std::cout << "I: " << std::hex << I << std::endl;
            break;
        }
        case 0xC:
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            int randomValue = dis(gen);

            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);
            std::cout << randomValue << std::endl;
            V[x] = randomValue & nn;
            std::cout << "V[" << static_cast<int>(x) << "]: " << static_cast<int>(V[x]) << std::endl;
            break;
        }
        case 0x3:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);
            if (V[x] == nn)
            {
                pc += 2;
                std::cout << "Skipping next instruction" << std::endl;
            }
            else
            {
                std::cout << "Not skipping next instruction" << std::endl;
            }
            break;
        }
        case 0xD:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            uint8_t n = (opcode & 0x000F);

            // 2. Fix: Initialize the collision flag to 0 before processing pixels
            V[15] = 0;

            for (int i = 0; i < n; i++)
            {
                uint8_t spriteRow = memory[I + i];
                for (int j = 0; j < 8; j++)
                {
                    if ((spriteRow & (0x80 >> j)) != 0)
                    {
                        // Note: Modulo implements screen wrapping (standard for many CHIP-8 games)
                        int xCoord = (V[x] + j) % 64;
                        int yCoord = (V[y] + i) % 32;

                        if (display[xCoord][yCoord])
                        {
                            V[15] = 1; // Collision detected
                        }

                        display[xCoord][yCoord] ^= true;
                    }
                }
            }
            break;
        }
        };
    }

    void prtScr()
    {
        for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                std::cout << (display[x][y] ? "#" : ".");
            }
            std::cout << std::endl;
        }
    }
};
int main()
{
    Chip8 chip8;
    chip8.loadRom("D:\\Projects\\chip8\\maze.ch8");
    for (int i = 0; i < 10; i++)
    {
        chip8.fetchInstruction();
        chip8.executeInstruction();
    }

    chip8.prtScr();

    return 0;
}