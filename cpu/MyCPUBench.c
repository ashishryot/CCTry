#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h>

//FILL ME IN
#define ITERATIONS 100000000000

//Functions in this file:
static void scan_dir(const char *dir);
void readfile(const char str[], int j);
void compute_1();
void* operationQP(void *arg);
void* operationHP(void *arg);
void* operationSP(void *arg);
void* operationDP(void *arg);
void cpuQP(int i);
void cpuHP(int i);
void cpuSP(int i);
void cpuDP(int i);



struct work_concurrency {
  char work[10];
  char con[1];
}wc[12];

//The Thread program to execute for Workload SP
void* operationSP(void *arg){
	long long *a=(long long*)arg;
	long long val=*a;
	//printf("Value of arg is: %lld\n",val);
	int aa=2000,b=10;
	long long i;

	//do SP Arithmetic Operations
	for(i=0;i<val;i++){
		aa=(aa*b)/b;		//	2 operations
	}

	pthread_exit(0);
}

//The Thread program to execute for Workload DP
void* operationDP(void *arg){
	long long *a=(long long*)arg;
	long long val=*a;
	//printf("Value of arg is: %lld\n",val);
	double sum=2000000.08542;
	double x=9340059.0025;
	long long i;

	//do DP Arithmetic Operations
	for(i=0;i<val;i++){
		sum=(x/sum)*sum;		//	2 Operations
	}

	pthread_exit(0);
}

//The Thread program to execute for Workload HP
void* operationHP(void *arg){
	long long *a=(long long*)arg;
	long long val=*a;
	//printf("Value of arg is: %lld\n",val);
	short sum=100;
	long long i;

	//do HP Arithmetic Operations
	for(i=0;i<val;i++){
		sum=sum*10/10;		//	2 operations
	}

	pthread_exit(0);
}

//The Thread program to execute for Workload QP
void* operationQP(void *arg){
	long long *a=(long long*)arg;
	long long val=*a;
	//printf("Value of arg is: %lld\n",val);
	char c='a';
	long long i;
	
	//do SP Arithmetic Operations
	for(i=0;i<val;i++){
		c=c*2/2;		//	2 operations
	}

	pthread_exit(0);
}

void* operation(void *arg){
	long long *a=(long long*)arg;
	long long val=*a;
	printf("Value of arg is: %lld\n",val);
	
	pthread_exit(0);
}

//The function to scan for file names in Directory:
static void scan_dir(const char *dir){

	struct dirent *e;
	DIR* d;
	d = opendir( dir );
	if (d == 0) {
        perror("opendir");
        return;
    }
	
	
	int j=0;
    while ((e = readdir(d))) {
        if(strstr(e->d_name, "dat") != NULL) {
    	
        	printf("%s\n", e->d_name);
        	readfile(e->d_name,j);		//Function call to the readfile() function
        	
		//printf("%s : %s\n",wc[j].work,wc[j].con);
       		j++;
   		}
	}
   
    closedir(d);
   
}

//The function to read the file with .dat type and store its data in the structure: 
void readfile(const char str[], int j){
 	
 char c[1000];
    FILE *fp;

    if ((fp = fopen(str, "r")) == NULL)
    {
        printf("Error! opening file. File does not exist.");
        // exit from program if file pointer returns NULL.
        exit(1);         
    }
int i=0;
    while(!feof(fp)&&i<3){
		
		if(i==0){
			fgets(wc[j].work,3,fp);
				//printf("Data from the file:\n%s", wc[j].work);
			
		}
		if(i==1){
			fgets(c,2,fp);
				//printf("Data from the file:\n%s", wc[j].con);
		}
		if(i==2){
			fgets(wc[j].con,2,fp);
				//printf("Data from the file:\n%s", wc[j].con);
		}
    	i++;
	
	}
	fclose(fp);
	printf("%s : %s\n",wc[j].work,wc[j].con);
       		
 }

//The function to call the diffferent Workload specific functions:
void compute_1(){
	int i;
	for(i=0;i<12;i++){
		if(strstr(wc[i].work,"QP")!=NULL){
			cpuQP(i);
		}
		else if(strstr(wc[i].work,"HP")!=NULL){
			cpuHP(i);
		}
		else if(strstr(wc[i].work,"SP")!=NULL){
			cpuSP(i);
		}
		else if(strstr(wc[i].work,"DP")!=NULL){
			cpuDP(i);
		}
	}
	
}

/*
	*
	*
	*The main function:
	*
	*
*/
int main(int argc, char *argv[]) {
    
    scan_dir(".");
    compute_1();
	long long itr=ITERATIONS;
    printf("%lld\n",itr);
    return 0;

}

/**
	*
	*	Below are the functions to create threads
	*	andcalculate their running time and GLOPS
	* 
*/

//This function creates the threads for QP
void cpuQP(int i){
	printf("This is QP\n");
	int num_thread;
	long long arg;
	if(strstr(wc[i].con,"1")!=NULL){
		printf("This is 1 thread\n");
		num_thread=1;
		arg=ITERATIONS;	
	}
	else if(strstr(wc[i].con,"2")!=NULL){
		printf("This is 2 thread\n");
		num_thread=2;
		arg=ITERATIONS/2;	
	}
	else if(strstr(wc[i].con,"4")!=NULL){
		printf("This is 4 thread\n");
		num_thread=4;
		arg=ITERATIONS/4;	
	}
	pthread_t tid[num_thread];
	int j;
	struct timeval time1, time2;
	

	gettimeofday(&time1, NULL);
	for(j=0;j<num_thread;j++){
		pthread_create(&tid[j], NULL, operationQP, &arg);
	}
	for(j=0;j<num_thread;j++){
		pthread_join(tid[j],NULL);
	}
	
	gettimeofday(&time2, NULL);
	double time=(time2.tv_sec+(time2.tv_usec/1000000.0))-(time1.tv_sec+(time1.tv_usec/1000000.0));
	
	printf("*************Time is: %f\n",time);		//Time of Execution printed
	
	//GOPS Calculation
	double gflops=((ITERATIONS*2.0)/time)/1000000000.0;
	printf("GFLOPS: %f\n",gflops);
	
	FILE *fp;
	fp=fopen("./output/cpu_SP_1thread.out.dat","a");
	if(fp==NULL){
		printf("Cannot write because the file is not there.\n");
	}
	printf("Printing in file now\n");
	fprintf(fp, "%s\t\t\t%d\t\t\t%f\t\t\t\t\t\t%d\n", wc[i].work, num_thread, gflops, 100);
	fclose(fp);
	
	
}

//This function creates the threads for HP
void cpuHP(int i){
	printf("This is HP\n");
	int num_thread;
	long long arg;
	if(strstr(wc[i].con,"1")!=NULL){
		printf("This is 1 thread\n");
		num_thread=1;
		arg=ITERATIONS;
		//return NULL;	
	}
	else if(strstr(wc[i].con,"2")!=NULL){
		printf("This is 2 thread\n");
		num_thread=2;
		arg=ITERATIONS/2;	
	}
	else if(strstr(wc[i].con,"4")!=NULL){
		printf("This is 4 thread\n");
		num_thread=4;
		arg=ITERATIONS/4;	
	}
	
	pthread_t tid[num_thread];
	int j;
	struct timeval time1, time2;
	

	gettimeofday(&time1, NULL);
	for(j=0;j<num_thread;j++){
		pthread_create(&tid[j], NULL, operationHP, &arg);
	}
	for(j=0;j<num_thread;j++){
		pthread_join(tid[j],NULL);
	}
	
	gettimeofday(&time2, NULL);
	double time=(time2.tv_sec+(time2.tv_usec/1000000.0))-(time1.tv_sec+(time1.tv_usec/1000000.0));
	
	printf("*************Time is: %f\n",time);		//Time of Execution printed
	
	//GOPS Calculation
	double gflops=((ITERATIONS*2.0)/time)/1000000000.0;
	printf("GFLOPS: %f\n",gflops);
	
	FILE *fp;
	fp=fopen("./output/cpu_SP_1thread.out.dat","a");
	if(fp==NULL){
		printf("Cannot write because the file is not there.\n");
	}
	printf("Printing in file now\n");
	fprintf(fp, "%s\t\t\t%d\t\t\t%f\t\t\t\t\t\t%d\n", wc[i].work, num_thread, gflops, 100);
	fclose(fp);
	
}

//This function creates the threads for QP
void cpuSP(int i){
	printf("This is SP\n");
	int num_thread;
	long long arg;
	if(strstr(wc[i].con,"1")!=NULL){
		printf("This is 1 thread\n");
		num_thread=1;
		arg=ITERATIONS;
		//return NULL;	
	}
	else if(strstr(wc[i].con,"2")!=NULL){
		printf("This is 2 thread\n");
		num_thread=2;
		arg=ITERATIONS/2;	
	}
	else if(strstr(wc[i].con,"4")!=NULL){
		printf("This is 4 thread\n");
		num_thread=4;
		arg=ITERATIONS/4;	
	}
	pthread_t tid[num_thread];
	int j;
	struct timeval time1, time2;
	

	gettimeofday(&time1, NULL);
	for(j=0;j<num_thread;j++){
		pthread_create(&tid[j], NULL, operationSP, &arg);
	}
	for(j=0;j<num_thread;j++){
		pthread_join(tid[j],NULL);
	}
	
	gettimeofday(&time2, NULL);
	double time=(time2.tv_sec+(time2.tv_usec/1000000.0))-(time1.tv_sec+(time1.tv_usec/1000000.0));
	
	printf("*************Time is: %f\n",time);		//Time of Execution printed
	
	//GOPS Calculation
	double gflops=((ITERATIONS*2.0)/time)/1000000000.0;
	printf("GFLOPS: %f\n",gflops);
	
	FILE *fp;
	fp=fopen("./output/cpu_SP_1thread.out.dat","a");
	if(fp==NULL){
		printf("Cannot write because the file is not there.\n");
	}
	printf("Printing in file now\n");
	fprintf(fp, "%s\t\t\t%d\t\t\t%f\t\t\t\t\t\t%d\n", wc[i].work, num_thread, gflops, 100);
	fclose(fp);
		
}

//This function creates the threads for QP
void cpuDP(int i){
	printf("This is DP\n");
	int num_thread;
	long long arg;
	if(strstr(wc[i].con,"1")!=NULL){
		printf("This is 1 thread\n");
		num_thread=1;
		arg=ITERATIONS;
		//return NULL;	
	}
	else if(strstr(wc[i].con,"2")!=NULL){
		printf("This is 2 thread\n");
		num_thread=2;
		arg=ITERATIONS/2;	
	}
	else if(strstr(wc[i].con,"4")!=NULL){
		printf("This is 4 thread\n");
		num_thread=4;
		arg=ITERATIONS/4;	
	}
	pthread_t tid[num_thread];
	int j;
	struct timeval time1, time2;
	gettimeofday(&time1, NULL);
	for(j=0;j<num_thread;j++){
		pthread_create(&tid[j], NULL, operationDP, &arg);
	}
	for(j=0;j<num_thread;j++){
		pthread_join(tid[j],NULL);
	}
	gettimeofday(&time2, NULL);
	double time=(time2.tv_sec+(time2.tv_usec/1000000.0))-(time1.tv_sec+(time1.tv_usec/1000000.0));
	
	printf("*************Time is: %f\n",time);		//Time of Execution printed
	
	//GOPS Calculation
	double gflops=((ITERATIONS*2.0)/time)/1000000000.0;
	printf("GFLOPS: %f\n",gflops);
	
	FILE *fp;
	fp=fopen("./output/cpu_SP_1thread.out.dat","a");
	if(fp==NULL){
		printf("Cannot write because the file is not there.\n");
	}
	printf("Printing in file now\n");
	fprintf(fp, "%s\t\t\t%d\t\t\t%f\t\t\t\t\t\t%d\n", wc[i].work, num_thread, gflops, 100);
	fclose(fp);
	
}

