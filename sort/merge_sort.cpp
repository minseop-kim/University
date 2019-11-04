//���� �պ� ����
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

#define MAX_SIZE 100000
int arr[MAX_SIZE];
int result[MAX_SIZE];
int n;
void merge_sort(int l,int r);
void merge(int left,int mid, int r);

// �Լ��� ������ �ð��� �����ϱ����� ������ ��ó��
#define CHECK_TIME_START __int64 freq, start, end; if (QueryPerformanceFrequency((_LARGE_INTEGER*)&freq)) {QueryPerformanceCounter((_LARGE_INTEGER*)&start);  
#define CHECK_TIME_END(a,b) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a=(float)((double)(end - start)/freq*1000); b=TRUE; } else b=FALSE; // a�� float type milli second�̰� b�� FALSE�϶��� �����Դϴ�
bool err;

void main(){
	int i;
	double runtime = 0.0;
	printf("Enter the number of numbers to gnerate : ");
	scanf("%d",&n);

	for(i=0;i<n;i++)
		arr[i]=rand();

	CHECK_TIME_START;
	merge_sort(0,n);            //���� ȣ��
	CHECK_TIME_END(runtime, err);

	printf("�������� %6d�� : %fms\n", n, runtime);
}

void merge_sort(int first, int last){
   int mid;
   if(first<last){
      mid=(first+last)/2;
      merge_sort(first,mid);
      merge_sort(mid+1,last);
      merge(first,mid,last);
   }
}

void merge(int first, int mid, int last){
   int i=first, j=mid+1, k=first ;
   while (i<=mid && j<=last)
   {
      if (arr[i]<=arr[j])
         result[k++]=arr[i++];
      else
         result[k++]=arr[j++];
   }

   while(i<=mid)
	   result[k++] = arr[i++];
   while(j<=last)
      result[k++]=arr[j++];

   for (i=first; i<k; i++)
      arr[i]=result[i];
}