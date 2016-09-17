#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<math.h>

typedef struct node {
	double dist;
	int * path;
	int processed;
}Node;

void ptnarr(Node * nodearr, int n);
void ptfarr(double * * arr, int i, int j);
void ptiarr(int * * arr, int i, int j);
void destroyStringArray(char * * strArr, int len);
char * * explode(const char * str, const char * delims, int len);

int cmpfunc (const void * a, const void * b);

double * * cgraph(FILE * fp, int n, double delta);
Node * cnodearr(double * * graph,int n,int src); //node arr for shortest path
void query(Node * nodearr,double * * graph,int n,int src, int d, double alpha);
int deg1(double * * graph,int n,int src);
int deg2(double * * graph,int n,int src);

int main(int argc, char * * argv)
{
	double * * graph = NULL;
	Node * nodearr = NULL;
	int n;//number of users
	int src; //source node
	double delta1; //threshold 1
	double delta2; //threshold 2
	double alpha;

	if (argc == 2) {
		FILE * fp = NULL;
		char * str = (char *) malloc(200 * sizeof(char));
		char * * strarr = NULL;

		fp = fopen(argv[1],"r");
		if (!fp) {
			fprintf(stderr,"Failed to open file '%s'\n", argv[1]);
			return EXIT_FAILURE;
		}
		else {
			if (!feof(fp)) { 
				fgets(str,200,fp);
				strarr = explode(str,",",5);
				n = atoi(strarr[0]);
				delta1 = atof(strarr[1]);
				delta2 = atof(strarr[2]);
				src = atof(strarr[3]);
				alpha = atof(strarr[4]);
				//printf("n = %d,delta1 = %f,delta2 = %f,src = %d,alpha = %f\n",n,delta1,delta2,src,alpha);
			}
			int offset = ftell(fp); 
			//delta1
			graph = cgraph(fp, n, delta1);
			nodearr = cnodearr(graph,n,src);
			query(nodearr,graph,n,src,1,alpha);

			printf("\n");
			//delta2
			fseek(fp, offset, SEEK_SET);
			graph = cgraph(fp, n, delta2);
			nodearr = cnodearr(graph,n,src);
			query(nodearr,graph,n,src,2,alpha);

			fclose(fp);
		}
	}
	else {
		printf("error!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

double * * cgraph(FILE * fp, int n, double delta)
{
	char * str = (char *) malloc(200 * sizeof(char));
	char * * strarr = NULL;
	int * * farr = malloc(n * sizeof(int *)); //file array
	int * * fcarr = farr;
	int i,j,k;
	int * fline;
	double * * graph = malloc((n+1) * sizeof(double *));

	//form initial matrix
	while (!feof(fp)) {
		fgets(str,200,fp);
		strarr = explode(str,",",9);
		fline = malloc(9 * sizeof(int));
		for (i = 0;i < 9;i++) fline[i] = atoi(strarr[i]);
		*fcarr = fline;
		fcarr++;
		destroyStringArray(strarr, 9);
	}//ptiarr(farr,n,9);

	//form graph
	for (i = 0;i <= n;i++) graph[i] = malloc((n+1) * sizeof(double)); 
	int ultot; //unnormalized length total
	double ulmax = 0; //max length
	for (i = 0;i <= n;i++) {
		for (j = 0;j <= n;j++) {
			if (i == j || i == 0 || j == 0) graph[i][j] = 0;
			else {
				ultot = 0;
				for	(k = 1;k < 9;k++) ultot += pow((farr[i-1][k])-(farr[j-1][k]),2);
				graph[i][j] = sqrt((double)ultot);
				if (graph[i][j] > ulmax) ulmax = graph[i][j];
			}
		}
	}//ptfarr(graph,n+1,n+1);

	//normalize..and disconnect(set 0) dont forget!
	double l; //normalized length
	for (i = 1;i <= n;i++) {
		for (j = 1;j <= n;j++) {
			if (i != j) {
				l = 1 - (graph[i][j])/ulmax;
				graph[i][j] = (double)((int)(l*100))/100 > delta? (double)((int)(l*100))/100:0;
			}
		}
	}//ptfarr(graph,n+1,n+1);

	return graph;
}

Node * cnodearr(double * * graph,int n,int src)
{
	int i,j,k;
	Node * nodearr = malloc((n+1)*sizeof(Node));
	nodearr[0].processed = 1;
	for (i = 1;i <= n;i++) {
		nodearr[i].dist = INT_MAX;
		nodearr[i].path = calloc(n,sizeof(int));
		nodearr[i].processed = 0;
	}
	nodearr[src].dist = 0;
	//start the first path from src
	for (i = 1; i <= n; i++) {
		if ((int)(graph[src][i]*100) > 0) {
			nodearr[i].dist = graph[src][i];
			(nodearr[i].path)[0] = src;
			(nodearr[i].path)[1] = i;	
		}
	}
	nodearr[src].processed = 1;
	//ptnarr(nodearr, n);

	//paths after first step
	int allprocessed = 0;
	while (!allprocessed) {
		allprocessed = 1;
		for (i = 1; i <= n; i++) {//go through all users,if path from source,process 
			if ((nodearr[i].path)[0] == src && nodearr[i].processed == 0) {
				for (j = 1; j <= n; j++) {//go through all users
					if((int)(graph[i][j]*100) != 0) {//find all adjcent j
						if ((int)(nodearr[j].dist*100) > (int)((graph[i][j] + nodearr[i].dist)*100)) {
							allprocessed = 0;
							//update the node dist and path
							nodearr[j].dist = graph[i][j] + nodearr[i].dist;
							free(nodearr[j].path);
							nodearr[j].path = calloc(n,sizeof(int));
							k = 0;
							while (nodearr[i].path[k] != 0) {
								nodearr[j].path[k] = nodearr[i].path[k];
								k++;
							}//last one left
							nodearr[j].path[k] = j;

							//if updated, set processed to 0
							nodearr[j].processed = 0;
						}
					}
				}
				nodearr[i].processed = 1;
			}
		}
	}//ptnarr(nodearr, n);

	return nodearr;
}

void query(Node * nodearr,double * * graph,int n,int src, int d, double alpha)
{
	int i;
	double ssdist = INT_MAX; //shortest amony shortest length
	int * ssarr = malloc((n+1)*sizeof(int));
	int ssarrnum = 0;
	//query1
	//printf("delta%d:\n",d);
	//printf("Q1:");
	for (i = 1;i <= n;i++) {
		if (ssdist > nodearr[i].dist && i != src) ssdist = nodearr[i].dist;
	}
	printf("%.2f",ssdist);
	for (i = 1;i <= n;i++) {
		if ((int)(ssdist*100) == (int)((nodearr[i].dist)*100)) {
			ssarr[ssarrnum++] = i;
		}
	}
	qsort(ssarr, ssarrnum, sizeof(int), cmpfunc);
	for (i = 0;i < ssarrnum;i++) printf(",%d",ssarr[i]);

	free(ssarr);
	printf("\n");

	//query2
	//printf("Q2:");
	int count = 0;
	for (i = 1;i <= n;i++) {
		if (alpha > nodearr[i].dist && i != src) count++;
	}
	printf("%d\n",count);

	//query3
	int firstf = 0;
	int * fstl = calloc(n+1,sizeof(int));
	//printf("Q3:");
	for (i = 1;i <= n;i++) {
		if ((int)(graph[src][i]*100)) {//if i node is adjcent
			fstl[firstf] = i;
			firstf++;
		}
	}
	qsort(fstl, firstf, sizeof(int), cmpfunc);
	printf("%d",firstf);
	for (i = 0;i < firstf;i++) printf(",%d",fstl[i]);
	printf("\n");

	//query4
	int secondf = 0;
	int * secl = calloc(n+1,sizeof(int));
	int j,k;
	int included;
	for (i = 0;i < firstf;i++) {
		for (j = 1;j <= n;j++) {
			if (((int)(graph[fstl[i]][j]*100)) && j!=src) {
				included = 0;
				for (k = 0;k < secondf;k++)	{//if in the seclist
					if (j == secl[k]) {
						included = 1;
					}
				}
				if (!included) {
					secl[secondf] = j;
					secondf++;
				}
			}
		}
	}
	qsort(secl, secondf, sizeof(int), cmpfunc);
	printf("%d",secondf);
	for (i = 0;i < secondf;i++) printf(",%d",secl[i]);
	printf("\n");
	free(fstl);
	free(secl);

	//query5
	//printf("Q5:");
	int totdeg1 = 0;
	for (i = 1;i <= n;i++) {totdeg1 += deg1(graph,n,i);}
	printf("%.2f\n",floor((double)totdeg1/n*100)/100);
	//printf("totdeg1 = %d\n",totdeg1);

	//query6
	//printf("Q6:");
	int totdeg2 = 0;
	for (i = 1;i <= n;i++) totdeg2 += deg2(graph,n,i);
	printf("%.2f\n",floor((double)totdeg2/n*100)/100);
}

int deg1(double * * graph,int n,int src)
{
	int i;
	int firstf = 0;
	int * fstl = calloc(n+1,sizeof(int));
	for (i = 1;i <= n;i++) {
		if ((int)(graph[src][i]*100)>0) {//if i node is adjcent
			fstl[firstf] = i;
			firstf++;
		}
	}
	free(fstl);
	return firstf;
}

int deg2(double * * graph,int n,int src)
{
	int i;
	int firstf = 0;
	int * fstl = calloc(n+1,sizeof(int));
	for (i = 1;i <= n;i++) {
		if ((int)(graph[src][i]*100)>0) {//if i node is adjcent
			fstl[firstf] = i;
			firstf++;
		}
	}
	int secondf = 0;
	int * secl = calloc(n,sizeof(int));
	int j,k;
	int included;
	for (i = 0;i < firstf;i++) {
		for (j = 1;j <= n;j++) {
			if ((int)(graph[fstl[i]][j]*100)>0 && j!=src) {
				included = 0;
				for (k = 0;k < secondf;k++) {//if in the seclist
					if (j == secl[k]) {
						included = 1;
					}
				}
				if (!included) {
					secl[secondf] = j;
					secondf++;
				}
			}
		}
	}
	free(fstl);
	free(secl);
	return secondf;
}


void destroyStringArray(char * * strArr, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		free(strArr[i]);
	}
	free(strArr);
}

char * * explode(const char * str, const char * delims, int len)
{
	int N = len;
	int i;
	int arrind = 0;
	int last = 0;

	char * * strarr = (char * *) malloc(sizeof(char *) * (N + 1));
	for (i = 0; i < (int) strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
			memcpy(strarr[arrind], str + last, i - last);
			strarr[arrind][i-last] = '\0';
			last = i + 1;
			arrind++;
		}
	}

	strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
	memcpy(strarr[arrind], str + last, i - last);
	strarr[arrind][i-last] = '\0';

	return(strarr);

}

void ptfarr(double * * arr, int i, int j)
{
	int k,m;
	for (k = 0;k < i;k++) {
		for (m = 0;m < j;m++) {
			printf("%-10.2f",arr[k][m]);
		}
		printf("\n");
	}
}

void ptiarr(int * * arr, int i, int j)
{
	int k,m;
	for (k = 0;k < i;k++) {
		for (m = 0;m < j;m++) {
			printf("%d ",arr[k][m]);
		}
		printf("\n");
	}
}

void ptnarr(Node * nodearr, int n)
{
	int i,j;
	for (i = 0;i <= n; i++) {
		printf("id = %-2d ",i);
		printf("dist = %.2f ",nodearr[i].dist);
		j = 0;
		printf("path = ");
		if (nodearr[i].path != NULL){
			while ((nodearr[i].path)[j] != 0) {
				printf("%d",(nodearr[i].path)[j]);
				j++;	
			}
		}
		printf(" processed = %d\n",nodearr[i].processed);
	}
}

int cmpfunc (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}
