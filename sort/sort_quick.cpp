#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void quickSort(int numbers[], int array_size);
void q_sort(int numbers[], int left, int right);

#define MAX_SIZE 100000
double runtime = 0.0;
int n;

// �Լ��� ������ �ð��� �����ϱ����� ������ ��ó��
#define CHECK_TIME_START __int64 freq, start, end; if (QueryPerformanceFrequency((_LARGE_INTEGER*)&freq)) {QueryPerformanceCounter((_LARGE_INTEGER*)&start);  
#define CHECK_TIME_END(a,b) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a=(float)((double)(end - start)/freq*1000); b=TRUE; } else b=FALSE; // a�� float type milli second�̰� b�� FALSE�϶��� �����Դϴ�
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
	printf("  ������ %6d�� : %fms\n", n, runtime);
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
	pivot = numbers[left]; // 0��° ���Ҹ� �Ǻ����� ����
	while (left < right)
	{
		// ���� ������ �Ǻ��� ���ų� ũ�ٸ�, �̵��� �ʿ䰡 ����
		while ((numbers[right] >= pivot) && (left < right))
			right --;

		// �׷��� �ʰ� ���� �Ǻ����� �۴ٸ�,
		// �Ǻ��� ��ġ�� ���� ���� �ִ´�.
		if (left != right)
		{
			numbers[left] = numbers[right];
		}
		// ���ʺ��� ���� ��ġ���� ���� �о���̸鼭
		// �Ǻ����� ū ���� �ִٸ�, ���� �̵��Ѵ�.
		while ((numbers[left] <= pivot) && (left < right))
			left ++;
		if (left != right)
		{
			numbers[right] = numbers[left];
			right --;
		}
	}
	// ��� ��ĵ�� �����ٸ�, �Ǻ����� ���� ��ġ�� �Է��Ѵ�.
	// ���� �Ǻ��� �������� ���ʿ��� �Ǻ����� �۰ų� ���� ���� ���Ҵ�.
	numbers[left] = pivot;
	pivot = left;
	left = l_hold;
	right = r_hold;

	// ���ȣ���� �����Ѵ�.
	if (left < pivot)
		q_sort(numbers, left, pivot - 1);
	if (right > pivot)
		q_sort(numbers, pivot+1, right);
}