BUILD_DIR = result
CXX = gcc
AR = ar
INCLUDE_DIR = include
INCLUDE_CORE_DIR = ${INCLUDE_DIR}/core
CXX_FLAGS = -std=c11 -c -Wall
LINK_FLAGS = -lSDL2
REXT = 

all: build_dir_make chip8${REXT}
	cp page.html ${BUILD_DIR}/index.html
	cp script.js ${BUILD_DIR}/script.js
	cp -rf games ${BUILD_DIR}/games

	rm -rf ${BUILD_DIR}/*.o

build_dir_make:
	mkdir -p ${BUILD_DIR}

chip8${REXT}: core.a main.o api.o gui.o
	${CXX} \
		 ${BUILD_DIR}/main.o \
		 ${BUILD_DIR}/api.o \
		 ${BUILD_DIR}/gui.o \
		 ${BUILD_DIR}/core.a \
		-o ${BUILD_DIR}/chip8${REXT} ${LINK_FLAGS} \

api.o:
	${CXX} src/api.c -o ${BUILD_DIR}/api.o -I${INCLUDE_DIR} ${CXX_FLAGS}

core.a: stack.o memory.o registers.o chip8.o screen.o keyboard.o
	${AR} rvs ${BUILD_DIR}/core.a \
		${BUILD_DIR}/stack.o \
		${BUILD_DIR}/memory.o \
		${BUILD_DIR}/registers.o \
		${BUILD_DIR}/chip8.o \
		${BUILD_DIR}/screen.o \
		${BUILD_DIR}/keyboard.o

registers.o:
	${CXX} src/core/registers.c -o ${BUILD_DIR}/registers.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

stack.o:
	${CXX} src/core/stack.c -o ${BUILD_DIR}/stack.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

memory.o:
	${CXX} src/core/memory.c -o ${BUILD_DIR}/memory.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

chip8.o:
	${CXX} src/core/chip8.c -o ${BUILD_DIR}/chip8.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

screen.o:
	${CXX} src/core/screen.c -o ${BUILD_DIR}/screen.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

keyboard.o:
	${CXX} src/core/keyboard.c -o ${BUILD_DIR}/keyboard.o -I${INCLUDE_CORE_DIR} ${CXX_FLAGS}

main.o:
	${CXX} src/main.c -o ${BUILD_DIR}/main.o -I${INCLUDE_DIR} ${CXX_FLAGS}

gui.o:
	${CXX} src/gui/sdl_runner.c -o ${BUILD_DIR}/gui.o -I${INCLUDE_DIR} ${CXX_FLAGS}

clean:
	rm -rf ${BUILD_DIR}/*
