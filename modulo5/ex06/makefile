# A Makefile with variables and suffix rules
# ------------------------------------------
# variaveis
#----------
INCLUDES = 
SOURCES = main.c
OBJFILES = main.o
EXEC = progrm
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
