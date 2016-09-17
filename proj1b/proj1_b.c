// Group Member : 
// Zhizhe Zhang;   Tian Qiu


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct tasknode {
	int artime; //arrival time
	int priority;    //priority
	int subnum;    //subtasks number
	int inisubnum;  //initial subnum
	int * dur;  //duration time list
	int * detime;   //depature time 

	struct tasknode * next;
}Node;


double ucpu(Node * list, int t);
int readft(char * filename);
Node * createnode(int artime, int priority, int subnum, int* dur, int* detime);
Node * insert(Node * node, Node * list);
void list_print(Node * list);
void mode1(float lam0,float lam1,float  mu,int  task);
int qcount(Node * q0, Node * q1);
int simulation(Node * list, double avg);
char * * explode(const char * str, const char * delims, int * arrLen);
void destroyStringArray(char * * strArr, int len);

int main(int argc, char * * argv)
{

	if (argc == 2) {
		readft(argv[1]);
	}
	else if (argc == 5) {

		float lam0, lam1, mu;
		int task;
		lam0 = atof(argv[1]);
		lam1 = atof(argv[2]);
		mu = atof(argv[3]);
		task = atoi(argv[4]);
		mode1(lam0, lam1, mu, task);
	}
	else {
		printf("error!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
int simulation(Node * list,double avg){
	int t = 0;
	int s = 64; // freed processors
	int ind = 0;
	Node* q0 = NULL;
	Node* q1 = NULL;
	Node* q0cpy = NULL;
	Node* q1cpy = NULL;
	Node* node = NULL;
	Node* serving = NULL;   //serving list
	int q0wait = 0;
	int q1wait = 0;
	int q0num = 0;
	int q1num = 0;
	int slength = 0; //sum of length
	int peeks = 0;
	Node * listcpy = list;

	while (1){
		//collect qlength
		if(list != NULL && list->artime == t){
			slength += qcount(q0,q1);
			peeks ++;
		}

		while (list != NULL && list->artime == t){
			//	if(q0 != NULL || q1 != NULL) {
			node = createnode(list->artime, list->priority, list->subnum, list->dur, list->detime);
			list = list->next;
			if(node->priority == 0) {q0 = insert(node, q0); q0num ++;}
			else if(node->priority == 1) {q1 = insert(node, q1); q1num ++;}
			/*	} 
				else if(q0 == NULL && q1 == NULL){
				node = createnode(list->artime, list->priority, list->subnum, list->dur, list->detime);
				list = list->next;
				if(s >= node->subnum){
			// freed processors deducted
			s -= node->subnum;
			// set departure time array
			for(ind = 0; ind < node->subnum; ++ind){
			node->detime[ind] += node->dur[ind];
			}
			// set serving
			serving = insert(node, serving);
			}else{ 
			if(node->priority == 1) q1 = insert(node, q1);
			else q0 = insert(node, q0);
			//	}
			}*/
		}


		// depart serving list, change s 
		Node* servingcpy = serving;
		if(serving != NULL){
			for(ind = 0; ind < serving->inisubnum; ++ind){
				if((serving->detime)[ind] == t){
					s++;
				//`	if(serving->subnum > 0)
						serving->subnum--;
				}   
			} 
		}
		while(serving != NULL && serving->next != NULL ){
			if(serving->next != NULL){
				for(ind = 0; ind < serving->next->inisubnum; ++ind){
					if(serving->next->detime[ind] == t){
						s++;
						serving->next->subnum--;
					}   
				} 
			}
			if(serving->subnum == 0){
				serving = serving ->next;
				servingcpy = serving;
			}
			else if(serving->next != NULL && serving->next->subnum !=0){
				serving = serving -> next;
			}
			else if(serving->next != NULL && serving->next->subnum == 0){
				free(serving->next);
				serving->next = serving->next->next;
			}
		}
		serving = servingcpy;
		if(serving != NULL && serving->subnum == 0){
			serving = serving->next;
		}


		// put into serving list
		q0cpy = q0;
		q1cpy = q1;
		while (s > 0){
			if(q0 != NULL && s >= q0->subnum){
				node = createnode(q0->artime, q0->priority, q0->subnum, q0->dur, q0->detime);
				for(ind = 0; ind < node->subnum; ind++){
					node->detime[ind] = t + node->dur[ind];
				}
				//collect waiting
				q0wait += t - node->artime;

				serving = insert(node, serving);
				s -= q0->subnum;
				q0 = q0->next;
				q0cpy = q0;
			}
			else if(q0 != NULL && q0->next != NULL && s < q0->next->subnum){
				q0 = q0->next;
			}
			else if(q0 != NULL && q0->next != NULL && s >= q0->next->subnum){
				node = createnode(q0->next->artime,q0->next->priority, q0->next->subnum, q0->next->dur, q0->next->detime);
				for(ind = 0; ind < node->subnum; ind++){
					node->detime[ind] = t + node->dur[ind];
				}
				q0wait += t - node->artime;

				serving = insert(node, serving);
				s -= q0->next->subnum;
				q0->next = q0->next->next;
			}
			else if(q1 != NULL && s >= q1->subnum){
				node = createnode(q1->artime, q1->priority, q1->subnum, q1->dur, q1->detime);
				for(ind = 0; ind< node->subnum; ind++){
					node->detime[ind] = t + node->dur[ind];
				}
				//collect wait
				q1wait += t - node->artime;

				serving = insert(node, serving);
				s -= q1->subnum;
				q1 = q1->next;
				q1cpy = q1;
			}
			else if(q1 != NULL && q1->next != NULL && s < q1->next->subnum){
				q1 = q1->next;
			}
			else if(q1 != NULL && q1->next != NULL && s >= q1->next->subnum){
				node = createnode(q1->next->artime,q1->next->priority, q1->next->subnum, q1->next->dur, q1->next->detime);
				for(ind = 0; ind < node->subnum; ind++){
					node->detime[ind] = t + node->dur[ind];
				}  
				//collect wait
				q1wait += t - node->artime;

				serving = insert(node, serving);
				s -= q1->next->subnum;
				q1->next = q1->next->next;
			}
			else if (q1 == NULL || q1->next == NULL) break;

			//	serving = insert(node, serving);
		}
		q0 = q0cpy;
		q1 = q1cpy;
		//	list_print(q0);
		if(serving == NULL && list == NULL && q1 == NULL && q0 == NULL) break;
		t++;
	}
	//printf("q0wait = %d, q1wait = %d, q0num = %d, q1num = %d\n",q0wait,q1wait,q0num,q1num);
	//printf("slength = %d, peeks = %d\n",slength,peeks);

	double aucpu = 0.0;
	aucpu = ucpu(listcpy, t);

	FILE * fp = NULL;
	fp = fopen("proj1-a_output","w");
	double awt0 = (double) q0wait/q0num;
	double awt1 = (double) q1wait/q1num;
	double aqlength = (double) slength/peeks;
	fprintf(fp,"%f\n%f\n%f\n%f\n%f\n",awt0,awt1,aqlength,aucpu,avg);
	fclose(fp);

	printf("%f\n%f\n%f\n%f\n%f\n",awt0,awt1,aqlength,aucpu,avg);


	return t;
}


void mode1(float lam0,float lam1,float  mu,int  task)
{
	double avg = 0.0;
	double longest = 0;
	double shortest= 0;
	float x = 0;
	Node * list = NULL;
	Node * node = NULL;
	int artime = 0;
	int priority = 0;
	int subnum = 0;
	int* dur = malloc(sizeof(int) * 32);
	do{
		int* detime = malloc(sizeof(int) * 32);
		detime[0] = -1;
		x = (rand() % 10 / 10.0);
		subnum = rand() % 32 + 1;
		dur = malloc(sizeof(int) * subnum);
		priority = rand() % 2;
		if(priority == 0){
			artime += (-log(1 - x) / lam0) + 1;
		}else if(priority == 1){
			artime += (-log(1 - x) / lam1) + 1;
		}
		int ind = 0;
		for(ind = 0; ind < subnum; ++ind){
			x = (rand()%10 / 10.0);
			dur[ind] = (-log(1 - x) / mu) + 1;      // service time
			if(dur[ind] > longest){
				longest = dur[ind];
			}
			if(dur[ind] < shortest){
				shortest = dur[ind];
			}
		}
		detime[0] = -1;
		node = createnode(artime, priority, subnum, dur, detime);
		list = insert(node,list);

	}while(--task);
	list_print(list);
	avg = (longest - shortest) / (1/mu);
	simulation(list, avg);
	return;
}

int readft(char * filename)
{
	double longest = 0;
	double shortest= 0;
	double avg = 0.0;  //average CPU time
	double tot = 0.0;  // total CPU time
	FILE * fp = NULL;
	fp = fopen(filename,"r");
	if (!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", filename);
		return EXIT_FAILURE;
	}
	char * str = (char *) malloc(2000 * sizeof(char));//////
	char * * strarr = NULL;//////

	Node * list = NULL; //the complete list of tasks

	int expnum = 0; //explode number
	while (!feof(fp)) {
		fgets(str,2000,fp);
		if (!feof(fp)) {
			//read number for explode
			strarr = explode(str, " ", &expnum);
			int * dur = malloc(sizeof(int)*expnum);
			int i;
			for (i = 0; i < expnum-3; i++){
				dur[i] = atoi(strarr[3+i]) ;
				tot += dur[i];
				if(dur[i] > longest){
					longest = dur[i];
				}
				if(dur[i] < shortest){
					shortest = dur[i];
				}
			}
			int* detime = malloc(sizeof(int) * 32);
		//	detime[0] = -1;
			Node * node = createnode(atoi(strarr[0]),atoi(strarr[1]),atoi(strarr[2]), dur, detime); //////
			list = insert(node, list);

			destroyStringArray(strarr, expnum);
		}
	}
	avg = (longest - shortest) / (tot / 64);
	simulation(list, avg);


	free(str);
	fclose(fp);
	return EXIT_SUCCESS;
}

Node * createnode(int artime, int priority, int subnum, int* dur, int* detime)
{
	int ind = 0;
	Node * node = malloc(sizeof(Node));
	node->artime = artime;
	node->priority = priority;
	node->subnum = subnum;
	node->inisubnum = subnum;
	node->dur = malloc(sizeof(int) * 32);
	node->detime = malloc(sizeof(int) * 32);
	for(ind = 0; ind < node->inisubnum; ++ind){ 
		node->dur[ind] = dur[ind];
		node->detime[ind] = detime[ind]; 
	}
	node->next = NULL;
	return node;
}

char * * explode(const char * str, const char * delims, int * arrLen)
{
	int N = 0;
	int i;
	int arrind = 0;
	int last = 0;

	for (i = 0; i < strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			N++;
		}
	}		
	char * * strarr = (char * *) malloc(sizeof(char *) * (N + 1));
	* arrLen = N + 1;

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

void destroyStringArray(char * * strArr, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		free(strArr[i]);
	}
	free(strArr);
}

Node * insert(Node * node, Node * list)
{
	if (list == NULL) return node;
	else {
		list->next = insert(node,list->next);
	}
	return list;
}

void list_print(Node * list)
{
	if (list == NULL) return;
	else {
		printf("%d %d %d ",list->artime,list->priority,list->subnum);
		int i;
		for (i = 0; i < list->subnum;i++) printf("%d ",list->dur[i]);
		printf("\n");
		list_print(list->next);
	}
}

int qcount(Node * q0, Node * q1)
{
	int sum = 0;
	while (q0 != NULL) {
		sum ++;
		q0 = q0->next;
	}
	while (q1 != NULL) {
		sum ++;
		q1 = q1->next;
	}
	//printf("sum = %d\n",sum);
	return sum;
}

double ucpu(Node * list, int t){
	double stime;    //service time
	double uCPU;     //utilization of CPU
	int ind = 0;

	if(list == NULL) return 0;
	else {
		while(list != NULL){
			for(ind = 0; ind < list->subnum; ind++){
				stime += list->dur[ind];
			}
			list = list->next;
		}
		uCPU = stime / t / 64;
	}
	return uCPU;
}

