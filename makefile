OUT_DIR = out
SRC_DIR = src
CC = gcc
CFLAGS = `sdl2-config --cflags --libs`

NAME = automata

all: $(SRC_DIR)/main.c $(SRC_DIR)/gui.c $(SRC_DIR)/grid.c
	$(CC) $(SRC_DIR)/main.c $(SRC_DIR)/gui.c $(SRC_DIR)/grid.c -o $(OUT_DIR)/$(NAME) $(CFLAGS)
	./$(OUT_DIR)/$(NAME)


clean:
	rm $(OUT_DIR)/*
