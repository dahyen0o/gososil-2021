#include "my_solver.h"

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i;

	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%d", &DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);


		/********************************/

		for (i = 0; i < NCOEF; i++) {
			if(!fscanf(fp_r, "%lf", &poly[i])) continue;
		}

		rpoly_(poly, &DEGREE, zeror, zeroi, &fail);

		if (fail) {
			printf("# Wrong at %s\n", readfile);
			fprintf(fp_w, "# Wrong\n");
			continue;
		}

		fprintf(fp_w, "zeror: ");
		for (i = 0; i < DEGREE; i++) fprintf(fp_w, "%f ", zeror[i]);
		fprintf(fp_w, "\n");

		fprintf(fp_w, "zeroi: ");
		for (i = 0; i < DEGREE; i++) fprintf(fp_w, "%f ", zeroi[i]);
		fprintf(fp_w, "\n\n");

		double sum[2]; int k;
		double temp[2];

		for (k = 0; k < DEGREE; k++) { 
			// (a+bi)(c+di) = (ac-bd)+(ad+bc)i
			sum[0] = poly[NCOEF - 1]; sum[1] = 0.0;

			double t0, t1;
			for (int p = 0;p < DEGREE;p++) { 
				temp[0] = zeror[k]; temp[1] = zeroi[k];

				for (int pp = p + 1; pp < DEGREE; pp++) {
					t0 = temp[0]; t1 = temp[1];
					temp[0] = t0 * zeror[k] - t1 * zeroi[k];
					temp[1] = t0 * zeroi[k] + t1 * zeror[k];
					
				}
				sum[0] += poly[p] * temp[0];
				sum[1] += poly[p] * temp[1];
			}
			fprintf(fp_w, "f(%lf+%lfi) = (%lf)+(%lf)i\n\n", zeror[k], zeroi[k], sum[0], sum[1]);
		}

		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}