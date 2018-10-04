#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define N 5 // numero de aviones

sem_t mutex; //pista de aterrizaje en exclusion mutua
sem_t solicitudnorte, solicitudsur;
sem_t permisonorte, permisosur;

int naviones=N;

void *aviones(void *argg){
	
	int id = *(int *) argg;
	int lado = rand()%2;
	
	sleep(rand()%10);
	
	if(lado==0){ //SUR
		printf("Avion %d solicita permiso para aterrizar en",id);
		printf("la pista SUR.\n"); fflush(stdout);
		sem_post(&solicitudsur);
		sem_wait(&permisosur);
		
	} else if(lado==1){ //NORTE
		printf("Avion %d solicita permiso para aterrizar en",id);
		printf("la pista NORTE.\n"); fflush(stdout);
		sem_post(&solicitudnorte);
		sem_wait(&permisonorte);
	}
	printf("Avion %d aterrizando por la pista ",id);
	if(lado==0) printf("sur.\n");
	if(lado==1) printf("norte.\n"); fflush(stdout);
	
	sleep(3);
	printf("El avion %d ha aterrizado\n",id); fflush(stdout);
	sem_post(&mutex); // pista libre
}

void *controladora(void *argg){

	int id = *(int *) argg;
	while(naviones>0){
		if(id==0){
			printf("Controladora Sur esperando solicitud\n");
			sem_wait(&solicitudsur);
		}
		if(id==1){
			printf("Controladora Norte esperando solicitud\n");
			sem_wait(&solicitudnorte);
		}
		sem_wait(&mutex); //esperando a pista
	
		if(id==0){
			printf("Permiso para pista SUR concedido.\n");	
			sem_post(&permisosur);
		}
		if(id==1){
			printf("Permiso para pista NORTE concedido.\n");
			sem_post(&permisonorte);
		}
		naviones--; 
	}
}

int main(){
	
	pthread_t th[2];//controladoras
	pthread_t avion[N]; //una hebra por cada avion
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	sem_init(&mutex,0,1);// se crean los semaforos
	sem_init(&solicitudnorte,0,0);
	sem_init(&solicitudsur,0,0);
	sem_init(&permisonorte,0,0);
	sem_init(&permisosur,0,0);

	int i, id[N];
	for(i=0;i<N;i++){ // se crean las hebras
		
		id[i]=i;
		pthread_create(&avion[N],&attr,aviones,&id[i]);
	}
	int th0=0, th1=1;
	pthread_create(&th[0],&attr,controladora,&th0);
	pthread_create(&th[1],&attr,controladora,&th1);
	
	pthread_join(th[0],NULL);//Con el ultimo avion se quedara una 
	pthread_join(th[1],NULL);//controladora siempre esperando
		
	return 0;
}
