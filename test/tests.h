#ifndef TESTS_H
#define TESTS_H

// Function pointer type for test functions
typedef int (*TestFunc)(void);

// Structure for holding test information
typedef struct {
    const char *description;  // Description of the test
    TestFunc test_func;       // Pointer to the test function
} test_t;

// Prototype for the main test execution function
void test();

#endif // TESTS_H

