#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
	double x0, x1;
	int n;

	if (fp == NULL)
		return;

	if(!scanf("%lf", &x0)) return;
	n = 0;
	
	const char* arr[3] = { "n", "xn1", "|f(xn1)|" };
	printf("%s %18s %25s\n", arr[0], arr[1], arr[2]);
	fprintf(fp, "%s %18s %25s\n", arr[0], arr[1], arr[2]);
	//printf("n xn1 |f(xn1)|\n");
	do {
		if(n != 0) x0 = x1;
		x1 = x0 - (_f(x0) / _fp(x0));
		printf("%d\t%.15e\t%.15e\n", n, x0, fabs(_f(x0)));
		fprintf(fp, "%d\t%.15e\t%.15e\n", n, x0, fabs(_f(x0)));
		n++; 
	}while (n < Nmax && fabs(_f(x1)) >= DELTA && fabs(x1 - x0) >= EPSILON);
}
