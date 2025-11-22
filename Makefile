CC := gcc
SRC_PATH := src
BUILD_PATH := build
CFLAGS += -Wall -std=c99 -pedantic -lpthread -Iinclude
SOURCES:= $(foreach dir,$(SRC_PATH),$(wildcard $(SRC_PATH)/*.c $(dir)/*.c))
OBJECTS = $(SOURCES:%.c=%.o)
DEPS = $(SOURCES:%.c=%.d)

$(info ${SOURCES})
$(info ${OBJECTS})
$(info ${DEPS})

all: build make_obj
run_tests: build test run_test clean

build:
	@mkdir -p build

test: ${OBJECTS}
	${CC} -o ${BUILD_PATH}/$@ $^ 

run_test:
	${BUILD_PATH}/test
	

make_obj: ${BUILD_PATH}/pthread_groups.o

${BUILD_PATH}/tests.o: ${SRC_PATH}/tests.c
	${CC} -o $@ $^ -c -Iinclude
${BUILD_PATH}/pthread_groups.o: ${SRC_PATH}/pthread_groups.c
	${CC} -o $@ $^ -c

.PHONY: clean

clean:
	rm -rf $(BUILD_PATH)

