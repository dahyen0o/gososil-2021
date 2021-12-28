#include "my_solver.h"
#define MY_RAND_MAX 32767

void program2_2()
{
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	int sampleNum, i, j;
	double n_h, integralVal;
	if (!fscanf(fp_r, "%d %lf", &sampleNum, &n_h)) exit(-1);

	double** pdf = (double**)malloc(sizeof(double*) * sampleNum);
	double** cdf = (double**)malloc(sizeof(double*) * sampleNum);
	double** f = (double**)malloc(sizeof(double*) * sampleNum);

	for (i = 0; i < sampleNum; i++) {
		pdf[i] = (double*)malloc(sizeof(double) * 2);
		cdf[i] = (double*)malloc(sizeof(double) * 2);
		f[i] = (double*)malloc(sizeof(double) * 2);

		if (!fscanf(fp_r, "%lf %lf", &pdf[i][0], &pdf[i][1])) exit(-1);
		cdf[i][0] = f[i][0] = pdf[i][0];
	}

	for (j = 0; j < sampleNum; j++) {
		integralVal = 0;

		for (i = 0; i <= j; i++)
			integralVal += pdf[i][1];

		for (i = 1; i <= j - 1; i++)
			integralVal += pdf[i][1];

		integralVal *= 0.5 * n_h;
		cdf[j][1] = integralVal;
	}

	double ui;
	int numRand;

	srand(unsigned int(time(NULL)));
	if (!scanf("%d", &numRand)) exit(-1);
	fprintf(fp_w, "%d\n", numRand);

	for (i = 0; i < numRand; i++) {
		ui = rand() / (double)(MY_RAND_MAX + 1);
		while (ui <= 0 && ui >= 1) // ui in (0, 1)
			ui = rand() / (double)(MY_RAND_MAX + 1);

		for (j = 0; j < sampleNum; j++)
			f[j][1] = cdf[j][1] - ui;

		// bisection
		int n = 0;
		double a, b, fa, fb; // f(a) = fa, f(b) = fb
		double x0, x1, fx0, fx1; // f(x0) = fx0, f(x1) = fx1
		double temp;

		a = f[0][0]; fa = f[0][1];
		b = f[sampleNum - 1][0]; fb = f[sampleNum - 1][1];
		x0 = (a + b) / 2;

		// get j(int)
		for (j = 0; j < sampleNum - 1; j++) {
			if (x0 >= f[j][0] && x0 <= f[j + 1][0])
				break;
		}

		temp = (pdf[j + 1][1] - pdf[j][1]) / (pdf[j + 1][0] - pdf[j][0]);
		temp *= ((x0 - pdf[j][0]) / 2);
		temp += pdf[j][1];
		temp *= (x0 - pdf[j][0]);

		fx0 = cdf[j][1] + temp - ui;

		if (fa * fx0 < 0) {
			b = x0;
			fb = fx0;
		}
		else {
			a = x0;
			fa = fx0;
		}

		do {
			if (n != 0) {
				x0 = x1;
			}

			x1 = (a + b) / 2;

			for (j = 0; j < sampleNum - 1; j++) {
				if (x1 >= f[j][0] && x1 <= f[j + 1][0])
					break;
			}

			temp = (pdf[j + 1][1] - pdf[j][1]) / (pdf[j + 1][0] - pdf[j][0]);
			temp *= ((x1 - pdf[j][0]) / 2);
			temp += pdf[j][1];
			temp *= (x1 - pdf[j][0]);

			fx1 = cdf[j][1] + temp - ui;

			if (fa * fx1 < 0) {
				b = x1;
				fb = fx1;
			}
			else {
				a = x1;
				fa = fx1;
			}
			n++;
		} while (fabs(fx1) >= DELTA && n < Nmax && fabs(x1 - x0) >= EPSILON);

		//printf("%.15lf\n", x1);
		fprintf(fp_w, "%.15lf\n", x1);
	}

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
