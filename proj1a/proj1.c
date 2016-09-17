#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct tasknode {
	int artime; //arrival time
	int detime; //departure time
	int priority; 
	int duration; //service time 

	struct tasknode * next;
}Node;


int readft(char * filename);
char * * explode(const char * str, const char * delims, int len);
void destroyStringArray(char * * strArr, int len);
int simulation(Node * list);
double ucpu(Node * list, int t);

Node * createnode(int artime, int priority, int duration, int detime);
Node * insert(Node * node, Node * list);
void list_print(Node * list);

int main(int argc, char * * argv)
{
	if (argc == 2) {
		readft(argv[1]);
	}
	else if (argc == 5) {

	}
	else {
		printf("error!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}

int readft(char * filename)
{
	FILE * fp = NULL;
	fp = fopen(filename,"r");
	if (!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", filename);
		return EXIT_FAILURE;
	}
	char * str = (char *) malloc(2000 * sizeof(char));//////
	char * * strarr = NULL;//////

	Node * list = NULL; //the complete list of tasks


	while (!feof(fp)) {
		fgets(str,100,fp);
		if (!feof(fp)) {
			strarr = explode(str," ",3);
			Node * node = createnode(atoi(strarr[0]),atoi(strarr[1]),atoi(strarr[2]), 0); //////
			list = insert(node, list);

			destroyStringArray(strarr, 3);
		}
	}
	int	t = simulation(list);
	
	
	free(str);
	fclose(fp);
	return EXIT_SUCCESS;
}

int simulation(Node * list)
{
	//sort outside (first mode) if needed


	//t = 0
	int t = list->artime; //current time
	int emptime = 0; //empty time of the cpu
	Node * q0 = NULL; //priority 0
	Node * q1 = NULL; //priority 1
	Node * node = NULL;
	int q1wait = 0;
	int q0wait = 0;
	int q0num = 0;
	int q1num = 0;
	int clength = 0; //current length 
	int peaks = 0;
	int slength = 0;
	Node * listcpy = list;

	//first task has been serviced(departured)
	//[current time] become first departure time(duration)
	t += list->duration;
	//set departure time = current time
	list->detime = t;
	//collect wait time seperately, task num = 1 or better idea
	node = createnode(list->artime, list->priority, list->duration, list->detime);
	//remove that task
	if(list -> next != NULL){
		list = list->next;
	}
	free(node);
	//move all the tasks arrived during that serive time to q1 and q0

	while (list != NULL && list->artime < t) {
		node = createnode(list->artime, list->priority, list->duration, list->detime);

		if (list->priority == 0) {clength++;  q0 = insert(node,q0); slength += clength; peaks++;}
		if (list->priority == 1) {clength++;  q1 = insert(node,q1); slength += clength; peaks++;}
		list = list->next;
	}
	if (list == NULL) {
	  	while (q0 != NULL){
			t += q0->duration;
			q0->detime = t;
			node = createnode(q0->artime, q0->priority, q0->duration,q0->detime);
			q0wait += q0->detime - q0->duration - q0->artime;
			q0num ++;
			q0 = q0->next;
			free (node);
			if (clength > 0) clength--; 
			// collect data
			while (list != NULL && list->artime < t){
				node = createnode(list->artime, list->priority, list->duration,list->detime);
				if (list->priority == 0) {clength++; q0 = insert(node, q0); slength += clength; peaks++;} 
				if (list->priority == 1) {clength++; q1 = insert(node, q1); slength += clength; peaks++;} 
				list = list->next;
			}
		}
		while (q1 != NULL){
			t += q1->duration;
			q1->detime = t;
			node = createnode(q1->artime,q1->priority, q1->duration,q1->detime);
			q1wait += q1->detime - q1->duration - q1->artime;
			q1num ++;
			q1 = q1->next;
			free(node);
			if (clength > 0) clength--;
			// collect data
			while (list != NULL && list->artime < t){			
				node = createnode(list->artime, list->priority, list->duration,list->detime);
				if (list->priority == 0) {clength++; q0 = insert(node, q0); slength += clength; peaks++;} 
				if (list->priority == 1) {clength++; q1 = insert(node, q1); slength += clength; peaks++;}
				list = list->next;
			}
			if (q0 != NULL) break;
		}
	}

	while (list != NULL){
		while (list != NULL && q0 == NULL && q1 == NULL) {
			emptime += list->artime - t;
			t = list->artime;
			t += list->duration;
			list->detime = t;
			node = createnode(list->artime, list->priority, list->duration, list->detime);
			

			list = list->next;
			free(node);
			if (clength > 0) clength--;
			while (list != NULL && list->artime < t) {
				node = createnode(list->artime, list->priority, list->duration, list->detime);
				if (list->priority == 0) {clength++; q0 = insert(node, q0); slength += clength; peaks++;} 
				if (list->priority == 1) {clength++; q1 = insert(node, q1); slength += clength; peaks++;}
				list = list->next;
			}
		}

		while (q0 != NULL){
			t += q0->duration;
			q0->detime = t;
			node = createnode(q0->artime, q0->priority, q0->duration,q0->detime);
			q0wait += q0->detime - q0->duration - q0->artime;
			q0num ++;
			q0 = q0->next;
			free (node);
			if (clength > 0) clength--;
			// collect data
			while (list != NULL && list->artime < t){
				node = createnode(list->artime, list->priority, list->duration,list->detime);
				if (list->priority == 0) {clength++; q0 = insert(node, q0); slength += clength; peaks++;} 
				if (list->priority == 1) {clength++; q1 = insert(node, q1); slength += clength; peaks++;}
				list = list->next;
			}
		}
		while (q1 != NULL){
			t += q1->duration;
			q1->detime = t;
			node = createnode(q1->artime,q1->priority, q1->duration,q1->detime);
			q1wait += q1->detime - q1->duration - q1->artime;
			q1num ++;
			q1 = q1->next;
			free(node);
			if (clength > 0) clength--;
			// collect data
			while (list != NULL && list->artime < t){			
				node = createnode(list->artime, list->priority, list->duration,list->detime);
				if (list->priority == 0) {clength++; q0 = insert(node, q0); slength += clength; peaks++;} 
				if (list->priority == 1) {clength++; q1 = insert(node, q1); slength += clength; peaks++;}
				list = list->next;
			}
			if (q0 != NULL) break;
		}
	}
	//printf("q0: %d %d\nq1: %d %d\n",q0num,q0wait,q1num,q1wait);
	
	FILE * fp = NULL;
	fp = fopen("proj1-a_output","w");	
	double awt0 = (double) q0wait/q0num;
	double awt1 = (double) q1wait/q1num;
	double aqlength = (double) slength/peaks;
	double aucpu = 0.0;
	aucpu = ucpu(listcpy, t);

	fprintf(fp,"%f\n%f\n%f\n%f",awt0,awt1,aqlength,aucpu);
	fclose(fp);
	//printf("clength = %d slength = %d peaks = %d",clength,slength,peaks);
    

	//simulation loop(ps. repeatively entering time == service time of all departured and blank time) {
	//if next task in the q blank(no task in q), collect that time(for cpu uti)(blank time = next arrival - current time)
	// and set *[current time]* = current + that blank time
	//service that task:means set *[current time]* += duration, then ......same as the first task


	//next task in q(0 first) has been serviced
	//[current time] = current time + that task's duration 
	//move all the tasks arrived during that serive time to q1 and q0
	//set departure time = current time
	//collect wait time separately, task++; or better idea
	//remove that task     }	 
	return t;
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

Node * createnode(int artime, int priority, int duration, int detime)
{
	Node * node = malloc(sizeof(Node));
	node->artime = artime;
	node->priority = priority;
	node->duration = duration;
	node->detime = detime;
	node->next = NULL;
	return node;
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
		printf("%d %d %d\n",list->artime,list->priority,list->duration);
		list_print(list->next);
	}
}

double ucpu(Node * list, int t){
	double itime;    //inter arrival time
	double citime;   //current interarrival time
	double stime;    //service time
	double aqlength; //average queue length
	
	double uCPU;      //utilization of CPU
	int sum = 0;

	if(list == NULL) return 0;
	else {
		while(list -> next != NULL){
			sum++;
			citime = list->artime;
			stime += list->duration;
			list = list->next;
			itime += (list->artime - citime);
		}
		stime += list->duration;
		sum++;
	}
	//printf("%f\n", stime);
	//printf("%f\n", itime);
	printf("%d\n", t);
	aqlength = ((1.0/(itime / sum)) / (1.0/(stime / sum))) / (1 - ((1.0/(itime / sum)) / (1.0/(stime / sum))));
	uCPU = stime / t;
	//printf("%f\n", uCPU);
	return uCPU;
}






