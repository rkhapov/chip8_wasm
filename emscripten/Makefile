BUILD_DIR = result
CXX = emcc
INCLUDE_DIR = include
INCLUDE_CORE_DIR = ${INCLUDE_DIR}/core
CXX_FLAGS = -std=c11 -s WASM=1 -Os -c
LINK_FLAGS = -s EXPORTED_FUNCTIONS='["_test_stack"]'

all: build_dir_make core.js
	cp page.html ${BUILD_DIR}/page.html
	cp script.js ${BUILD_DIR}/script.js

	rm -rf ${BUILD_DIR}/*.bc

build_dir_make:
	mkdir -p ${BUILD_DIR}

core.js: stack.bc main.bc memory.bc registers.bc chip8.bc
	${CXX} \
		 ${BUILD_DIR}/stack.bc \
		 ${BUILD_DIR}/main.bc \
		 ${BUILD_DIR}/memory.bc \
		 ${BUILD_DIR}/registers.bc \
		 ${BUILD_DIR}/chip8.bc \
		 -s WASM=1 -o ${BUILD_DIR}/core.js ${LINK_FLAGS}

registers.bc:
	${CXX} src/core/registers.c -o ${BUILD_DIR}/registers.bc -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

stack.bc:
	${CXX} src/core/stack.c -o ${BUILD_DIR}/stack.bc -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

memory.bc:
	${CXX} src/core/memory.c -o ${BUILD_DIR}/memory.bc -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

chip8.bc:
	${CXX} src/core/chip8.c -o ${BUILD_DIR}/chip8.bc -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

main.bc:
	${CXX} src/main.c -o ${BUILD_DIR}/main.bc -I${INCLUDE_DIR} ${CXX_FLAGS}

clean:
	rm -rf ${BUILD_DIR}/*
