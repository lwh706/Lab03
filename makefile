CC=gcc
SRC_DIR=src
BIN_DIR=bin
LIB_DIR=lib
INCLUDE_DIR=include
CFLAG=-Wall -m64
SRC_FILES= \
	${SRC_DIR}/Lab03.c


.PHONY: clean rebuild

Lab03: ${SRC_FILES}
	${CC} ${CFLAG} ${SRC_FILES} -lpthread -o ${BIN_DIR}/Lab03

clean:
	rm ./test_員工編號.txt
	rm ${BIN_DIR}/Lab03

rebuild: clean Lab03
