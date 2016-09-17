#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int readft(char * filename);
char * * explode(const char * str, const char * delims, int len);

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
	char * linestr = (char *) malloc(2000 * sizeof(char));//////
	int linenum = 0;

	while (!feof(fp)) {
		fgets(linestr,100,fp);
		linenum ++;
	}
	//printf("line number = %d\n",linenum);
	int * * intarr = malloc(sizeof(int *)*linenum); //////
	int i = 0;
	fseek(fp,0,SEEK_SET);
	char * * strarr = NULL;//////

	while (!feof(fp)) {
		fgets(linestr,100,fp);
		strarr = explode(linestr," ",3);
		//printf("%s\n",linestr);
		intarr[i] = malloc(sizeof(int)*3);//////
		linestr = malloc(sizeof(char)*100);//////
		intarr[i][0] = atoi(linestr[0]);
		intarr[i][1] = atoi(linestr[2]);
		intarr[i][2] = atoi(linestr[4]);
		printf("%d %d %d\n",intarr[i][0],intarr[i][1],intarr[i][2]);
		i++;
		free(linestr);
	}


	free(linestr);
	fclose(fp);

	return EXIT_SUCCESS;
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
