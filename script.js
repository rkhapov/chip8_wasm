async function start() {
    var e = document.getElementById("game");
    var value = e.options[e.selectedIndex].value;

    let ptr = Module.allocate(Module.intArrayFromString(value), 'i8', Module.ALLOC_NORMAL);

    Module._run_chip8(ptr);

    Module._free(ptr);
}

async function init() {
}