#include "my_solver.h"

#include <string.h>
#define indexArr(row, col, dim) row * dim + col

void program3_4(void)
{
	FILE* fp_r, * fp_w;

	float *Aarr, *Barr, *Xarr, *Sarr;
	float *A_copy, *B_copy;
	float *error;

	int *Larr;
	int n, IA, info;
	int i, j;

	fp_r = fopen("linear_system_3-4.txt", "r");
	fp_w = fopen("solution_3-4.txt", "w");
	if (!fp_r) exit(-1);

	fscanf(fp_r, "%d", &n);

	Aarr = (float *)malloc(sizeof(float) * (n * n));
	Barr = (float *)malloc(sizeof(float) * n);
	Xarr = (float *)calloc(n, sizeof(float));
	Sarr = (float *)calloc(n, sizeof(float));
	Larr = (int *)calloc(n, sizeof(int));

	A_copy = (float *)malloc(sizeof(float) * (n * n));
	B_copy = (float *)malloc(sizeof(float) * n);

	error = (float *)calloc(n, sizeof(float));

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fscanf(fp_r, "%f", &Aarr[j * n + i]);
			A_copy[j * n + i] = Aarr[j * n + i];
		}
	}

	for (i = 0; i < n; i++) {
		fscanf(fp_r, "%f", &Barr[i]);
		B_copy[i] = Barr[i];
	}

	IA = n;

	//
	info = gespp_(&n, Aarr, &IA, Larr, Sarr);
	info = solve_(&n, Aarr, &IA, Larr, Barr, Xarr);

	fprintf(fp_w, "%d\n", n);
	for (i = 0; i < n; i++) fprintf(fp_w, "%.6e\n", Xarr[i]);

	float sum = 0.0; double temp = 0.0;

	for (i = 0; i < n; i++) {
		for (sum = 0.0, j = 0; j < n; j++) 
			sum += A_copy[indexArr(j, i, n)] * Xarr[j];
	
		error[i] = sum - B_copy[i];
	}

	for (sum = 0.0, i = 0; i < n; i++) {
		sum += pow(error[i], 2);
		temp += pow(B_copy[i], 2);
	}

	fprintf(fp_w, "\n%.6e\n", sqrt(sum) / sqrt(temp));
	printf("Error: %.6e\n", sqrt(sum) / sqrt(temp));

	free(Aarr); free(Barr); free(Xarr); free(Sarr); free(Larr);
	free(error);
	free(A_copy); free(B_copy);

	fclose(fp_r);
	fclose(fp_w);
}