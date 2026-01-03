#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    uint8_t memory[4096];
    uint8_t v[16];
    uint16_t I;
    uint16_t pc;

    uint8_t gfx[64 * 32];

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t stack[16];
    uint8_t sp;

    uint8_t key[16];
} chip8;

chip8 init_start(chip8 *c)
{

    int memory_size = 4096;

    int count_v = 16;

    int stack_size = 16;

    for (int i = 0; i <= memory_size - 1; i++)
    {
        c->memory[i] = 0;
    }

    for (int i = 0; i <= count_v - 1; i++)
    {
        c->v[i] = 0;
    }

    for (int i = 0; i <= stack_size - 1; i++)
    {
        c->stack[i] = 0;
    }

    for (int i = 0; i <= 64; i++)
    {
        for (int x = 0; x <= 32; x++)
        {
            c->gfx[i * x] = 0;
        }
    }

    c->pc = 0x200;

    c->sp = 0;

    c->I = 0;

    c->delay_timer = 0;

    c->sound_timer = 0;
}

void read_file(char *file_name, chip8 *c)
{
    FILE *file_ptr;
    file_ptr = fopen(file_name, "rb");

    fseek(file_ptr, 0, SEEK_END);

    long size = ftell(file_ptr);

    rewind(file_ptr);

    if (file_ptr == NULL)
    {
        printf("Cannot open file");
    }

    if (size > 3584)
    {
        printf("File too large!!!");
        fclose(file_ptr);
    }

    fread(&c->memory[512], sizeof(uint8_t), size, file_ptr);

    fclose(file_ptr);
}

int main()
{
    char *file_name = "Pong.ch8";

    chip8 c;
    init_start(&c);

    read_file(file_name, &c);

    c.pc = 512;

    uint16_t opcode, first_4_bit, second_4_bit, last_12_bit, third_4_bit, last_8_bit, last_4_bit;

    while (true)
    {
        opcode = c.memory[c.pc];

        opcode = opcode << 8;

        opcode = opcode | (uint16_t)c.memory[c.pc + 1];

        first_4_bit = (opcode & 0xF000) >> 12;

        second_4_bit = (opcode & 0x0F00) >> 8;

        third_4_bit = (opcode & 0x00F0) >> 4;

        last_4_bit = (opcode & 0x000F);

        last_12_bit = opcode & 0x0FFF;

        last_8_bit = opcode & 0x00FF;

        switch (first_4_bit)
        {
        case 0x1:
            c.pc = last_12_bit;
            break;
        case 0x6:
            c.v[second_4_bit] = last_8_bit;
            c.pc += 2;
            break;
        case 0x7:
            c.v[second_4_bit] += last_8_bit;
            c.pc += 2;
            break;
        case 0xA:
            c.I = last_12_bit;
            c.pc += 2;
            break;
        case 0xD:
            uint8_t Coordonate_x, Coordinate_y, Height, Sprite, Scanner, Target_X, Target_Y, Index;

            Scanner = 0x80;

            c.v[0xF] = 0;

            Coordonate_x = c.v[second_4_bit];
            Coordinate_y = c.v[third_4_bit];

            Height = last_4_bit;

            for (uint8_t row = 0; row < Height; row++)
            {
                uint8_t sprite_bytes = c.memory[c.I + row];
                for (uint8_t col = 0; col < 8; col++)
                {
                    if (((Scanner >> col) & sprite_bytes) != 0)
                    {
                        Target_X = (Coordonate_x + col) % 64;
                        Target_Y = (Coordinate_y + row) % 32;
                        Index = (Target_Y * 64) + Target_X;

                        if (c.gfx[Index] == 1)
                        {
                            c.v[0xF] = 1;
                        }
                        c.gfx[Index] ^= 1;
                    }
                    else
                        continue;
                }
            }
            c.pc += 2;
            break;

        default:
            c.pc += 2;
        }
    }
}