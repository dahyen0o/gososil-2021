#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	float x0, x1;
	int n;

	if (fp == NULL)
		return;

	if (!scanf("%f", &x0)) return;
	n = 0;

	const char* arr[3] = { "n", "xn1", "|f(xn1)|" };
	printf("%s %18s %25s\n", arr[0], arr[1], arr[2]);
	fprintf(fp, "%s %18s %25s\n", arr[0], arr[1], arr[2]);
	do {
		if (n != 0) x0 = x1;
		x1 = x0 - (_sp_f(x0) / _sp_fp(x0));
		printf("%d\t%.15e\t%.15e\n", n, x0, (float)fabs(_sp_f(x0)));
		fprintf(fp, "%d\t%.15e\t%.15e\n", n, x0, (float)fabs(_sp_f(x0)));
		n++;
	} while (n < Nmax && fabs(_sp_f(x1)) >= DELTA && fabs(x1 - x0) >= EPSILON);

}
