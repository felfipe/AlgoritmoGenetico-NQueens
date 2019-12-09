#include <stdio.h>

void pecas(); //print da tabela de peças
void receberPecas(int *pontVec, int qnt);

void atribuirPos(int *matriz); 

//void dama(int posX, int posY, int n, int *tab);
void lateral(int posX, int posY, int n, int *tab);
void diagonal(int posX, int posY, int n, int *tab);
//Em tab 0 indica que não há peças no local e 1 indica que há peças

int main()
{
    int n; //tamanho do tabuleiro 
    int qnt; //quantidade de peças
    int *pointer; //transmite os vetores como paramêtros

    printf("Sizeof(int)= %d\n", sizeof(int));
    printf("Digite a dimensao n desejada para o tabuleiro: ");
    scanf("%d", &n);
    printf("Digite a quantidade de peças que deseja no tabuleiro: ");
    scanf("%d", &qnt);

    int *tabuleiro = (int *)malloc(n * n * sizeof(int));
    //tabuleiro NxN, n é entrada
    int *vetorPec = (int *)malloc(qnt * 3 * sizeof(int)); 
    //matriz com a identificação da peça e posição dela no tabuleiro
    
    
    printf("Digite quais pecas deseja no tabuleiro %dx%d\nsao %d pecas\n", n, n,qnt);
    pecas(); //printa a tabela de referencia de pecas
    receberPecas(vetorPec, qnt); //função para definir as peças

    printf("\n\n");
    //printf("Gerando posições iniciais...");
    int j;
    for(j=0; j<qnt; j++){
      printf("%d, ", *(vetorPec+4*3*j));
    }

    //dama(3,4,n, tabuleiro);


    free(tabuleiro);
    free(vetorPec);
    printf("\nFim");
    return 0;
}

void pecas(){ //print da tabela de peças

  printf("1. Dama\n2. Torre\n3. Bispo\n");

}

void receberPecas(int *pontVec, int qnt){

    int i, var;
    printf("%d\n", pontVec);
    for(i=0; i<qnt; i++){ //fazer percorrer o sempre na primeira linha
      scanf("%d", &var);//adiciona a peça
      if(var>0 && var<4){ //de 1 a 3
        *(pontVec+sizeof(int)*3*i) = var;  //4bytes, 3 colunas
      }
      else {
        *(pontVec+sizeof(int)*3*i) = 1; //por default se alguem digitar errado é dama
      }
      printf("Peca %d = %d\n\n", i+1, *(pontVec+sizeof(int)*3*i));
      printf("Endereço = %d\n\n", (pontVec+sizeof(int)*3*i));
    }

}

void atribuirPos(int *matriz){

}

//*******FUNÇÕES ANDAR LATERAL E DIAGONAL, NO CASE DA DAMA CHAMAR OS 2*******//

/*
void dama(int posX, int posY, int n, int *tab){
    int x,y;
    int cont;

    for(x=0; x<n; x++){ 
      //movimento na coluna
        if(*(tab+sizeof(int)*(n*x + posY)) == 1){ //Se tiver uma peça
          *(tab+sizeof(int)*(n*x + posY)) = 0;    //a peça é comida 
          //Só some no tabuleiro
          //no vetor de peças se mantém
        }
    }
    for(y=0; y<n; y++){ 
      //movimento na linha
        if(*(tab+sizeof(int)*(n*posX + y)) == 1){
          *(tab+sizeof(int)*(n*posX + y)) = 0;  
      }
    }

    
    for(cont=0; posX+cont<n && posY+cont<n; cont++){
      //"primeiro quadrante"
        if(*(tab+sizeof(int)*(n*(posX+cont)+(posY+cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX+cont)+(posY+cont))) = 0;
        }
    }
    for(cont=0 ; posX-cont>=0 && posY+cont<n; cont++){
      //desce em x e sobe em y
      //"Segundo quadrante"
        if(*(tab+sizeof(int)*(n*(posX-cont)+(posY+cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX-cont)+(posY+cont)))=0;
        }
    }
    for(cont=0 ; posX-cont>=0 && posY-cont>=0; cont++){
      //"Terceiro quadrante"
        if(*(tab+sizeof(int)*(n*(posX-cont)+(posY-cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX-cont)+(posY-cont)))=0;
        }
    }
    for(cont=0 ; posX+cont<n && posY-cont>=0; cont++){ 
      //sobe em x e desce em y
      //"Quarto quadrante"
        if(*(tab+sizeof(int)*(n*(posX+cont)+(posY-cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX+cont)+(posY-cont)))=0;
        }
    }

}
*/



void lateral(int posX, int posY, int n, int *tab){
    int x,y;
    int cont;

    for(x=0; x<n; x++){ 
      //movimento na coluna
        if(*(tab+sizeof(int)*(n*x + posY)) == 1){ //Se tiver uma peça
          *(tab+sizeof(int)*(n*x + posY)) = 0;    //a peça é comida 
          //Só some no tabuleiro
          //no vetor de peças se mantém
        }
    }
    for(y=0; y<n; y++){ 
      //movimento na linha
        if(*(tab+sizeof(int)*(n*posX + y)) == 1){
          *(tab+sizeof(int)*(n*posX + y)) = 0;  
      }
    }
}
void diagonal(int posX, int posY, int n, int *tab){
    int x,y, cont;

    for(cont=0; posX+cont<n && posY+cont<n; cont++){
      //"primeiro quadrante"
        if(*(tab+sizeof(int)*(n*(posX+cont)+(posY+cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX+cont)+(posY+cont))) = 0;
        }
    }
    for(cont=0 ; posX-cont>=0 && posY+cont<n; cont++){
      //desce em x e sobe em y
      //"Segundo quadrante"
        if(*(tab+sizeof(int)*(n*(posX-cont)+(posY+cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX-cont)+(posY+cont)))=0;
        }
    }
    for(cont=0 ; posX-cont>=0 && posY-cont>=0; cont++){
      //"Terceiro quadrante"
        if(*(tab+sizeof(int)*(n*(posX-cont)+(posY-cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX-cont)+(posY-cont)))=0;
        }
    }
    for(cont=0 ; posX+cont<n && posY-cont>=0; cont++){ 
      //sobe em x e desce em y
      //"Quarto quadrante"
        if(*(tab+sizeof(int)*(n*(posX+cont)+(posY-cont))) == 1 ){
          *(tab+sizeof(int)*(n*(posX+cont)+(posY-cont)))=0;
        }
    }
}