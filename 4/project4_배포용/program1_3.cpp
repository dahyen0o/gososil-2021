#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double a0, b0, x0, x1 , temp;
	int n;

	if (fp == NULL) return;
	if (!scanf("%lf %lf", &a0, &b0)) return;

	n = 0;
	const char* arr[3] = { "n", "xn1", "|f(xn1)|" };
	printf("%s %18s %25s\n", arr[0], arr[1], arr[2]);
	do {
		x0 = (a0 + b0) / 2;
		printf("%d\t%.15f\t%.15e\n", n, x0, fabs(_f(x0)));
		if (_f(a0) * _f(x0) < 0) b0 = x0;
		else a0 = x0;

		x1 = (a0 + b0) / 2;
		n++;
	} while (n < Nmax && fabs(_f(x1)) >= DELTA && fabs(x1 - x0) >= EPSILON);

}