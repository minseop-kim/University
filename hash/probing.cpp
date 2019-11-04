#include "probing.h"
#include <math.h>
#include <stdio.h>

#define TABLE_SIZE 30

void input_linear(int *arr, int key)
{
	int hash;

	hash = key%TABLE_SIZE;
	while(arr[hash] != 0)
		hash = ++hash % TABLE_SIZE;
	arr[hash] = key;
}

void input_quadratic(int *arr, int key)
{
	int hash, hash_count=0, calc;

	hash = key%TABLE_SIZE;
	calc = hash;
	while(arr[calc] != 0)
	{
		hash_count++;
		calc = (hash + (int)pow((double)hash_count, 2)) % TABLE_SIZE;
	}
	arr[calc] = key;
}

void dispaly(int *arr)
{
	int i;

	for(i=0; i<TABLE_SIZE; i++)
	{
		printf("배열의 %2d 번째 요소 = %3d\n", i, arr[i]);
	}
}