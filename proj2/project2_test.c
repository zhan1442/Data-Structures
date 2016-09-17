#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void printArray(int * array, int size);
void sort(int list[], int n);
void quicksort(int * list, int d, int u,int min, int max);

void quicksort(int * list, int d, int u,int min, int max){
	int i=d;
	int j=u;
	int mid=((long int)min+max)/2;
	do {
		while ((list[i]<mid)&&(i<=u)) i++;
		while ((list[j]>mid)&&(j>=d)) j--;
		if (i<=j){int temp=list[i]; list[i]=list[j];list[j]=temp;
			i++;j--;}
	} while (i<=j);
	if (i<u) quicksort(list,i,u,mid,max);
	if (j>d) quicksort(list,d,j,min,mid);
}                            

void printArray(int * array, int size)
{
	int i;
	printf("[ ");
	for (i = 0; i < size; i++)
		printf("%d ", array[i]);
	printf("]\n");
}

int main(int argc,char * * argv)
{
	//printf("----------------------------------\n");
	int size = 0;
	//size = atoi(argv[1]);
	int i;
	//for (i = 0;i < size;i++) if(i<size-10) list[i] = (int) (rand()%10-5)*(rand()%((i+1)*10))/2; else list[i]=(int) (rand()%10-5)*rand()/5;
	int j = 1000;
	int k;
	//for(j = 10;j<=100000000;j*=10){printf("\n==============times = %d===============\n",j);
		int * list = malloc(j*sizeof(int));
		size = j;
		//for (k = 100000;k<990000000;k*=2){ printf("\n------max = %d------\n",k);
		//for (i = 0;i < size;i++) list[i]= rand();
		for (i = 0;i < j;i++) if(i<size-10) list[i] = (int) (rand()%10-5)*(rand()%((i+1)*10))/2; else list[i]=(int) (rand()%10-5)*rand()/5;
		//for (i = 0;i < size;i++) list[i]= srand((unsigned)(time(NULL)));
		printf("\nUnsorted List:");
		printArray(&list[0], size);

		clock_t st;
		clock_t ed;
		int max = list[0];
		int min = list[0];
		st = clock();
		//sort(list, size);
		for (i = 1;i<1000;i++) {if (list[i]>max) max = list[i]; else if (list[i]<min) min = list[i];}
		quicksort(list,0, j,min,max);
		ed = clock();

		printf("Sorted List:");
		printArray(&list[0], size);
		printf("time = %f\n",(double) (ed-st)/CLOCKS_PER_SEC);
		free(list);//}

		return 0;
	}
