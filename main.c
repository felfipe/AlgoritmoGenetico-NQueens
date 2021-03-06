#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int** cria_tabuleiro(int n);
int insere_peca(int** tabuleiro, int posX, int posY, int tipo_peca);
int retira_peca(int** tabuleiro, int posX, int posY);
int verifica_ataque(int** tabuleiro, int n, int posX, int posY);
void apaga_tabuleiro(int **tabuleiro, int n);
void limpa_tabuleiro(int **tabuleiro, int n);
void printa_tabuleiro(int **tabuleiro, int n);
int insere_individuo(int *individuo,int** tabuleiro, int n);
/**** funnções do AG ****/

int** inicializa_populacao(int n);
void apaga_populacao(int** populacao);
void preenche_aleatorio(int* vetor, int n);
int* gera_vetor_fitness(int** population, int** tabuleiro, int n);
int avalia_individuo(int **tabuleiroint, int* individuo, int n);
int recombina_individuos_elitismo(int** population, int n, int* fitness);
int recombina_individuos_torneio(int*** population, int n, int* fitness);
int escolhe_pai(int* fitness);
int mutacao(int** population, int n, int taxa_mutacao);

#define MAX_POPULATION 50
#define MUTATION_RATE  1
#define PROB_PAI 0.5     // PROBABILIDADE DE SELECIONAR O MELHOR INDIVIDUO PARA SER PAI NO TORNEIO DE 2


int main()
{
    srand(time(NULL));
    int n; //tamanho do tabuleiro 
    printf("Digite a dimensao n desejada para o tabuleiro: ");
    scanf("%d", &n);
    int **tabuleiro = cria_tabuleiro(n);
    if(tabuleiro == NULL){
        printf("\nErro, tabuleiro não alocado.");
        return 0;
    }
    limpa_tabuleiro(tabuleiro, n);
    int genocidios = 0;
    int criterio_parada = 1;
    int melhor_individuo[n];
    int geracao = 0;
    int taxa_mutacao = 1;
    int fitness_anterior = 1000000;
    while(criterio_parada){
      int **populacao = inicializa_populacao(n);
      if(geracao > 0)
        if(rand()%2)
          for(int i = 0; i < n; i++)
            populacao[0][i] = melhor_individuo[i];
      while(1){
        int* fitness = gera_vetor_fitness(populacao,tabuleiro,n);
        //int index_melhor_individuo = recombina_individuos_torneio(&populacao,n,fitness);    
        int index_melhor_individuo = recombina_individuos_elitismo(populacao,n,fitness);
        for(int i = 0; i < n; i++)
          melhor_individuo[i] = populacao[index_melhor_individuo][i];
        mutacao(populacao,n, taxa_mutacao);
        limpa_tabuleiro(tabuleiro, n);
        insere_individuo(populacao[index_melhor_individuo], tabuleiro, n);
        //printa_tabuleiro(tabuleiro, n);
        geracao++;
        int fitness_melhor_individuo = avalia_individuo(tabuleiro, melhor_individuo, n);
        //printf("geracao: %d\n",geracao);
        //printf("fitness: %d\n", fitness_melhor_individuo);
        free(fitness);
        if(fitness_melhor_individuo == 0){
          insere_individuo(melhor_individuo, tabuleiro, n);
          printa_tabuleiro(tabuleiro, n);
          printf("geracao: %d\n",geracao);
          printf("genocidios: %d", genocidios);
          printf("fitness: %d\n", fitness_melhor_individuo);
          criterio_parada = 0;
          break;
        }
        if(fitness_melhor_individuo == fitness_anterior)
          taxa_mutacao++;
        else if(fitness_melhor_individuo < fitness_anterior)
          taxa_mutacao = 0;
        fitness_anterior = fitness_melhor_individuo;
        if(taxa_mutacao == n){
          genocidios++;
          taxa_mutacao = 1;
          apaga_populacao(populacao);
          break;
        }
      }
    }
    free(tabuleiro);
    printf("\nFim");
    return 0;
}




/*
Gera vetor de fitness normalizado da população.
100 -> melhor
0   -> pior
*/
int* gera_vetor_fitness(int** population, int** tabuleiro, int n){
    int* fitness_population = (int*)calloc(MAX_POPULATION,sizeof(int));
    for(int i = 0; i < MAX_POPULATION; i++){
      fitness_population[i] = avalia_individuo(tabuleiro,population[i],n);
    }
    return fitness_population;
}



/*
Seleciona os indivíduos para nova população com base no método de torneio de 2.
retorna o index do melhor indivíduo.
*/
int recombina_individuos_torneio(int*** population, int n, int *fitness){
  int index_melhorIndividuo;
  int fitness_melhorIndividuo;
  for(int i = 0; i< MAX_POPULATION; i++)
      if(fitness[i] < fitness_melhorIndividuo){
        index_melhorIndividuo = i;
        fitness_melhorIndividuo = fitness[i];
      }
  int** new_population = inicializa_populacao(n);
  for(int i = 0; i < n; i++)
    new_population[0][i] = (*population)[index_melhorIndividuo][i];

  for(int i = 1; i < MAX_POPULATION; i++){
    int pai1 = escolhe_pai(fitness);
    int pai2 = escolhe_pai(fitness);
    int corte = rand()%n;
    for(int j = 0; j < corte; j++)
      new_population[i][j] = (*population)[pai1][j];
    for(int j = corte; j < n; j++)
      new_population[i][j] = (*population)[pai2][j];
    apaga_populacao(*population);
    *population = new_population;
    return 0;
  }


}
/*
Função que escolhe aleatoriamente 2 indivíduos e seleciona o melhor para ser pai.
*/
int escolhe_pai(int* fitness){
  int individuo1 = rand()%MAX_POPULATION;
  int individuo2 = rand()%MAX_POPULATION;
  float num = rand()%1000/1000.0;
  if(num < PROB_PAI)
    return fitness[individuo1]>fitness[individuo2]?individuo1:individuo2;
  else
    return fitness[individuo1]<fitness[individuo2]?individuo1:individuo2;
  
}
/*
Função que faz a recombinação dos indivíduos.
O método utilizado é pegar o melhor indivíduo e recombinar com toda a população.
*/
int recombina_individuos_elitismo(int** population, int n, int *fitness){
    int index_melhorIndividuo;
    int fitness_melhorIndividuo = 100000000;
    for(int i = 0; i< MAX_POPULATION; i++)
      if(fitness[i] < fitness_melhorIndividuo){
        index_melhorIndividuo = i;
        fitness_melhorIndividuo = fitness[i];
      }
    for(int i = 0; i < MAX_POPULATION; i++){
      int corte = rand()%n;
      for(int j = 0; j < n; j++){
          population[i][j] = population[index_melhorIndividuo][j];
      }
    }
    return index_melhorIndividuo;
}


/*
Seleciona e retorna o index do melhor indivíduo da população.
*/
int seleciona_melhor_individuo(int *fitness){
  int melhor_fitness = __INT32_MAX__;
  int index;
  for(int i = 0; i < MAX_POPULATION; i++)
    if(fitness[i] < melhor_fitness){
      index = i;
      melhor_fitness = fitness[i];
    }

  return index;
}


/*
Passa por todos os indivíduos e calcula a probabilidade de mutação.
Caso ocorra, sorteia um genoma entre 0 e n e soma ou subtrai 1 deste aleatoriamente.
*/
int mutacao(int **population, int n, int taxa_mutacao){
  for(int i = 0; i < MAX_POPULATION; i++){
      if((rand()%1000/1000.0) <= MUTATION_RATE){
        for(int j = 0; j < taxa_mutacao; j++){
          int genoma_mutado = rand()%n;
          if(population[i][genoma_mutado] == 0)
            population[i][genoma_mutado] +=1;
          else if(population[i][genoma_mutado] == n-1)
            population[i][genoma_mutado] -=1;
          else if(rand()%2)
            population[i][genoma_mutado] +=1;
          else
            population[i][genoma_mutado] -=1;
        }
      }
  }
  return 1;
}



/*
Inicializa uma população aleatória com indivíduos que possuem n genomas.
retorna a matriz de população
retorna null -> erro
*/
int** inicializa_populacao(int n){
    int **population = (int**)calloc(MAX_POPULATION,sizeof(int*));
    if(population == NULL)
      return NULL;
    for(int i = 0; i < MAX_POPULATION; i++){
      population[i] = (int*)calloc(n,sizeof(int));
        preenche_aleatorio(population[i], n);
    }
    return population;
}

/*
Apaga a populacao.
*/
void apaga_populacao(int** populacao){
    for(int i = 0; i < MAX_POPULATION; i++)
      free(populacao[i]);
    free(populacao);
}

/*
Preenche um vetor de n elementos com números aleatórios entre 0 e n
*/
void preenche_aleatorio(int* vetor, int n){
    for(int i = 0; i < n; i++){
        vetor[i] = rand()%n;
    }
}



/*
Calcula o fitness de um único indivíduo
retorna sua avaliação
*/
int avalia_individuo(int** tabuleiro,int* individuo, int n){
    int soma_fitness = 0;
    limpa_tabuleiro(tabuleiro, n);
    for(int i = 0; i < n; i++)
      insere_peca(tabuleiro,i,individuo[i],1);
    for(int i = 0; i < n; i++){
      soma_fitness += verifica_ataque(tabuleiro, n,i,individuo[i]);
      retira_peca(tabuleiro, i, individuo[i]);
    } 
    return soma_fitness;
}
/*
  Insere um indivíduo no tabuleiro.
*/
int insere_individuo(int* individuo, int** tabuleiro, int n){
  if(individuo == NULL)
    return 0;
  for(int i = 0; i < n; i++)
    tabuleiro[i][individuo[i]] = 1;
  return 1;
}



/*
Função que aloca o tabuleiro do jogo
retorna:
end. do tabuleiro -> SUCESSO
-1 -> ERRO
*/
int** cria_tabuleiro(int n){
    int **tabuleiro = calloc(n,sizeof(int*));
    if(tabuleiro == NULL)
        return NULL;
    for(int i = 0; i < n;i++){
        tabuleiro[i] = calloc(n,sizeof(int));
        if(tabuleiro[i] == NULL)
            return NULL;
    }
    return tabuleiro;
}


/*
Apaga o tabuleiro nxn
*/
void apaga_tabuleiro(int **tabuleiro, int n){
  for(int i = 0; i < n; i++)
    free(tabuleiro[i]);
  free(tabuleiro);
}


/*
Reinicializa o tabuleiro (seta todos os campos = 0)
*/
void limpa_tabuleiro(int** tabuleiro, int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      tabuleiro[i][j] = 0;
  }
}


/*
Insere uma peça no tabuleiro
Retorno:
-1 -> ERRO
 1 -> SUCESSO
*/
int insere_peca(int** tabuleiro, int posX, int posY, int tipo_peca){
    if(tabuleiro[posX][posY] != 0)
        return -1;
    tabuleiro[posX][posY] = tipo_peca;
    return 1;
}
/*
Remove uma peça do tabuleiro
-1 -> ERRO
 1 -> SUCESSO
*/


int retira_peca(int** tabuleiro, int posX, int posY){
    if(tabuleiro[posX][posY] == 0)
        return -1;
    tabuleiro[posX][posY] = 0;
    return 1;
}


/*
Verifica a quantidade de ataques que uma peça pode realizar.
retorna:
-1 -> ERRO
num ataques -> SUCESSO
*/
int verifica_ataque(int** tabuleiro, int n, int posX, int posY){
    int ataques = 0;
    if(posX >= n|| posY >= n)
        return -1;
    int tipo = tabuleiro[posX][posY];
    if(tipo == 1|| tipo == 2){
        for(int i = 0; i < n; i++){
            if(i == posY)
              continue;
            if(tabuleiro[posX][i] != 0){
                ataques++;
            }                
        }
        for(int i = 0; i < n; i++){
            if(i == posX)
                continue;
            if(tabuleiro[i][posY] != 0){
                ataques++;
            }                
        }
    }
    if(tipo == 1|| tipo == 3){
        for(int i = 1; (posX+i<n)&&(posY+i<n); i++){
            if(tabuleiro[posX+i][posY+i] != 0){
                ataques++;
            }                
        }
        for(int i = 1; (posX-i>=0)&&(posY-i>=0); i++){
            if(tabuleiro[posX-i][posY-i] != 0){
                ataques++;
            }                
        }
        for(int i = 1; (posX+i<n)&&(posY-i>=0); i++){
            if(tabuleiro[posX+i][posY-i] != 0){
                ataques++;
            }                
        }
        for(int i = 1; (posX-i>=0)&&(posY+i<n); i++){
            if(tabuleiro[posX-i][posY+i] != 0){
                ataques++;
            }                
        }
    }
    return ataques;
}


/*
Printa o tabuleiro na tela do console
*/
void printa_tabuleiro(int **tabuleiro, int  n){
    for(int i = 0; i < n; i++){
      for(int j = 0; j< n; j++)
        printf(" %d ", tabuleiro[i][j]);
      printf("\n");
    }

}
