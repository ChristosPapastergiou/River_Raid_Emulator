LIB = ../lib
MODULES = ../modules
INCLUDE = ../include

CC = gcc

CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

OBJS = game.o interface.o $(MODULES)/state.o $(LIB)/game.a $(LIB)/libraylib.a

EXEC = game

ARGS =

include $(LIB)/libraylib.mk


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)