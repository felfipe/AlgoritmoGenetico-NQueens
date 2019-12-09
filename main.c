#include <stdio.h>
#include <stdlib.h>
int** cria_tabuleiro(int n);
int insere_peca(int** tabuleiro, int posX, int posY, int tipo_peca);
int retira_peca(int** tabuleiro, int posX, int posY);
int verifica_ataque(int** tabuleiro, int n, int posX, int posY);
void apaga_tabuleiro(int **tabuleiro, int n);
void limpa_tabuleiro(int **tabuleiro, int n);
void printa_tabuleiro(int **tabuleiro, int n);
int** inicializa_populacao(int n);
void preenche_aleatorio(int* vetor, int n);
int avalia_individuo(int **tabuleiroint, int* individuo, int n);
/**** funnções do AG ****/
#define MAX_POPULATION 20
#define MUTATION_RATE  0.01


int main()
{
    srand(time(NULL));
    int n; //tamanho do tabuleiro 
    int qnt; //quantidade de peças
    printf("Digite a dimensao n desejada para o tabuleiro: ");
    scanf("%d", &n);
    int **tabuleiro = cria_tabuleiro(n);
    if(tabuleiro == NULL){
        printf("\nErro, tabuleiro não alocado.");
        return 0;
    }
    limpa_tabuleiro(tabuleiro, n);
    for(int i = 0; i < n; i++){
      int posX, posY;
      scanf("%d %d", &posX, &posY);
      insere_peca(tabuleiro,posX,posY,1);
    }
    printa_tabuleiro(tabuleiro, n);
    int ataque = verifica_ataque(tabuleiro, n, 0, 1);
    printf("\nataque: %d",ataque);
    free(tabuleiro);
    printf("\nFim");
    return 0;
}
/*
Gera vetor de fitness normalizado da população.
100 -> melhor
0   -> pior
*/
double* gera_vetor_fitness(int** population, int** tabuleiro, int n){
    int aux[n];
    int soma_fitness = 0;
    for(int i = 0; i < MAX_POPULATION;i++){
      soma_fitness+= avalia_individuo(tabuleiro,population[i],n);
    }
    double* fitness_population = (double*)calloc(n,sizeof(double));
    for(int i = 0; i < MAX_POPULATION; i++){
      fitness_population[i] = 1-avalia_individuo(tabuleiro,population[i],n)/soma_fitness;
    }
    return fitness_population;
}

int





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
      if(population[i] == NULL)
        return NULL;
        preenche_aleatorio(population[i], n);
    }
    return population;
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
      soma_fitness = verifica_ataque(tabuleiro, n,i,individuo[i]);
      retira_peca(tabuleiro, i, individuo[i]);
    }
    return soma_fitness;
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
                break;
            }                
        }
        for(int i = 0; i < n; i++){
            if(i == posX)
                continue;
            if(tabuleiro[i][posY] != 0){
                ataques++;
                break;
            }                
        }
    }
    if(tipo == 1|| tipo == 3){
        for(int i = 1; (posX+i<n)&&(posY+i<n); i++){
            if(tabuleiro[posX+i][posY+i] != 0){
                ataques++;
                break;
            }                
        }
        for(int i = 1; (posX-i>=0)&&(posY-i>=0); i--){
            if(tabuleiro[posX-i][posY-i] != 0){
                ataques++;
                break;
            }                
        }
        for(int i = 1; (posX+i<n)&&(posY-i>=0); i++){
            if(tabuleiro[posX+i][posY-i] != 0){
                ataques++;
                break;
            }                
        }
        for(int i = 1; (posX-i>=0)&&(posY+i<n); i++){
            if(tabuleiro[posX-i][posY+i] != 0){
                ataques++;
                break;
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
