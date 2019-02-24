var key_to_number = {
    '1': 1, '2': 2, '3': 3, '4': 0xc,
    'q': 4, 'Q': 4, 'w': 5, 'W': 5,
    'e': 6, 'E': 6, 'r': 0xd, 'R': 0xd,
    'a': 7, 'A': 7, 's': 8, 'S': 8,
    'd': 9, 'D': 9, 'f': 0xe, 'F': 0xe,
    'z': 0xa, 'Z': 0xa, 'x': 0, 'X': 0,
    'c': 0xb, 'C': 0xb, 'v': 0xf, 'V': 0xF
}

function onKeyDown(e) {
    if (e.key in key_to_number) {
        Module._chip8_do_key_down(key_to_number[e.key]);
    }
}

function onKeyUp(e) {
    if (e.key in key_to_number) {
        Module._chip8_do_key_up(key_to_number[e.key]);
    }
}

function draw_screen() {
    let height = Module._chip8_get_screen_height();
    let width = Module._chip8_get_screen_width();
    let context = document.getElementById("canvas").getContext("2d");

    context.fillStyle='black';
    context.fillRect(0, 0, 640, 320);
    context.fillStyle='white';

    for (let i = 0; i < height; i++) {
        for (let j = 0; j < width; j++) {
            if (Module._chip8_get_pixel(i, j) == 1) {
                context.fillRect(j * 10, i * 10, 10, 10);            
            }
        }
    }
}

function tick() {
    Module._chip8_do_tick();
    draw_screen();

    setTimeout(tick, 1);
}

async function init() {
    // document.addEventListener('keydown', (e) => onKeyDown(e))
    // document.addEventListener('keyup', (e) => onKeyUp(e))

    // Module._chip8_start();

    let ptr = Module.allocate(Module.intArrayFromString('games/PONG2'), 'i8', Module.ALLOC_NORMAL);

    // Module._chip8_load(ptr);
    Module._run(ptr);

    Module._free(ptr);

    // setTimeout(tick, 5);
}