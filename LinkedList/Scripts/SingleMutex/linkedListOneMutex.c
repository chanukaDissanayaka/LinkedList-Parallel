#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int thread_count;
pthread_mutex_t mutex;


struct list_node_s *head ;
struct list_node_s **head_p ;

int n;
int m;
double m_member;
double m_insert;
double m_delete;

int total_member;
int total_insert;
int total_delete;

int completed_operation_count = 0;
int completed_member_count = 0;
int completed_insert_count = 0;
int completed_delete_count = 0;



struct list_node_s {
	int data;
	struct list_node_s *next;
};


int Member(int value, struct list_node_s *head_p){
	struct list_node_s *curr_p = head_p;

	while (curr_p !=NULL && (curr_p->data) < value){
		curr_p = curr_p->next;
	}

	if(curr_p == NULL || (curr_p->data) > value){
		printf("not a member\n");
		return 0;
	}
	else {
		printf("member\n");
		return 1;
	}
}


int insert(int value, struct list_node_s **head_p){
	

	struct list_node_s *curr_p = *head_p;
	struct list_node_s *pred_p = NULL;
	struct list_node_s *temp_p;

	while (curr_p != NULL && curr_p->data < value){
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if( curr_p == NULL || curr_p->data > value){
		temp_p = malloc(sizeof(struct list_node_s));
		temp_p->data = value;
		temp_p->next = curr_p;

		if(pred_p == NULL){
			*head_p = temp_p;
		}
		else{
			pred_p->next = temp_p;

		printf("insert successful\n");
		return 1;
		}
	}
	else {
		printf("insert not successful\n");
		return 0;
	}
}


int delete(int value, struct list_node_s **head_pp){
	struct list_node_s *curr_p = *head_pp;
	struct list_node_s *pred_p = NULL;

	while(curr_p != NULL && curr_p-> data < value){
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if(curr_p != NULL && curr_p->data == value){
		if(pred_p == NULL){
			*head_pp = curr_p->next;
			free(curr_p);
		}
		else{
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		printf("delete successful\n");
		return 1;
	}
	else {
		printf("error deleting\n");
		return 0;
	}
}

void print_list(struct list_node_s **head){
	struct list_node_s *curr_p = *head;
	while(curr_p != NULL){
		printf("%d, ", curr_p->data);
		curr_p = curr_p->next;
	}
	printf("\n");
}



void generateList(int size){
	long lower = 0;
	long upper = pow(2, 16)-1;
	long count;
	srand(time(NULL));

	for(count =0; count< size; count++){
		long ran = (rand() % (upper+1 -lower) + lower);
		insert(ran, head_p);
	}		
}

void writeResultstoFile(float result){
	FILE *fp;
	//fp = fopen ("results-case3.txt","a");
	fp = fopen ("results-case3-thread_count_8.txt","a");
	fprintf(fp, "%.6f\n", result);
}

void *randomOperation(){


	while (completed_operation_count < m) {

		int random_operation = rand() % 3 ;

		int random_value = rand() % 65535 ;

		//aquire lock
		pthread_mutex_lock(&mutex);

		//make sure total opertion count is not exceeded
		if(completed_operation_count < m){

			if(random_operation == 0 && completed_member_count < total_member){

				Member(random_value, head);
				completed_member_count++ ;
				completed_operation_count++;
			}

			else if (random_operation == 1 && completed_insert_count < total_insert){

				insert(random_value, head_p);
				completed_insert_count++;
				completed_operation_count++;
			}

			else if(random_operation == 2 && completed_delete_count < total_delete){


				delete(random_value, head_p);
				completed_delete_count++;
				completed_operation_count++;
			}
		}
		//release lock
		pthread_mutex_unlock(&mutex);
		
	}

}


int main(int argc, char* argv[]){

	//initialising head. head's value is set to -1 for convineient
	head = (struct list_node_s*)malloc(sizeof(struct list_node_s));
	head->data = -1;
	head->next = NULL;

	//pointer to the list
	head_p = &head;

	int thread_count;
	long thread;
	pthread_t *thread_handles;

	clock_t start_time;
	clock_t end_time;
	double timeSpent ;


	/*int v;
	scanf("%d", &v);
	printf(" input is : %d\n", v);*/

	long ran = pow(2,16);

	thread_count = strtol(argv[1], NULL, 10);
	n = strtol(argv[2], NULL, 10);
	m = strtol(argv[3], NULL, 10);

	m_member = (double) atof(argv[4]);
	m_insert = (double) atof(argv[5]);
	m_delete = (double) atof(argv[6]);

	total_member = (int) m * m_member;
	total_insert = (int) m * m_insert;
	total_delete = (int) m * m_delete;


	thread_handles = malloc(thread_count* sizeof(pthread_t));

	//generate list with given n
	generateList(n);
	printf("-------------------------List generated---------------------------------------------------------\n");

	start_time = clock();

	for(thread = 0; thread < thread_count; thread++){
		pthread_create(&thread_handles[thread], NULL, randomOperation, (void*) thread);
	}

	for(thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	end_time = clock();

	timeSpent = ((double) (end_time - start_time)/ CLOCKS_PER_SEC);
	printf("\n--------------------------------results-----------------------------------------\n\n");
	printf("threads : %li\n", thread_count);
	printf("operations : %li\n", m);
	printf("m_member : %.3f\n", m_member);
	printf("m_insert : %.3f\n", m_insert);
	printf("m_delete : %.3f\n", m_delete);
	printf("time spent : %.8f\n", timeSpent);
	
	pthread_mutex_destroy(&mutex);
	writeResultstoFile(timeSpent);
}
