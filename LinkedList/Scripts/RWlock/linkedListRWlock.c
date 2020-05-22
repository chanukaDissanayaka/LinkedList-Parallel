#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int thread_count;
pthread_rwlock_t rwlock;


struct list_node_s *head ;
struct list_node_s **head_p ;

int thread_count;

int n;
int m;
double m_member;
double m_insert;
double m_delete;

int total_member;
int total_insert;
int total_delete;


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
		return 0;
	}
	else {
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

		return 1;
		}
	}
	else {
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
		return 1;
	}
	else {
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
	//fp = fopen ("test.txt","a");
	fp = fopen ("results-case1-thread_count_8.txt","a");
	fprintf(fp, "%.6f\n", result);
}

void *randomOperation(){

	long thread_m;
	long thread_m_member;
	long thread_m_insert;
	long thread_m_delete;

	//divide the operation between threads. add 1 to execute without loss
	thread_m = (m / thread_count) + 1;
	thread_m_member = (int)(total_member / (float) thread_count) + 1;
	thread_m_insert = (int)(total_insert / (float) thread_count) + 1;
	thread_m_delete = (int)(total_delete / (float) thread_count) + 1;


	int thread_completed_operations = 0;
	int thread_completed_member = 0;
	int thread_completed_insert = 0;
	int thread_completed_delete = 0;

	while (thread_completed_operations < thread_m) {

		int random_operation = rand() % 3 ;

		int random_value = rand() % 65535 ;


		if(random_operation == 0 && thread_completed_member < thread_m_member){

			pthread_rwlock_rdlock(&rwlock);
			Member(random_value, head);
			thread_completed_member++ ;
			pthread_rwlock_unlock(&rwlock);

		}

		else if (random_operation == 1 && thread_completed_insert < thread_m_insert){

			pthread_rwlock_wrlock(&rwlock);
			insert(random_value, head_p);
			thread_completed_insert++;
			pthread_rwlock_unlock(&rwlock);
		}

		else if(random_operation == 2 && thread_completed_delete < thread_m_delete){

			pthread_rwlock_wrlock(&rwlock);
			delete(random_value, head_p);
			thread_completed_delete++;
			pthread_rwlock_unlock(&rwlock);
		}
		thread_completed_operations = thread_completed_member + thread_completed_insert + thread_completed_delete;	
	}

}


int main(int argc, char* argv[]){

	//initialising head. head's value is set to -1 for convineient
	head = (struct list_node_s*)malloc(sizeof(struct list_node_s));
	head->data = -1;
	head->next = NULL;

	//pointer to the list
	head_p = &head;

	long thread;
	pthread_t *thread_handles;

	clock_t start_time;
	clock_t end_time;
	double timeSpent ;

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
	
	pthread_rwlock_destroy(&rwlock);
	//writeResultstoFile(timeSpent);
}
