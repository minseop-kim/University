#include <stdio.h>
#include <memory.h>
#include "probing.h"

void main()
{
	int arr[30]={0,}, input[5]={1,31,61,91,121}, i;

	for(i=0;i<5;i++)
	{
		input_linear(arr, input[i]);
	}
	printf("선형 조사 결과\n");
	dispaly(arr);
	printf("\n\n이차원 조사 결과\n");

	memset(arr, 0, sizeof(int)*30);
	for(i=0;i<5;i++)
	{
		input_quadratic(arr, input[i]);
	}
	dispaly(arr);
}