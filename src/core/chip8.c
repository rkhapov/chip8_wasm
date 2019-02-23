#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "chip8.h"

#define BUFFER_SIZE 4096

byte buffer[BUFFER_SIZE];

#define CLS "00E0"
#define RTS "00EE"
#define JMP "1xxx"
#define JSR "2xxx"
#define JEQ "3xxx"
#define JNE "4xxx"
#define JER "5xx0"
#define MCR "6xxx"
#define ACR "7xxx"
#define MRR "8xx0"
#define OR  "8xx1"
#define AND "8xx2"
#define XOR "8xx3"
#define ARR "8xx4"
#define SUB "8xx5"
#define SHR "8xx6"
#define SBN "8xx7"
#define SHL "8xxE"
#define JNR "9xx0"
#define MVI "Axxx"
#define JMI "Bxxx"
#define RND "Cxxx"
#define DRW "Dxxx"
#define JKP "Ex9E"
#define JNP "ExA1"
#define GDT "Fxx7"
#define WKP "Fx0A"
#define SDT "Fx15"
#define SST "Fx18"
#define ADI "Fx1E"
#define LDC "Fx29"
#define BCD "Fx33"
#define STR "Fx55"
#define LDR "Fx65"

char hex_digit_to_char(int d) {
    static char buf[2];

    sprintf(buf, "%x", d);

    return toupper(buf[0]);
}

int is(word opcode, const char *op) {
    for (int i = 0; i < 4; i++) {
        char c = op[4 - i - 1];

        if (c == 'x') {
            continue;
        }

        int b = (opcode >> (4 * i)) & 0xF;
                    
        if (hex_digit_to_char(b) != c) {
            return 0;
        }
    }

    return 1;
}


void jmp(chip8 *chip8, word opcode) {
    chip8->registers->pc = opcode & 0x0FFF;
}

void jsr(chip8 *chip8, word opcode) {
    push(chip8->stack, chip8->registers->pc + 2);
    chip8->registers->pc = opcode & 0x0FFF;
}

void jeq(chip8 *chip8, word opcode) {
    int v = (opcode & 0x0F00) >> 8;    
    byte value = opcode & 0x00FF;

    if (chip8->registers->v[v] == value) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void jne(chip8 *chip8, word opcode) { 
    int v = (opcode & 0x0F00) >> 8;    
    byte value = opcode & 0x00FF;

    if (chip8->registers->v[v] != value) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void jer(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;      

    if (chip8->registers->v[x] == chip8->registers->v[y]) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void mcr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;

    chip8->registers->v[x] = opcode & 0x00FF;
}

void acr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;

    chip8->registers->v[x] += opcode & 0x00FF;
}

void mrr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[x] = chip8->registers->v[y];
}

void or(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[x] |= chip8->registers->v[y];
}

void and(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[x] &= chip8->registers->v[y];
}

void xor(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[x] ^= chip8->registers->v[y];
}

void arr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[V_FLAG_REGISTER] = chip8->registers->v[x] + (int)chip8->registers->v[y] >= 0x100; 

    chip8->registers->v[x] += chip8->registers->v[y];
}

void sub(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[V_FLAG_REGISTER] = chip8->registers->v[x] > chip8->registers->v[y]; 

    chip8->registers->v[x] -= chip8->registers->v[y];
}

void shr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;

    chip8->registers->v[V_FLAG_REGISTER] = chip8->registers->v[x] & 1; 

    chip8->registers->v[x] >>= 1;    
}

void sbn(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;  

    chip8->registers->v[V_FLAG_REGISTER] = chip8->registers->v[y] > chip8->registers->v[x]; 

    chip8->registers->v[x] = chip8->registers->v[y] - chip8->registers->v[x];    
}

void shl(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;

    chip8->registers->v[V_FLAG_REGISTER] = chip8->registers->v[x] >> 7; 

    chip8->registers->v[x] <<= 1;  
}

void jnr(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;      

    if (chip8->registers->v[x] != chip8->registers->v[y]) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void mvi(chip8 *chip8, word opcode) {
    chip8->registers->address = opcode & 0x0FFF;
}

void jmi(chip8 *chip8, word opcode) {
    chip8->registers->pc = (opcode & 0x0FFF) + (chip8->registers->v[0]);
}

void rnd(chip8 *chip8, word opcode) {
    int x = (opcode & 0x0F00) >> 8;

    chip8->registers->v[x] = (rand() % (0x100)) & (opcode & 0x00FF);
}

int get_bit(byte b, int i) {
    return (b & (1 << i)) >> i;
}

void drw(chip8 *chip8, word opcode) {
    word x = chip8->registers->v[(opcode & 0x0F00) >> 8];
    word y = chip8->registers->v[(opcode & 0x00F0) >> 4];
    int height = opcode & 0x000F;

    chip8->registers->v[V_FLAG_REGISTER] = 0;

    for (int i = 0; i < height; i++) {
        byte next_pixels = chip8->memory->memory[chip8->registers->address + i];

        for (int j = 0; j < 8; j++) {
            int pixel = get_bit(next_pixels, 7 - j);
            chip8->registers->v[V_FLAG_REGISTER] |= set_pixel(chip8->screen, y + i, x + j, pixel);
        }
    }
}

void jkp(chip8 *chip8, word opcode) {
    int key = chip8->registers->v[(opcode & 0x0F00) >> 8];

    if (is_key_pressed(chip8->keyboard, key)) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void jnp(chip8 *chip8, word opcode) {
    int key = chip8->registers->v[(opcode & 0x0F00) >> 8];

    if (!is_key_pressed(chip8->keyboard, key)) {
        chip8->registers->pc += 4;
    }
    else {
        chip8->registers->pc += 2;
    }
}

void gdt(chip8 *chip8, word opcode) {
    chip8->registers->v[(opcode & 0x0F00) >> 8] = chip8->delay_timer;
}

void wkp(chip8 *chip8, word opcode) {
    chip8->blocked = 1;

    if (!is_any_key_pressed(chip8->keyboard)) {
        return;        
    }

    chip8->registers->v[(opcode & 0x0F00) >> 8] = get_first_pressed(chip8->keyboard);

    chip8->blocked = 0;
}

void sdt(chip8 *chip8, word opcode) {
    chip8->delay_timer = chip8->registers->v[(opcode & 0x0F00) >> 8];
}

void sst(chip8 *chip8, word opcode) {
    chip8->sound_timer = chip8->registers->v[(opcode & 0x0F00) >> 8];
}

void adi(chip8 *chip8, word opcode) {
    byte x = chip8->registers->v[(opcode & 0x0F00) >> 8];

    if (x + chip8->registers->address > 0xFFF) {
        chip8->registers->v[V_FLAG_REGISTER] = 1;
    }
    else {
        chip8->registers->v[V_FLAG_REGISTER] = 0;
    }

    chip8->registers->address += x;
}

void ldc(chip8 *chip8, word opcode) {
    chip8->registers->address = chip8->registers->v[(opcode & 0x0F00) >> 8] * 5;
}

void bcd(chip8 *chip8, word opcode) {
    word I = chip8->registers->address;
    byte x = chip8->registers->v[(opcode & 0x0F00) >> 8];

    chip8->memory->memory[I] = x / 100;
    chip8->memory->memory[I + 1] = (x / 10) % 10;
    chip8->memory->memory[I + 2] = x % 10;
}

void str(chip8 *chip8, word opcode) { 
    int x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i++) {
        chip8->memory->memory[chip8->registers->address + i] = chip8->registers->v[i];        
    }

    //in older chip8 compatibility mode
    chip8->registers->address += x + 1;
}

void ldr(chip8 *chip8, word opcode) { 
    int x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i++) {
        chip8->registers->v[i] = chip8->memory->memory[chip8->registers->address + i];        
    }

    //in older chip8 compatibility mode
    chip8->registers->address += x + 1;
}


chip8* create_chip8() {
    chip8* c8 = malloc(sizeof(chip8));

    c8->exit_code = -1;
    c8->blocked = 0;
    c8->delay_timer = 0;
    c8->sound_timer = 0;
    c8->registers = create_registers();
    c8->memory = create_memory(RECOMMENDED_MEMORY_SIZE);
    c8->stack = create_stack(RECOMMENDED_STACK_SIZE);
    c8->screen = create_screen();
    c8->keyboard = create_keyboard();

    return c8;
}

void delete_chip8(chip8 *chip8) {
    delete_memory(chip8->memory);
    delete_stack(chip8->stack);
    delete_screen(chip8->screen);
    delete_keyboard(chip8->keyboard);
    delete_registers(chip8->registers);

    free(chip8);
}

void reset(chip8 *chip8) {
    delete_memory(chip8->memory);
    delete_stack(chip8->stack);
    delete_screen(chip8->screen);
    delete_keyboard(chip8->keyboard);
    delete_registers(chip8->registers);

    chip8->exit_code = -1;
    chip8->blocked = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->registers = create_registers();
    chip8->memory = create_memory(RECOMMENDED_MEMORY_SIZE);
    chip8->stack = create_stack(RECOMMENDED_STACK_SIZE);
    chip8->screen = create_screen();
    chip8->keyboard = create_keyboard();
}

int finished(chip8 *chip8) {
    return chip8->exit_code != -1;
}

void load_program(chip8 *chip8, byte *program, int size, int offset) {
    if (offset < 0 || offset + size >= chip8->memory->size) {
        return;
    }

    memcpy(chip8->memory->memory + offset, program, sizeof(byte) * size);

    chip8->registers->pc = offset;
}

int load_program_file(chip8 *chip8, const char *path, int offset) {
    if (offset < 0) {
        return 0;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        return 0;
    }

    int memory_offset = offset;

    while (1) {
        size_t read = fread(buffer, sizeof(byte), BUFFER_SIZE, file);

        if (read * sizeof(byte) + memory_offset >= chip8->memory->size) {
            fclose(file);
            return 0;
        }

        memcpy(chip8->memory->memory + memory_offset, buffer, read * sizeof(byte));
        memory_offset += read * sizeof(byte);

        if (read < sizeof(byte) * BUFFER_SIZE) {
            break;
        }
    }

    fclose(file);
    chip8->registers->pc = offset;

    return 1;
}

word get_next_opcode(chip8 *chip8) {
    return chip8->memory->memory[chip8->registers->pc] << 8 | chip8->memory->memory[chip8->registers->pc + 1];
}

int end_program_reached(chip8 *chip8) {
    return chip8->registers->pc >= chip8->memory->size || get_next_opcode(chip8) == 0;
}

int is_set_pc_instruction(word opcode) {
    return 
        is(opcode, RTS) ||
        is(opcode, JMP) ||
        is(opcode, JSR) ||
        is(opcode, JEQ) ||
        is(opcode, JNE) ||
        is(opcode, JER) ||
        is(opcode, JNR) ||
        is(opcode, JMI) ||
        is(opcode, JKP) ||
        is(opcode, JNP);
}

typedef void (*instruction_executer)(chip8*, word);

typedef struct {
    instruction_executer executer;
    const char *mask;
} handler;

#define HANDLERS_COUNT 32

handler handlers_table[HANDLERS_COUNT] = {
    {jmp, JMP},
    {jsr, JSR},
    {jeq, JEQ},
    {jne, JNE},
    {jer, JER},
    {mcr, MCR},
    {acr, ACR},
    {mrr, MRR},
    {or, OR},
    {and, AND},
    {xor, XOR},
    {arr, ARR},
    {sub, SUB},
    {shr, SHR},
    {sbn, SBN},
    {shl, SHL},
    {jnr, JNR},
    {mvi, MVI},
    {jmi, JMI},
    {rnd, RND},
    {drw, DRW},
    {jkp, JKP},
    {jnp, JNP},
    {gdt, GDT},
    {wkp, WKP},
    {sdt, SDT},
    {sst, SST},
    {adi, ADI},
    {ldc, LDC},
    {bcd, BCD},
    {str, STR},
    {ldr, LDR}
};

void execute_opcode_from_table(chip8 *chip8, word opcode) {
    for (int i = 0; i < HANDLERS_COUNT; i++) {
        if (is(opcode, handlers_table[i].mask)) {
            handlers_table[i].executer(chip8, opcode);
            return;
        }
    }

    printf("Unknown code: %x\n", opcode);
    abort();
}

void execute_opcode(chip8 *chip8, word opcode) {
    if (is(opcode, CLS)) {
        clear_screen(chip8->screen);
    }
    else if (is(opcode, RTS)) {
        chip8->registers->pc = pop(chip8->stack);
    }
    else {
        execute_opcode_from_table(chip8, opcode);        
    }
}

void execute_next(chip8 *chip8) {
    if (end_program_reached(chip8)) {
        chip8->exit_code = 0;
        return;
    }

    word opcode = get_next_opcode(chip8);
    
    execute_opcode(chip8, opcode);

    if (!is_set_pc_instruction(opcode) && !chip8->blocked) {
        chip8->registers->pc += 2;        
    }
}