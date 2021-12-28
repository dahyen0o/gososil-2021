#include "my_solver.h"

void program2_1()
{
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	double resultTime = 0;

	fp_r = fopen("sampling_table.txt", "r");
	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/

	int sampleNum, i, j;
	double h, n_h;

	if(!fscanf(fp_r, "%d %lf", &sampleNum, &h)) exit(-1);

	double** arr = (double **)malloc(sizeof(double *) * sampleNum);
	for (i = 0; i < sampleNum; i++){
		*(arr + i) = (double *)malloc(sizeof(double) * 2);
		if(!fscanf(fp_r, "%lf %lf", *(arr + i) + 0, *(arr + i) + 1)) exit(-1);
	}

	double x0 = arr[0][0], xn = arr[sampleNum - 1][0];
	double diff = xn - x0;

	// normalize
	for (i = 0; i < sampleNum; i++)
		arr[i][0] = (arr[i][0] - x0) / (xn - x0);
	n_h = h / (xn - x0);

	// get integral value (use nemo)
	double integralVal = 0;
	for (i = 1; i < sampleNum - 1; i++)
		integralVal += 2 * arr[i][1];
	integralVal += arr[0][1] + arr[sampleNum - 1][1];
	integralVal *= n_h * 0.5;

	// Px(x)
	for (i = 0; i < sampleNum; i++)
		arr[i][1] /= integralVal;

	// integrate again
	integralVal = arr[0][1] + arr[sampleNum - 1][1];
	for (i = 1; i < sampleNum - 1; i++)
		integralVal += 2 * (arr[i][1]);
	integralVal *= 0.5 * n_h;

	printf("*** Integrating the pdf from 0.0 to 1.0 = %lf\n", integralVal);

	// print
	double x[5] = {0.0, 0.25, 0.5, 0.75, 1.0};
	int startI, lastI;

	for (i = 0, lastI = 0; i < 4;i++) {
		integralVal = 0;
		
		if(i != 3){
			for (j = lastI; arr[j][0] <= x[i + 1]; j++)
				integralVal += arr[j][1];
		}
		else {
			for (j = lastI; j < sampleNum; j++)
				integralVal += arr[j][1];
		}

		startI = lastI + 1;
		lastI = j - 1;
		
		for (j = startI; j < lastI; j++)
			integralVal += arr[j][1];
		integralVal *= 0.5 * n_h;

		printf("*** Integrating the pdf from %.2g to %.2g = %lf\n", x[i], x[i + 1], integralVal);
	}

	fprintf(fp_w, "%d %.15lf\n", sampleNum, n_h);
	
	for (i = 0; i < sampleNum; i++)
		fprintf(fp_w, "%.15lf %.15lf\n", arr[i][0], arr[i][1]);

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
