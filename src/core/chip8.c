#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chip8.h"

#define BUFFER_SIZE 4096

byte buffer[BUFFER_SIZE];

#define OP_CLASS(opcode)    ((opcode) & 0xF000)
#define OP_SUBCLASS(opcode) ((opcode) & 0x000F)

#define IS(opcode, op)    (((opcode) & (op)) == (op))

#define CLS 0x00E0
#define RTS 0x00EE
#define JMP 0x1000
#define JSR 0x2000
#define JEQ 0x3000
#define JNE 0x4000
#define JER 0x5000
#define MCR 0x6000
#define ACR 0x7000
#define MRR 0x8000
#define OR  0x8001
#define AND 0x8002
#define XOR 0x8003
#define ARR 0x8004
#define SUB 0x8005
#define SHR 0x8006
#define SBN 0x8007
#define SHL 0x800E
#define JNR 0x9000
#define MVI 0xA000
#define JMI 0xB000
#define RND 0xC000
#define DRW 0xD000
#define JKP 0xE09E
#define JNP 0xE0A1
#define GDT 0xF007
#define WKP 0xF00A
#define SDT 0xF015
#define SST 0xF018
#define ADI 0xF01E
#define LDC 0xF029
#define BCD 0xF033
#define STR 0xF055
#define LDR 0xF065

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

    chip8->registers->v[V_FLAG_REGISTER] = (int)chip8->registers->v[x] + (int)chip8->registers->v[y] >= 0x100; 

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
    word y = chip8->registers->v[(opcode & 0x0F00) >> 4];
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
        IS(opcode, RTS) ||
        IS(opcode, JMP) ||
        IS(opcode, JSR) ||
        IS(opcode, JEQ) ||
        IS(opcode, JNE) ||
        IS(opcode, JER) ||
        IS(opcode, JNR) ||
        IS(opcode, JMI) ||
        IS(opcode, JKP) ||
        IS(opcode, JNP);
}

typedef void (*instruction_executer)(chip8*, word);

typedef struct {
    instruction_executer executer;
    int mask;
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
        if (IS(opcode, handlers_table[i].mask)) {
            handlers_table[i].executer(chip8, opcode);
            return;
        }
    }

    printf("Not implemented instruction: %x\n", opcode);
    abort();
}

void execute_opcode(chip8 *chip8, word opcode) {
    if (opcode == CLS) {
        clear_screen(chip8->screen);
    }
    else if (opcode == RTS) {
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