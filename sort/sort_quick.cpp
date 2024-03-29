#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void quickSort(int numbers[], int array_size);
void q_sort(int numbers[], int left, int right);

#define MAX_SIZE 100000
double runtime = 0.0;
int n;

// 함수를 실행한 시간을 측정하기위해 선언한 전처리
#define CHECK_TIME_START __int64 freq, start, end; if (QueryPerformanceFrequency((_LARGE_INTEGER*)&freq)) {QueryPerformanceCounter((_LARGE_INTEGER*)&start);  
#define CHECK_TIME_END(a,b) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a=(float)((double)(end - start)/freq*1000); b=TRUE; } else b=FALSE; // a는 float type milli second이고 b가 FALSE일때는 에러입니다
bool err;

int main(int argc, char **argv)
{
	int *arr = (int*)calloc(MAX_SIZE, sizeof(int));
	int i;
	printf("Enter the number of numbers to gnerate : ");
	scanf("%d",&n);

	for(i=0;i<n;i++)
		arr[i]=rand();

	CHECK_TIME_START;
	quickSort(arr, n);
	CHECK_TIME_END(runtime, err);
	printf("  퀵정렬 %6d개 : %fms\n", n, runtime);
}


void quickSort(int numbers[], int array_size)
{
	q_sort(numbers, 0, array_size -1);
}

void q_sort(int numbers[], int left, int right)
{
	int pivot, l_hold, r_hold;
	l_hold = left;
	r_hold = right;
	pivot = numbers[left]; // 0번째 원소를 피봇으로 선택
	while (left < right)
	{
		// 값이 선택한 피봇과 같거나 크다면, 이동할 필요가 없다
		while ((numbers[right] >= pivot) && (left < right))
			right --;

		// 그렇지 않고 값이 피봇보다 작다면,
		// 피봇의 위치에 현재 값을 넣는다.
		if (left != right)
		{
			numbers[left] = numbers[right];
		}
		// 왼쪽부터 현재 위치까지 값을 읽어들이면서
		// 피봇보다 큰 값이 있다면, 값을 이동한다.
		while ((numbers[left] <= pivot) && (left < right))
			left ++;
		if (left != right)
		{
			numbers[right] = numbers[left];
			right --;
		}
	}
	// 모든 스캔이 끝났다면, 피봇값을 현재 위치에 입력한다.
	// 이제 피봇을 기준으로 왼쪽에는 피봇보다 작거나 같은 값만 남았다.
	numbers[left] = pivot;
	pivot = left;
	left = l_hold;
	right = r_hold;

	// 재귀호출을 수행한다.
	if (left < pivot)
		q_sort(numbers, left, pivot - 1);
	if (right > pivot)
		q_sort(numbers, pivot+1, right);
}