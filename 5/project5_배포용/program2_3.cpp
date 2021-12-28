#include "my_solver.h"
#define MY_RAND_MAX 32767

// integral (0, 1)
double integral01(double** pdf, double a, double b, double size) {
	double integralVal = 0.0;
	double n_h = pdf[1][0] - pdf[0][0];
	int i;

	for (i = 0; pdf[i][0] < a;i++);

	for (; i < size - 1 && pdf[i + 1][0] <= b;i++)
		integralVal += 0.5 * n_h * (pdf[i][1] + pdf[i + 1][1]);
	
	if (pdf[i][0] != b && i < 99)
		integralVal += (b - pdf[i][0]) * (pdf[i][1] + (pdf[i + 1][1] - pdf[i][1]) / n_h * (b - pdf[i][0]));

	return integralVal;
}

// integral (0, xn)
double integralab(double** pdf, double b, double size) {
	double integralVal = 0.0;
	double n_h = pdf[1][0] - pdf[0][0];
	int i;

	for (i = 0; i <= b; i++) integralVal += pdf[i][1];
	for (i = 1; i <= b - 1; i++) integralVal += pdf[i][1];
	integralVal *= 0.5 * n_h;

	return integralVal;
}

double integralmini(double** pdf, int j, double x) {
	double temp;

	temp = (pdf[j + 1][1] - pdf[j][1]) / (pdf[j + 1][0] - pdf[j][0]);
	temp *= ((x - pdf[j][0]) / 2);
	temp += pdf[j][1];
	temp *= (x - pdf[j][0]);

	return temp;
}

// HOMEWORK
void program2_3()
{
	int numHist, numRand, sampleNum, i, j;
	double n_h, *rands, **pdf;
	int *histogram;

	FILE* fp_p = fopen("pdf_table.txt", "r");
	FILE* fp_r = fopen("random_event_table.txt", "r");
	FILE* fp_w = fopen("histogram.txt", "w");

	if (!fscanf(fp_p, "%d %lf", &sampleNum, &n_h)) exit(-1);

	pdf = (double **)malloc(sizeof(double *) * sampleNum);
	for (i = 0; i < sampleNum; i++) {
		pdf[i] = (double*)malloc(sizeof(double) * 2);
		if (!fscanf(fp_p, "%lf %lf", &pdf[i][0], &pdf[i][1])) exit(-1);
	}

	printf("Type number of histogram index\n");
	if(!scanf("%d", &numHist)) exit(-1);
	histogram = (int *)calloc(numHist, sizeof(int));

	if(!fscanf(fp_r, "%d", &numRand)) exit(-1);
	rands = (double *)malloc(sizeof(double) * numRand);

	for (i = 0; i < numRand; i++)
		if(!fscanf(fp_r, "%lf", &rands[i])) exit(-1);

	for (i = 0; i < numRand; i++) {
		for (j = 0; j < numHist; j++) {
			if (rands[i] - (double)j / numHist < 1.0 / numHist) {
				histogram[j]++;
				break;
			}
		}
	}

	fprintf(fp_w, "# range: count(ratio) <- integral\n");
	printf("# range: count(ratio)\n");

	for (i = 0; i < numHist; i++) {
		double a = (double)i / numHist;
		double b = ((double)i + 1) / numHist;
		double integral = integral01(pdf, a, b, sampleNum);
		
		fprintf(fp_w, "%lf - %lf: %4d (%lf) <- %lf\n", a, b, histogram[i], (double)histogram[i] / numRand, integral);
		printf("%lf - %lf: %4d (%lf) <- %lf\n", a, b, histogram[i], (double)histogram[i] / numRand, integral);
	}

	fclose(fp_p);
	fclose(fp_r);
	fclose(fp_w);
}

// HOMEWORK
void program2_2_a()
{
	__int64 start, freq, end;
	float resultTime = 0;

	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	//fp_w = fopen("random_event_table.txt", "w");

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
		cdf[j][1] = integralab(pdf, (double)j, sampleNum);
	}

	double ui;
	int numRand;

	srand(unsigned int(time(NULL)));
	printf("Type num of rand numbers using Bisection.\n");
	if (!scanf("%d", &numRand)) exit(-1);

	CHECK_TIME_START;

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

		fx0 = cdf[j][1] + integralmini(pdf, j, x0) - ui;

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

			fx1 = cdf[j][1] + integralmini(pdf, j, x1) - ui;

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
	}

	CHECK_TIME_END(resultTime);

	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);
}

void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;

	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	//fp_w = fopen("random_event_table.txt", "w");

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
		cdf[j][1] = integralab(pdf, (double)j, sampleNum);
	}

	double ui;
	int numRand;

	srand(unsigned int(time(NULL)));
	printf("Type num of rand numbers using Secant.\n");
	if (!scanf("%d", &numRand)) exit(-1);

	CHECK_TIME_START;

	for (i = 0; i < numRand; i++) {
		ui = rand() / (double)(MY_RAND_MAX + 1);
		while (ui <= 0 && ui >= 1) // ui in (0, 1)
			ui = rand() / (double)(MY_RAND_MAX + 1);

		for (j = 0; j < sampleNum; j++)
			f[j][1] = cdf[j][1] - ui;

		// secant
		int n = 0;
		double a, b, fa, fb; // f(a) = fa, f(b) = fb
		double temp;
		double x, fx;

		a = f[0][0]; fa = f[0][1];
		b = f[sampleNum - 1][0]; fb = f[sampleNum - 1][1];

		do{
			if (n > 0) {
				a = b; fa = fb;
				b = x; fb = fx;
			}

			x = b - fb * ((b - a) / (fb - fa));

			if (x < 0) x = 0;

			// set j
			for (j = 0; j < sampleNum - 1; j++) {
				if (x >= f[j][0] && x <= f[j + 1][0])
					break;
			}

			if (j == sampleNum - 1) {
				j--; x = 1.0;
			}

			fx = cdf[j][1] + integralmini(pdf, j, x) - ui;
			
			n++;

		} while (n < Nmax && fabs(fx) >= DELTA && fabs(x - b) >= EPSILON);
		
		//printf("%.15lf\n", b);
	}

	CHECK_TIME_END(resultTime);

	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);

	/* ***** */
	CHECK_TIME_START;

	for (i = 0; i < numRand; i++)
	{
		ui = rand() / (double)(MY_RAND_MAX + 1);
		while (ui <= 0 && ui >= 1) // ui in (0, 1)
			ui = rand() / (double)(MY_RAND_MAX + 1);

		for (j = 0; j < sampleNum; j++)
			f[j][1] = cdf[j][1] - ui;

		double a, b, fa, fb; // f(a) = fa, f(b) = fb
		double x0, x1, fx0, fx1; // f(x0) = fx0, f(x1) = fx1
		double temp;

		int n = 0;

		x0 = f[sampleNum / 2][0]; fx0 = f[sampleNum / 2][1];
		if (x0 < 0) x0 = 0;

		for (j = 0; j < sampleNum - 1; j++){
			if (x0 >= f[j][0] && x0 <= f[j + 1][0]) break;
		}
		if (j == sampleNum - 1){
			j--; x0 = 1.0;
		}

		double s, fpx0;

		s = (x0 - f[j][0]) / (f[j + 1][0] - f[j][0]);

		do {
			if (n > 0) {
				x0 = x1; fx0 = fx1;
			}

			fpx0 = (1 - s) * pdf[j][1] + s * pdf[j + 1][1];

			x1 = x0 - (fx0 / fpx0);
			if (x1 < 0) x1 = 0;

			for (j = 0; j < sampleNum - 1; j++)	{
				if (x1 >= f[j][0] && x1 <= f[j + 1][0])
					break;
			}

			if (j == sampleNum - 1) {
				j--; x1 = 1.0;
			}

			s = (x1 - f[j][0]) / (f[j + 1][0] - f[j][0]);

			fx1 = cdf[j][1] + integralmini(pdf, j, x1) - ui;

			n++;

		} while (n < Nmax && fabs(fx1) >= DELTA && fabs(x1 - x0) >= EPSILON);
	}

	CHECK_TIME_END(resultTime);
	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);

	// newton-raphson with bisection
	CHECK_TIME_START;
	
	for (i = 0; i < numRand; i++){
		ui = rand() / (double)(MY_RAND_MAX + 1);
		while (ui <= 0 && ui >= 1) // ui in (0, 1)
			ui = rand() / (double)(MY_RAND_MAX + 1);

		for (j = 0; j < sampleNum; j++)
			f[j][1] = cdf[j][1] - ui;

		// bisection first

		double a, b, fa, fb; // f(a) = fa, f(b) = fb
		double x0, x1, fx0, fx1; // f(x0) = fx0, f(x1) = fx1
		double temp;

		int n = 0;

		a = f[0][0]; fa = f[0][1];
		b = f[sampleNum - 1][0]; fb = f[sampleNum - 1][1];

		x0 = (a + b) / 2.0;

		for (j = 0; j < sampleNum - 1; j++){
			if (x0 >= f[j][0] && x0 <= f[j + 1][0])
				break;
		}

		fx0 = cdf[j][1] + integralmini(pdf, j, x0) - ui;

		if (fa * fx0 < 0) {
			b = x0; fb = fx0;
		}

		else{
			a = x0; fa = fx0;
		}

		do {
			if (n > 0) {
				x0 = x1;
			}

			x1 = (a + b) / 2.0;

			for (j = 0; j < sampleNum - 1; j++) {
				if (x1 >= f[j][0] && x1 <= f[j + 1][0])
					break;
			}

			fx1 = cdf[j][1] + integralmini(pdf, j, x1) - ui;

			if (fa * fx1 < 0) {
				b = x1; fb = fx1;

			}
			else{
				a = x1; fa = fx1;
			}

			n++;
			
			// set bisection count with n
		} while (n < 5 && fabs(fx1) >= DELTA && fabs(x1 - x0) >= EPSILON);

		// newton
		x0 = x1; fx0 = fx1;
		n = 0;

		if (x0 < 0) x0 = 0;

		for (j = 0; j < sampleNum - 1; j++) {
			if (x0 >= f[j][0] && x0 <= f[j + 1][0])
				break;
		}

		if (j == sampleNum - 1) {
			j--; x0 = 1.0;
		}

		double s, fpx0;

		s = (x0 - f[j][0]) / (f[j + 1][0] - f[j][0]);

		do{
			if (n > 0) {
				x0 = x1; fx0 = fx1;
			}

			fpx0 = (1 - s) * pdf[j][1] + s * pdf[j + 1][1];

			x1 = x0 - (fx0 / fpx0);
			if (x1 < 0) x1 = 0;

			for (j = 0; j < sampleNum - 1; j++) {
				if (x1 >= f[j][0] && x1 <= f[j + 1][0])
					break;
			}

			if (j == sampleNum - 1) {
				j--; x1 = 1.0;
			}

			s = (x1 - f[j][0]) / (f[j + 1][0] - f[j][0]);

			fx1 = cdf[j][1] + integralmini(pdf, j, x1) - ui;

			n++;

		} while (n < Nmax && fabs(fx1) >= DELTA && fabs(x1 - x0) >= EPSILON);
	}

	CHECK_TIME_END(resultTime);
	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);
}

void program2_4()
{
	int randNum, i;
	double ui, lambda;
	double E = 0, V = 0;
	
	srand((unsigned int)time(NULL));

	printf("Type number of rand: ");
	if(!scanf("%d", &randNum)) exit(-1);

	printf("Type lambda: ");
	if(!scanf("%lf", &lambda)) exit(-1);

	double* rands = (double *)malloc(sizeof(double) * randNum);

	for (i = 0; i < randNum; i++) {
		ui = rand() / (double)(MY_RAND_MAX + 1);
		while (ui <= 0 && ui >= 1) // ui in (0, 1)
			ui = rand() / (double)(MY_RAND_MAX + 1);

		rands[i] = (-1) * log(1 - ui) / lambda;
		//printf("%.15lf\n", rands[i]);
	}

	for (i = 0; i < randNum; i++)
		E += rands[i];
	E /= randNum;

	for (i = 0; i < randNum; i++)
		V += pow((rands[i] - E), 2);
	V /= randNum;

	printf("# Expected data: ");
	printf("E(X): %lf, V(X): %lf\n", 1 / lambda, 1 / pow(lambda, 2));
	printf("# Real Data: ");
	printf("E(X): %lf, V(X): %lf\n", E, V);
}
