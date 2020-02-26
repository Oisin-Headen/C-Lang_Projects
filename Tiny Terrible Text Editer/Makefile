HEADERS=picedit_line.h picedit_line_list.h picedit_main.h picedit_command.h \
	picedit_shared.h picedit_utility.h

SOURCES=picedit_line.c picedit_line_list.c picedit_main.c picedit_command.c \
	picedit_utility.c

OBJECTS=picedit_line.o picedit_line_list.o picedit_main.o picedit_command.o \
	picedit_utility.o

picedit: $(OBJECTS) 
	$(CC) $(LFLAGS) $^ -o $@

archive:
	zip $(USER)-a2i $(HEADERS) $(SOURCES) picedit_readme.txt Makefile

CC=gcc

CFLAGS = -ansi -Wall -pedantic -g

LFLAGS=


%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean: 
	@rm -f *.o picedit
