CC = gcc
FLAGS = -g  -lncurses -lsqlite3 -o
SOURCE_FILES = ./src/main.c ./src/db.c
EXECUTABLE = ./build/Hygieia


main: $(SOURCE_FILES)
	@mkdir -p ./build
	$(CC) $(SOURCE_FILES) $(FLAGS) $(EXECUTABLE)

run: main
	./$(EXECUTABLE)
