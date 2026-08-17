#include <stdio.h>

int calculate_sum(const int num[], int cnt){
	int sum = 0;
	for (int i=0; i< cnt; i++) {
		sum += num[i];
	}
	return sum;
}

float calculate_avg(const int num[], int cnt){
	return calculate_sum(num, cnt) / (float) cnt;
}

void print_report(const int num[], int cnt){
	float avg = calculate_avg(num, cnt);
	printf("average: %f\n", avg);
}

int main(void) {
	const int num[5] = {10, 20, 30, 40, 50};
	print_report(num, 5);
	return 0;
}
