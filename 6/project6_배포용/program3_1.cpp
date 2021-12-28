#include "my_solver.h"
#include <string.h>

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double c, b;
double p11, p12, p13, t1, tr1;
double p21, p22, p23, t2, tr2;
double p31, p32, p33, t3, tr3;
double p41, p42, p43, t4, tr4;

void fcn3_1_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
    // origin function F(x)
    if (*iflag == 1) {
        /********************************/
        fvec[0] = pow(x[0] - p11, 2) + pow(x[1] - p12, 2) + pow(x[2] - p13, 2) - pow(c * (tr1 + x[3] - t1), 2);
        fvec[1] = pow(x[0] - p21, 2) + pow(x[1] - p22, 2) + pow(x[2] - p23, 2) - pow(c * (tr2 + x[3] - t2), 2);
        fvec[2] = pow(x[0] - p31, 2) + pow(x[1] - p32, 2) + pow(x[2] - p33, 2) - pow(c * (tr3 + x[3] - t3), 2);
        fvec[3] = pow(x[0] - p41, 2) + pow(x[1] - p42, 2) + pow(x[2] - p43, 2) - pow(c * (tr4 + x[3] - t4), 2);
        /********************************/
    }
    // Jacobi matrix J(x)
    else if (*iflag == 2) {
        /********************************/
        fjac[0] = 2 * (x[0] - p11);	fjac[4] = 2 * (x[1] - p12);	fjac[8] = 2 * (x[2] - p13);	fjac[12] = -2 * c * c * (tr1 + x[3] - t1);
        fjac[1] = 2 * (x[0] - p21);	fjac[5] = 2 * (x[1] - p22);	fjac[9] = 2 * (x[2] - p23);	fjac[13] = -2 * c * c * (tr2 + x[3] - t2);
        fjac[2] = 2 * (x[0] - p31);	fjac[6] = 2 * (x[1] - p32);	fjac[10] = 2 * (x[2] - p33); fjac[14] = -2 * c * c * (tr3 + x[3] - t3);
        fjac[3] = 2 * (x[0] - p41);	fjac[7] = 2 * (x[1] - p42);	fjac[11] = 2 * (x[2] - p43); fjac[15] = -2 * c * c * (tr4 + x[3] - t4);
        /********************************/
    }
}

void fcn3_1_2(int* n, double* x, double* fvec, int* iflag)
{
	fvec[0] = pow(x[0] - p11, 2) + pow(x[1] - p12, 2) + pow(x[2] - p13, 2) - pow(c * (tr1 + x[3] - t1), 2);
	fvec[1] = pow(x[0] - p21, 2) + pow(x[1] - p22, 2) + pow(x[2] - p23, 2) - pow(c * (tr2 + x[3] - t2), 2);
	fvec[2] = pow(x[0] - p31, 2) + pow(x[1] - p32, 2) + pow(x[2] - p33, 2) - pow(c * (tr3 + x[3] - t3), 2);
	fvec[3] = pow(x[0] - p41, 2) + pow(x[1] - p42, 2) + pow(x[2] - p43, 2) - pow(c * (tr4 + x[3] - t4), 2);

}

void program3_1() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, 0.0, 0.0, 0.0 };
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	char readfile[256];
	char writefile[256];

	printf("program 3_1_1\n");
	for (int i = 0; i < 3; i++) {
		sprintf(readfile, "GPS_signal_%d.txt", i);
		sprintf(writefile, "GPS_position_3-1_%d.txt", i);

		FILE* fp_r = fopen(readfile, "r");
		FILE* fp_w = fopen(writefile, "w");
		if (!fp_r) exit(-1);

		fscanf(fp_r, "%lf %lf\n", &c, &b);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p11, &p12, &p13, &t1, &tr1);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p21, &p22, &p23, &t2, &tr2);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p31, &p32, &p33, &t3, &tr3);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p41, &p42, &p43, &t4, &tr4);

		printf("Type x10 x20 x30\n");
		scanf("%lf %lf %lf", &(x[0]), &(x[1]), &(x[2])); x[3] = b;
		fprintf(fp_w, "# [%d] x0 = (%lf, %lf, %lf, %lf)\n", i + 1, x[0], x[1], x[2], x[3]);

		memset(wa, 0, sizeof(wa)); memset(fvec, 0, sizeof(fvec)); memset(fjac, 0, sizeof(fjac));
		hybrj1_(fcn3_1_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

		if(info == 1) {
			for (int k = 0; k < SOLNUMS - 1; k++)
				fprintf(fp_w, "x%d = %lf, ", k + 1, x[k]);
			fprintf(fp_w, "x%d = %lf\n", SOLNUMS, x[SOLNUMS - 1]);

			fprintf(fp_w, "Accuration(f(x)): ");
			for (int k = 0; k < SOLNUMS; k++) 
				fprintf(fp_w, "%lf ", fabs(fvec[i]));
			fprintf(fp_w, "\n");
		}
		else{
			printf("Wrong\n");
			fprintf(fp_w, "Wrong\n\n");
		}

		fclose(fp_r);
		fclose(fp_w);
	}
	
	printf("\n\n");
	printf("program 3_1_2\n");
	n = SOLNUMS; ldfjac = SOLNUMS; tol = TOLERANCE;
	double wa_d[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa_d = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;
	memset(fvec, 0, sizeof(fvec)); memset(x, 0, sizeof(x));

	for (int i = 0; i < 3; i++) {
		sprintf(readfile, "GPS_signal_%d.txt", i);
		sprintf(writefile, "GPS_position_3-2_%d.txt", i);

		FILE* fp_r = fopen(readfile, "r");
		FILE* fp_w = fopen(writefile, "w");
		if (!fp_r) exit(-1);

		fscanf(fp_r, "%lf %lf\n", &c, &b);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p11, &p12, &p13, &t1, &tr1);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p21, &p22, &p23, &t2, &tr2);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p31, &p32, &p33, &t3, &tr3);
		fscanf(fp_r, "%lf %lf %lf %lf %lf\n", &p41, &p42, &p43, &t4, &tr4);

		printf("Type x10 x20 x30\n");
		scanf("%lf %lf %lf", &(x[0]), &(x[1]), &(x[2])); x[3] = b;
		fprintf(fp_w, "# [%d] x0 = (%lf, %lf, %lf, %lf)\n", i + 1, x[0], x[1], x[2], x[3]);

		//memset(wa, 0, sizeof(wa)); memset(fvec, 0, sizeof(fvec));
		hybrd1_(fcn3_1_2, &n, x, fvec, &tol, &info, wa_d, &lwa_d);

		if (info == 1) {
			for (int k = 0; k < SOLNUMS - 1; k++)
				fprintf(fp_w, "x%d = %lf, ", k + 1, x[k]);
			fprintf(fp_w, "x%d = %lf\n", SOLNUMS, x[SOLNUMS - 1]);

			fprintf(fp_w, "Accuration(f(x)): ");
			for (int k = 0; k < SOLNUMS; k++)
				fprintf(fp_w, "%lf ", fabs(fvec[k]));
			fprintf(fp_w, "\n");
		}
		else {
			printf("Wrong\n");
			fprintf(fp_w, "Wrong\n\n");
		}

		fclose(fp_r);
		fclose(fp_w);
	}
}
