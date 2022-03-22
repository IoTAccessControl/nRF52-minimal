#include "fun.h"

static int test_func1(int op1, int op2);
static int test_func2(int op1);

// __attribute__((at(0x5000)))  
func_api_t func_exposed_api  __attribute__((section("lib_func_test"))) = {
	.func_test1 = test_func1,
	.func_test2 = test_func2
};

int g_value[] = {1, 2, 3, 4, 5};

static int test_func1(int op1, int op2) {
	return op1 + op2;
}

static int test_func2(int op1) {
	return op1 + g_value[op1 % 5];
}