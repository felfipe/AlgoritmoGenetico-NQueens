#include <stdio.h>
#include <stdlib.h>
int** cria_tabuleiro(int n);
int insere_peca(int** tabuleiro, int posX, int posY, int tipo_peca);
int verifica_ataque(int** tabuleiro, int n, int posX, int posY);
void apaga_tabuleiro(int **tabuleiro, int n);
void limpa_tabuleiro(int **tabuleiro, int n);
void printa_tabuleiro(int **tabuleiro, int n);
//Em tab 0 indica que não há peças no local e 1 indica que há peças

int main()
{
    int n; //tamanho do tabuleiro 
    int qnt; //quantidade de peças
    int *pointer; //transmite os vetores como paramêtros
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
-1 - ERRO
1 - SUCESSO
*/
int insere_peca(int** tabuleiro, int posX, int posY, int tipo_peca){
    if(tabuleiro[posX][posY] != 0)
        return 0;
    tabuleiro[posX][posY] = tipo_peca;
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
