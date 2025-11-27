#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TEST_SUITE_ON
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BEGIN_TESTS int main(int argc,char *argv[]){ unsigned int test_counter = 0;unsigned int test_passed = 0;
#define TEST_ASSERTION(OUTPUT,EXPECTED_VALUE) if(OUTPUT == EXPECTED_VALUE)\
	{ printf(ANSI_COLOR_GREEN "_____ Test passed _____"ANSI_COLOR_RESET "\n");\
		test_passed ++; \
	}else\
	printf(ANSI_COLOR_RED"_____ Test failed _____"ANSI_COLOR_RESET "\n");

#define TEST(TEST_NAME)  \
	printf("\n"ANSI_COLOR_CYAN "Exectuting test" ANSI_COLOR_RESET "\n" ANSI_COLOR_CYAN "___________ %s ___________" ANSI_COLOR_RESET "\n", TEST_NAME);\
	test_counter++ ;\
	if (run_test(TEST_NAME,argc,argv))

#define END_TESTS \
	printf("\n" ANSI_COLOR_GREEN "Test Passed: %u" ANSI_COLOR_RESET "\n",test_passed);\
	printf(ANSI_COLOR_RED "Test Failed: %u" ANSI_COLOR_RESET "\n",test_counter-test_passed);\
	printf(ANSI_COLOR_CYAN "Test Run: %u" ANSI_COLOR_RESET "\n",test_counter);\
	if((test_counter - test_passed)> 0)return 1;\
	return 0;}

#endif

int run_test(const char test_name[], int argc, char* argv[]) {
    // we run every test by default
    if (argc == 1) {
			return 1; }
    // else we run only the test specified as a command line argument
    for (int i = 1; i < argc; i++) {
        if (!strcmp(test_name, argv[i])) { 

					return 1; }

    }
    return 0;
}
