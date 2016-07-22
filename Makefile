.DEFAULT_GOAL=all


MODULE=hashtable

DIR_BIN=bin
DIR_OBJ=obj
DIR_SRC=src

# source files, with directory and extension
SOURCES=$(wildcard $(DIR_SRC)/*.c)
# target names of object files, no directory
OBJECTS=$(patsubst %.c, %.o, $(notdir $(SOURCES)))


all: static shared

static: $(OBJECTS)
	ar rcs $(DIR_BIN)/lib$(MODULE).a $(DIR_OBJ)/$^

shared:

%.o: $(DIR_SRC)/%.c
	gcc -c $^ -o $(DIR_OBJ)/$@ -Wall

clean:
	rm -rf $(DIR_OBJ)/* $(DIR_BIN)/*

