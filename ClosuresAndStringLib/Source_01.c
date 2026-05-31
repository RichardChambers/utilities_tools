// See stackoverflow post
// https://stackoverflow.com/questions/4393716/is-there-a-a-way-to-achieve-closures-in-c

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>

#define NAME_CLOSURE(name,t) ClosureStruct_ ## name ## t

#define DEF_CLOSURE(name,t) \
typedef struct {      \
t (*p) ();        \
size_t  sargs;        \
size_t  cargs;        \
unsigned char* args;  \
} NAME_CLOSURE(name,t);

typedef struct {
	void (*p)();            // pointer to the function of this closure
	size_t  sargs;          // size of the memory area allocated for closure data
	size_t  cargs;          // current memory area in use for closure data
	unsigned char* args;   // pointer to the allocated closure data area
} ClosureStruct;

void* makeClosure(void (*p)(), size_t sargs)
{
	// allocate the space for the closure management data and the closure data itself.
	// we do this with a single call to calloc() so that we have only one pointer to
	// manage.
	ClosureStruct* cp = calloc(1, sizeof(ClosureStruct) + sargs);

	if (cp) {
		cp->p = p;             // save a pointer to the function
		cp->sargs = sargs;     // save the total size of the memory allocated for closure data
		cp->cargs = 0;         // initialize the amount of memory used
		cp->args = (unsigned char*)(cp + 1);  // closure data is after closure management block
	}

	return cp;
}

void* pushClosureArg(void* cp, size_t sarg, void* arg)
{
	if (cp) {
		ClosureStruct* p = cp;
		if (p->cargs + sarg <= p->sargs) {
			// there is room in the closure area for this argument so make a copy
			// of the argument and remember our new end of memory.
			memcpy(p->args + p->cargs, arg, sarg);
			p->cargs += sarg;
		}
	}

	return cp;
}

// example functions that we will use with closures

#define MY_NAME_CLOSURE  NAME_CLOSURE(,int)

DEF_CLOSURE(, int)

// funcadd() is a function that accepts a closure with two int arguments
// along with three additional int arguments.
// it is similar to the following function declaration:
//  void funcadd(int x1, int x2, int a, int b, int c);
//
void funcadd(ClosureStruct* cp, int a, int b, int c)
{
	// using the variable argument functionality we will set our
	// variable argument list address to the closure argument memory area
	// and then start pulling off the arguments that are provided by the closure.
	va_list jj;
	va_start(jj, cp->args);    // get the address of the first argument

	int x1 = va_arg(jj, int);    // get the first argument of the closure
	int x2 = va_arg(jj, int);

	printf("  funcadd(): x1 = %d and x2 = %d with additional args a = %d  b = %d  c = %d\n", x1, x2, a, b, c);
}

int zFunc(MY_NAME_CLOSURE * cp, int j, int k)
{
	int  iRet = 0;
	va_list jj;

	va_start(jj, cp->args);    // get the address of the first argument
	int i = va_arg(jj, int);

	iRet = i + j + k;

	printf("  zFunc() i = %d, j = %d, k = %d and returns %d\n", i, j, k, iRet);

	return iRet;
}

typedef struct { char xx[24]; } thing1;

int z2func(MY_NAME_CLOSURE * cp, int i)
{
	va_list jj;

	va_start(jj, cp->args);    // get the address of the first argument
	thing1 a = va_arg(jj, thing1);

	printf("  z2func() i = %d, %s returns 0\n", i, a.xx);
	return 0;
}

int main_Source_01(void)
{

	ClosureStruct* p;
	MY_NAME_CLOSURE * pint;

	int x;
	thing1 xpxp = { "1234567890123" };

	printf("closure on void function funcadd() pushing two int arguments of 4 and 10.\n");

	p = makeClosure(funcadd, 256);
	x = 4;  pushClosureArg(p, sizeof(int), &x);
	x = 10;  pushClosureArg(p, sizeof(int), &x);

	p->p(p, 1, 2, 3);

	free(p);

	printf("closure on int function z2func() pushing struct thing1 with 45.\n");

	pint = makeClosure(z2func, sizeof(thing1));
	pushClosureArg(pint, sizeof(thing1), &xpxp);

	int k = pint->p(pint, 45);
	printf("  first k = %d\n", k);

	free(pint);

	printf("closure on int function zfunc() pushing single int argument of 5 with 12 and 7.\n");

	pint = makeClosure(zFunc, sizeof(int));
	x = 5; pushClosureArg(pint, sizeof(int), &x);

	k = pint->p(pint, 12, 7);
	printf("  second k = %d\n", k);


	return 0;
}
