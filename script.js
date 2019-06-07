async function start() {
    var e = document.getElementById("game");
    var value = e.options[e.selectedIndex].value;

    let ptr = Module.allocate(Module.intArrayFromString(value), 'i8', Module.ALLOC_NORMAL);

    Module._run_chip8(ptr);

    Module._free(ptr);
}

async function init() {
    var canvas = document.getElementById("canvas");
    var ctx = canvas.getContext("2d");
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    ctx.font = '24px serif';
    ctx.fillStyle = 'white';
    ctx.fillText('Select game and press Start button', 80, 180);
}

// init();