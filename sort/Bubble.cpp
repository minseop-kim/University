#include "Bubble.h"
#include <stdio.h>
#include <Windows.h>


// 함수를 실행한 시간을 측정하기위해 선언한 전처리
#define CHECK_TIME_START __int64 freq, start, end; if (QueryPerformanceFrequency((_LARGE_INTEGER*)&freq)) {QueryPerformanceCounter((_LARGE_INTEGER*)&start);  
#define CHECK_TIME_END(a,b) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a=(float)((double)(end - start)/freq*1000); b=TRUE; } else b=FALSE; // a는 float type milli second이고 b가 FALSE일때는 에러입니다
bool err;

int bubble_sort(int *raw, int size, double *runtime)
{
	bool shifted = false;
	int index, index_cmp, temp;


	CHECK_TIME_START;
	if(size == 0)
		return 0;
	if(size == 1)
		return 1;
	for( index = 0; index < size; index++)
	{
		shifted = false;
		for( index_cmp = 0; index_cmp < size-index-1; index_cmp++)
		{
			if(raw[index_cmp] > raw[index_cmp+1])
			{
				shifted = true;
				temp = raw[index_cmp+1];
				raw[index_cmp+1] = raw[index_cmp];
				raw[index_cmp] = temp;
			}
		}
		if(!shifted)
			break;
	}
	CHECK_TIME_END(*runtime, err);
	return 1;
}
int arry_printf(int *arr, int size)
{
	int index;

	if(size <= 0)
		return 0;

	for(index =0; index < size; index++)
		printf("%5d", arr[index]);

	printf("\n");

	return 1;
}