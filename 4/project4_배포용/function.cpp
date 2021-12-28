#define _USE_MATH_DEFINES
#include <math.h>

double _f1(double x) {
	// x^2 -4x +4 -lnx = 0
	return pow(x, 2) - (4 * x) + 4.0f - log(x);
}

double _fp1(double x) {
	// 2x -4 -(1/x)
	return (2 * x) - 4.0f - (1 / x);
}

double _f2(double x) {
	//f2 = x + 1 - 2sin(PI * x) = 0
	return x + 1 - 2.0f * (double)sinf(M_PI * x);
}

double _fp2(double x) {
	// 1 - 2*pi*cos(pi * x)
	return 1 - 2 * M_PI * (double)cosf(M_PI * x);
}

double _f3(double x) {
	// f3 = x^4 -11.0x^3 +42.35x^2 -66.55x +35.1384 = 0
	return pow(x, 4) - 11.0f * pow(x, 3) + 42.35f * pow(x, 2) - 66.55f * x + 35.1384f;
}

double _fp3(double x) {
	// 4x^3 -33.0x^2 + 84.70x - 66.55
	return 4.0f * pow(x, 3) - 33.0f * pow(x, 2) + 84.70f * x - 66.55f;
}

double _f_sqrt(double x) {
	return x * x - 2.0;
}

double _fp_sqrt(double x) {
	return 2.0 * x;
}

double _f_vehicle(double x) {
	x = x * M_PI / 180;
	double l = 89.0, h = 49.0, D = 55.0, beta1 = 11.5 *M_PI / 180;
	double A, B, C, E;
	A = l * sinf(beta1);
	B = l * cosf(beta1);
	C = (h + 0.5 * D) * sinf(beta1) - 0.5 * D * tanf(beta1);
	E = (h + 0.5 * D) * cosf(beta1) - 0.5 * D;
	return A * sinf(x) * cosf(x) + B * sinf(x) * sinf(x) - C * cosf(x) - E * sinf(x);
}

double _fp_vehicle(double x) {
	x = x * M_PI / 180;
	double l = 89.0, h = 49.0, D = 55.0, beta1 = 11.5 *M_PI / 180;
	double A, B, C, E;
	A = l * sinf(beta1);
	B = l * cosf(beta1);
	C = (h + 0.5 * D) * sinf(beta1) - 0.5 * D * tanf(beta1);
	E = (h + 0.5 * D) * cosf(beta1) - 0.5 * D;
	return A * (pow(cosf(x), 2.0) - pow(sinf(x), 2.0)) + 2.0 * B * sinf(x) * cosf(x) + C * sinf(x) - E * cosf(x);
}

double _f_comp(double x) {
	// lnx - 1
	return log(x) - 1;
}

double _fp_comp(double x) {
	return 1 / x;
}
