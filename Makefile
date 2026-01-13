CC := gcc
SRC_PATH := src
BUILD_PATH := build
TEST_PATH := ${SRC_PATH}/tests
CFLAGS += -Wall -std=c99 -pedantic -lpthread -Iinclude
SOURCES:= $(foreach dir,$(SRC_PATH),$(wildcard $(SRC_PATH)/*.c ))
TEST_SOURCES := $(foreach dir, $(TEST_PATH),$(wildcard $(TEST_PATH)/*.c ))
OBJECTS = $(SOURCES:%.c=%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.c=%.o)
TEST_OBJECTS := $(subst $(TEST_PATH),$(BUILD_PATH),$(TEST_OBJECTS))
OBJECTS := $(subst $(SRC_PATH),$(BUILD_PATH),$(OBJECTS))
DEPS = $(SOURCES:%.c=%.d)

$(info ${SOURCES})
$(info ${TEST_OBJECTS})
$(info ${OBJECTS})
$(info ${DEPS})

all: build make_obj
run_tests: build test run_test clean

build: 
	@mkdir -p build

test: ${TEST_OBJECTS} ${OBJECTS}
	${CC} -o ${BUILD_PATH}/$@ $^ 

run_test:
	${BUILD_PATH}/test
	
${BUILD_PATH}/tests.o: ${TEST_PATH}/tests.c 
	${CC} -o $@ $^ -c -Iinclude

${BUILD_PATH}/pthread_groups.o: ${SRC_PATH}/pthread_groups.c
	${CC} -o $@ $^ -c -Iinclude

${BUILD_PATH}/queue.o: ${SRC_PATH}/queue.c
	${CC} -o $@ $^ -c -Iinclude

${BUILD_PATH}/commons.o: ${SRC_PATH}/commons.c
	${CC} -o $@ $^ -c -Iinclude

${BUILD_PATH}/dictionary.o: ${SRC_PATH}/dictionary.c
	${CC} -o $@ $^ -c -Iinclude

.PHONY: clean

clean:
	rm -rf $(BUILD_PATH)

