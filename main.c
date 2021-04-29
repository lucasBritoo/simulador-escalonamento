#include <stdio.h>
#include <stdlib.h>


typedef struct task_struct Task_struct;

struct task_struct{
	int id;
	int quantum;
	int time_sleep;
	int time_exe;
	int prioridade;
	struct task_struct *prox_task;
};


Task_struct* init_process(int cont){

	int id, quantum, prioridade;
	Task_struct *processo = (Task_struct*) malloc(sizeof(Task_struct)); 

	if(processo == NULL) printf("Falha ao alocar processo");

	else{
		

		printf("Digite o quantum do processo: ");
		scanf("%i", &quantum);

		printf("Digite a prioridade do processo: ");
		scanf("%i", &prioridade);

		
		processo->id 		 = cont;
		processo->quantum 	 = quantum;
		processo->prioridade = prioridade;
		processo->time_sleep = 0;
		processo->time_exe   = 0;
		processo->prox_task  = NULL;
		
	}
	return processo;
}

//ADICIONAR UM PROCESSO
void add_process(Task_struct *lista, int cont){

	if(lista->prox_task == NULL) {
		lista->prox_task = init_process(cont);
	}

	else{
		Task_struct *aux = lista;

		while(aux->prox_task != NULL){
			aux = aux->prox_task;
		}
		aux->prox_task = init_process(cont);
	}
}


//REMOVER UM PROCESSO
Task_struct* remove_task(Task_struct *lista, int id){

	Task_struct *aux = lista;

	if(lista->id == id){
		lista = aux->prox_task;
		free(aux);
		//printf("Processo removido com sucesso...\n");
		return lista;
	}
	else{
		while(aux->prox_task->id != id){
			aux = aux->prox_task;
		}

		if(aux->prox_task->id == id){
			Task_struct *processo = aux->prox_task;
			//printf("Processo %i removido com sucesso...\n", processo->id);
			aux->prox_task = processo->prox_task;
			free(processo);
			
			return lista;
		}
		else{
			printf("Processo não encontrado com esse ID\n");
			return lista;
		}	
	}
}


//MOSTRAR PROCESSO
void mostrar(Task_struct* lista){

	if(lista == NULL){
		printf("Não há processos na lista...\n");
	}
	else{
		printf("\nLISTA DE PROCESSOS: \n\n");
		Task_struct *aux = lista;

		printf("|  ID  |    QUANTUM  |    PRIORIDADE  |    TIME SLEEP  |    TIME_EXE    |\n");
		do{
			
			printf("|  %i   |       %i     |        %i       |         %i      |         %i      |\n", aux->id, aux->quantum, aux->prioridade, aux->time_sleep, aux->time_exe);
			aux = aux->prox_task;

		}while(aux != NULL);
		free(aux);
	}
}

void round_robin(Task_struct *copy, int cont1){
	int cont = 0;
	int timer = 0;

	
	Task_struct *aux; //= copy_list(&copy);
	Task_struct *aux2;
	

	if(copy == NULL){
		printf("Lista Vazia...\n");
	}
	else{
		printf("\n----------------------EXECUTANDO ROUND ROBIN-------------------------\n");
		//printf("_____________________________________________________________________\n");

		aux = copy;
		aux2 = aux;

		do{
			if(aux->quantum > 0){
				printf("EXECUTANDO PROCESSO %i  -> QUANTUM: %i\n", aux->id, aux->quantum);
				aux->quantum  = aux->quantum - 1;
				aux->time_exe = aux->time_exe + 1;
				//cont++;

				if(aux->quantum == 0){
					cont++;
					aux->time_sleep = timer;
				}
				timer++;
			}

			if(aux->prox_task == NULL){
				aux = aux2;
			}
			else{
				aux = aux->prox_task;
			}
			
			
		}while(cont != cont1);

	}
}

void prioridade(Task_struct *copy, int cont1){
	int cont = 0;
	int timer = 0;

	
	Task_struct *aux;
	Task_struct *aux_maior;
	

	if(copy == NULL){
		printf("Lista Vazia...\n");
	}
	else{
		printf("\n----------------------EXECUTANDO PRIORIDADE--------------------------\n");
		//printf("_____________________________________________________________________\n");

		aux = copy;
		aux_maior = copy;
		

		do{
			if((aux->prioridade) > (aux->prox_task->prioridade)){
				aux_maior = aux;
			}

			if(aux->prox_task == NULL){

				if(aux_maior->quantum > 0) {
					printf("EXECUTANDO PROCESSO %i  -> QUANTUM: %i\n", aux_maior->id, aux_maior->quantum);
					aux_maior->quantum = aux_maior->quantum - 1;
					aux_maior->time_exe = aux_maior->time_exe + 1;
					aux_maior->prioridade = aux_maior->prioridade -1;

					if(aux->quantum == 0){
						cont++;
						aux_maior->time_sleep = timer;
					}
					timer++;
				}
				aux = copy;
			}
			else{
				aux = aux->prox_task;
			}	
		}while(cont != cont1);
	}
}

void finalizar(Task_struct *lista){
	Task_struct *aux = lista;

	do{
		
		aux->quantum = aux->time_exe;
		aux = aux->prox_task;
	}while(aux != NULL);

	printf("-----------------------------------------------------------------------\n");
	mostrar(lista);
	printf("\nAperter uma tecla para continuar...");
	char x;
	scanf("%s", &x);
}

Task_struct* copy_list(Task_struct *lista, Task_struct *copy){

	
	Task_struct *aux = lista;
	Task_struct *anterior = NULL;

	do{
		Task_struct *processo = (Task_struct*) malloc(sizeof(Task_struct));

		processo->id 		 = aux->id;
		processo->quantum 	 = aux->quantum;
		processo->prioridade = aux->prioridade;
		processo->time_sleep = aux->time_sleep;
		processo->time_exe   = aux->time_exe;
		processo->prox_task  = NULL;

		if(anterior == NULL){
			copy = processo;
		}
		else{
			anterior->prox_task = aux;
		}

		anterior = processo;
		aux = aux->prox_task;

	}while(aux != NULL);
	
	free(aux);
	free(anterior);
	return copy; 
}


int main(){
	int cont = 0, cont1 = 0;
	int opc  = 1;
	int id;

	Task_struct *lista_processos = (Task_struct*) malloc(sizeof(Task_struct));
	Task_struct *copy = (Task_struct*) malloc(sizeof(Task_struct));
	//Task_struct *lista_processos;

	printf("_____ - _____ - _____ - _____ - _____ - _____ - _____ - _____ - _____ - _____\n");
	printf("|                                                                            |\n");
	printf("|                  SIMULADOR DE ESCALONAMENTO DE PROCESSOS                   |\n");
	printf("|        ROUND ROBIN   *     PRIORIDADE       *    FILA MÚLTIPLAS FILAS      |\n");
	printf("|                                                                            |\n");
	printf("| ALUNO: LUCAS DA SILVA BRITO            CURSO: ENGENHARIA DA COMPUTAÇÃO     |\n");
	printf("| DISCIPLINA: SISTEMAS OPERACIONAIS I 	 DATA: 30/04/2021                    |\n");
	printf("|                                                                            |\n");
	printf("|_____ - _____ - _____ - _____ - _____ - _____ - _____ - _____ - _____ - ____|\n\n");


	while(opc != 0) {

		printf("_____________________________________________________________________\n");
		printf("|                       MENU DE OPÇÕES                              |\n");
		printf("|( 1 ) - ADICIONAR UM NOVO PROCESSO                                 |\n");
		printf("|( 2 ) - REMOVER UM PROCESSO                                        |\n");
		printf("|( 3 ) - EXECUTAR ROUND ROBIN                                       |\n");
		printf("|( 4 ) - EXECUTRAR PRIORIDADE                                       |\n");
		printf("|( 5 ) - EXECUTAR MúLTIPLAS FILAS                                   |\n");
		printf("|( 6 ) - MOSTRAR PROCESSOS                                          |\n");
		printf("|( 0 ) - SAIR                                                       |\n");
		printf("Digite uma opção: ");
		scanf("%i", &opc);


		switch(opc){
			case 1:
				if(cont != 0 ){
					add_process(lista_processos, cont++);
				}
				else{
					lista_processos = init_process(cont++);
				}
				cont1++; 
				break;

			case 2:
				printf("\nDigite o ID do processo que deseja remover: ");
				scanf("%i", &id); 
				lista_processos = remove_task(lista_processos, id);
				printf("Processo removido com sucesso...\n");
				cont1--;
				break;

			case 3:
				round_robin(lista_processos, cont1);
				finalizar(lista_processos);
				break;

			case 4: 
				prioridade(lista_processos, cont1);
				finalizar(lista_processos);
				break;
			case 6:
				mostrar(lista_processos);
				//mostrar(copy);
				break;
		}

	}

	return 0;

}