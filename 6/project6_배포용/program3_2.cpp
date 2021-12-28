#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_2(int* n, double* x, double* fvec, int* iflag)
{	
	fvec[0] = x[1] + 10 * x[2] + 9;
	fvec[1] = sqrt(5) * (x[3] - x[0]) - 2 * sqrt(5);
	fvec[2] = pow(x[2] - 2 * x[3], 2) - 9;
	fvec[3] = sqrt(10) * pow(x[1] - x[0], 2) - 2 * sqrt(10);
}

void program3_2(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9, -0.9, 1.25, -1.25 };	
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-2.txt", "w");
	if (!fp_w) exit(-1);

	fprintf(fp_w, "x0 = (0.9, -0.9, 1.25, -1.25)\n");

	hybrd1_(fcn3_2, &n, x, fvec, &tol, &info, wa, &lwa);

	if (info == 1) {
		fprintf(fp_w, "w = %lf, x = %lf, y = %lf, z = %lf\n", x[0], x[1], x[2], x[3]);

		fprintf(fp_w, "Accuration(f(x)): ");
		for (int k = 0; k < SOLNUMS; k++)
			fprintf(fp_w, "%lf ", fabs(fvec[k]));
		fprintf(fp_w, "\n");
	}
	else {
		printf("Wrong\n");
		fprintf(fp_w, "Wrong\n\n");
	}

	/********************************/

	fclose(fp_w);
	printf("program3_2 ended.\n");
}