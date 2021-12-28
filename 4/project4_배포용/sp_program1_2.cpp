#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	float x0, x1, temp;
	int n;

	if (fp == NULL)
		return;

	if (!scanf("%f %f", &x0, &x1)) return;
	n = 0;

	const char *arr[3] = {"n", "xn1", "|f(xn1)|"};
	printf("%s %6s       %3s\n", arr[0], arr[1], arr[2]);
	fprintf(fp, "%s %18s %25s\n", arr[0], arr[1], arr[2]);
	//printf("n xn1 |f(xn1)|\n");
	do {
		temp = x1;
		x1 = x0 - _sp_f(x0) * ((x1 - x0) / (_sp_f(x1) - _sp_f(x0)));
		printf("%d %.15e %.15e\n", n, x0, (float)fabs(_sp_f(x0)));
		fprintf(fp,"%d %.15e %.15e\n", n, x0, (float)fabs(_sp_f(x0)));
		x0 = temp;
		n++;
	} while (n < Nmax && fabs(_sp_f(x1)) >= DELTA && fabs(x1 - x0) >= EPSILON);

}
