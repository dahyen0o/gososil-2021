#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 100000
float *hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];

/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
#define N_MAX 1000000



void main(void)
{
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(1);
	CHECK_TIME_START;
	hw1_calc_e();
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);


	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
	
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
	int i, j, r;
	puts("====== hw3 ======");
	puts("====== (1) ======");
	srand((unsigned)time(NULL));
	CHECK_TIME_START;
	for (i = 0; i < N_MAX; i++) {
		r = rand();

		if (r % 2);
	}
	CHECK_TIME_END(compute_time);
	printf("naive result: %f ms\n", compute_time);

	CHECK_TIME_START;
	for (i = 0; i < N_MAX; i++) {
		r = rand();

		if (r & 1);
	}
	CHECK_TIME_END(compute_time);
	printf("advanced result: %f ms\n", compute_time);

	puts("====== (2) ======");
	float arr[MATDIM];
	CHECK_TIME_START;
	for (i = 0; i < MATDIM; i++) {
		if (i % 2 == 0) arr[i] = i;
		else arr[i] = i + 1;
	}
	CHECK_TIME_END(compute_time);
	printf("naive result: %f ms\n", compute_time);

	CHECK_TIME_START;
	arr[0] = arr[1] = arr[2] = arr[3] = arr[4] = arr[5] = 2;
	for (i = 6; i < MATDIM; i++) {
		if (i % 2 == 0) arr[i] = i;
		else arr[i] = i + 1;
	}
	CHECK_TIME_END(compute_time);
	printf("advanced result: %f ms\n", compute_time);

	puts("====== (3) ======");
	float** arr1 = (float**)malloc(sizeof(float*) * MATDIM);
	for (i = 0; i < MATDIM; i++) {
		arr1[i] = (float*)malloc(sizeof(float) * MATDIM);
	}

	CHECK_TIME_START;
	for (j = 0; j < MATDIM; j++) {
		for (i = 0; i < MATDIM; i++) {
			arr1[i][j] = i + j;
		}
	}
	CHECK_TIME_END(compute_time);
	printf("naive result: %f ms\n", compute_time);
	CHECK_TIME_START;
	for (i = 0; i < MATDIM; i++) {
		for (j = 0; j < MATDIM; j++) {
			arr1[i][j] = i + j;
		}
	}
	CHECK_TIME_END(compute_time);
	printf("advanced result: %f ms\n", compute_time);

	puts("====== (4) ======");
	CHECK_TIME_START;
	for (j = 0; j < MATDIM; j++) {
		arr[j] = j;
	}
	CHECK_TIME_END(compute_time);
	printf("naive result: %f ms\n", compute_time);
	CHECK_TIME_START;
	for (j = 0; j < 100; j++) {
		arr[j] = j;
	}
	for (j = 100; j < 200; j++) {
		arr[j] = j;
	}
	for (j = 200; j < 500; j++) {
		arr[j] = j;
	}
	for (j = 500; j < MATDIM; j++) {
		arr[j] = j;
	}
	CHECK_TIME_END(compute_time);
	printf("advanced result: %f ms\n", compute_time);

	puts("====== (5) ======");
	CHECK_TIME_START;
	int flag = 1;
	for (j = 0; j < MATDIM; j++) {
		if (flag % 2 == 0) arr[j] = j;
		else arr[j] = j + 1;
	}
	CHECK_TIME_END(compute_time);
	printf("naive result: %f ms\n", compute_time);
	CHECK_TIME_START;
	if (flag % 2 == 0) {
		for (j = 0; j < MATDIM; j++)
			arr[j] = j;
	}
	else {
		for (j = 0; j < MATDIM; j++)
			arr[j] = j + 1;
	}
	CHECK_TIME_END(compute_time);
	printf("advanced result: %f ms\n", compute_time);


}

void init_hw1(int fixed)
{
	float *ptr;
	hw1_x = (float *)malloc(sizeof(float)*HW1_N);

	if (fixed)
	{
		float tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if( i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float)RAND_MAX) * 2;
	}
}
void hw1_calc_e()
{
	hw1_E = 0;

	for (int i = 0; i < HW1_N; i++) {
		hw1_E += hw1_x[i];
	}

	hw1_E /= HW1_N;
}
void hw1_calc_var1()
{
	float sum_x = 0;
	float square_x = 0;

	for (int i = 0; i < HW1_N; i++) {
		sum_x += hw1_x[i];
		square_x += pow(hw1_x[i], 2);
	}

	hw1_var1 = (HW1_N * square_x - pow(sum_x, 2)) / (HW1_N * (HW1_N - 1));
}
void hw1_calc_var2()
{
	hw1_var2 = 0;
	for (int i = 0; i < HW1_N; i++) {
		hw1_var2 += pow(hw1_x[i] - hw1_E, 2);
	}

	hw1_var2 /= HW1_N - 1;
}


/* hw2 */
void hw2_naive()
{
	hw2_naive_ans[0] = (-hw2_b + sqrt(pow(hw2_b, 2) - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_naive_ans[1] = (-hw2_b - sqrt(pow(hw2_b, 2) - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
}
void hw2_safe()
{
	hw2_pre_ans[1] = (-hw2_b - sqrt(pow(hw2_b, 2) - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	if (hw2_b > 0 && abs(hw2_a * hw2_c) / pow(hw2_b, 2) < 0.0001) {
		(2 * hw2_a);
		hw2_pre_ans[0] = (-4 * hw2_a * hw2_c / (hw2_b + sqrt(pow(hw2_b, 2) - 4 * hw2_a * hw2_c))) / (2 * hw2_a);
	}
	else {
		hw2_pre_ans[0] = (-hw2_b + sqrt(pow(hw2_b, 2) - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	}
}
float hw2_verify(float x)
{
	return hw2_a * x * x + hw2_b*x + hw2_c;
}
