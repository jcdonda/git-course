#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

sem_t sens[3], trab;

int ciclos=0;

void *sensor(void *argg){

	int id = *(int *) argg;
	
	while(1){
	
		sem_wait(&sens[id]);
	
		if(id==0) printf("S%d esta midiendo la luz...\n", id);
		else if(id==1) printf("S%d esta midiendo la humedad...\n", id);
		else if(id==2) printf("S%d esta midiendo la temp...\n", id);
		fflush(stdout);
	
		sem_post(&trab);
	}
}

void *trabajador(void *argg){

	int i;
	
	while(1){
		for(i=0;i<3;i++)
			sem_wait(&trab);
		
		ciclos++;
		printf("El dispositivo esta trabajando...\n"); 
		printf("Ciclo Nº%d\n",ciclos); fflush(stdout);
	
		for(i=0;i<3;i++)	
			sem_post(&sens[i]);
	}
}

int main(){

	pthread_t th[4];
	pthread_attr_t attr;

	sem_init(&sens[0],0,1);
	sem_init(&sens[1],0,1);
	sem_init(&sens[2],0,1);
	sem_init(&trab,0,0);
	
	pthread_attr_init(&attr);
	int i,id[3];
	for(i=0;i<3;i++){
	
		id[i]=i;
		pthread_create(&th[i],&attr,sensor,&id[i]);
	}
	pthread_create(&th[3],&attr,trabajador,NULL);
	
	pthread_join(th[3],NULL);
	
	return 0;
}
