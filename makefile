srcs := $(wildcard ./src/*.c)
headers := ./include
libs_include := -I./libs/SDL2/include/SDL2 -I./libs/SDL2_image/include/SDL2
libs_path := -L./libs/SDL2/lib -L./libs/SDL2_image/lib/
SDL2_config := ./libs/SDL2/bin/sdl2-config

all:
	gcc $(srcs) -Wl,-subsystem,windows ${libs_include} -I${headers} ${libs_path} -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o main