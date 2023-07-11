# The changer that changes the color in yaml-files

SRC = changer.cpp
# OBJ = ${SRC:.c=.o}

CXX = g++
LIBS = -lyaml-cpp
CXXFLAGS = -O3 -pedantic -Wall -Wextra

all: options changer

options:
	@echo changer build options:
	@echo "CXXFLAGS = ${CXXFLAGS}"
	@echo "LIBS   = ${LIBS}"
	@echo "CXX     = ${CXX}"

changer:
	${CXX} -o $@ ${SRC} ${CXXFLAGS} ${LIBS}

clean:
	rm -f changer changer-2.0

.PHONY: all clean options changer
