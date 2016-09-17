#include<stdio.h>
#include<stdlib.h>
#include <time.h>
int main(int argc,char * * argv)
{
	int i;
	int st,ed;
	int g=0;
	st = clock();
	printf("number = %d\n",atoi(argv[1]));
	for (i = 0;i < atoi(argv[1]);i++) g++;
	ed = clock();
	printf("time = %f\n\n",(double) (ed-st)/CLOCKS_PER_SEC);
	return(0);
}
