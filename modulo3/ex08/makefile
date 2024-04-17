# A Makefile with variables and suffix rules
# ------------------------------------------
# variaveis
#----------
INCLUDES = record_t.h
SOURCES = main.c
OBJFILES = main.o
EXEC = ex01
# Suffix rules
#--------------
.SUFFIXES : .c .o
.c.o:
	gcc -g -Wall -Wextra -fanalyzer -c $<

${EXEC}: ${OBJFILES}
	gcc ${OBJFILES} -o ${EXEC}

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
# EOF #
#######
