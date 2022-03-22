#ifndef FUN_H_
#define FUN_H_

typedef struct func_api_t {
	int (*func_test1)(int op1, int op2);
	int (*func_test2)(int op1);
} func_api_t;


#endif
