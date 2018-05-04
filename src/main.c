
/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <sys/wait.h> /* wait process*/
#include <sys/mman.h> /* memory map*/
#include <unistd.h> /* fork() */

int primo(unsigned long int n);
void working(int indice, unsigned long int sera_primo,int *compartilhado);

int main(){
    pid_t pid[64];
    unsigned long int entrada[64];
    int tam, i = 0, instancia = 0, j = 0,*situacao_entrada = NULL, soma = 0;
    char carac;


    
    // Lendo String e vendo o tamanho dela 
    i = 0;
    tam = 0;
    while( scanf("%ld", &entrada[i]) == 1 ){
        i++;
        if((entrada[i]!=' ')||(entrada[i]!= '\n')){
            tam++;
        }
    }
        /* Area de memoria compartilhada */
    situacao_entrada = (int*) mmap(NULL, sizeof(int)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
    // Zera todas as posicoes da memoria situacao entrada
    for(i = 0; i<tam; i++){
        situacao_entrada[i] = 0;
    }

    i = 0;
    

    /* Conta os primos em processos paralelos */
    for(int k = 0; (k < 4) && (i<tam); k++){
        pid[k] = fork();
        //Processo filho
        if (pid[k] == 0){
            working(i,entrada[i],situacao_entrada);
        }
        else
            i++;
    }
    

    // apos acabar a primeira
    for(; i<tam; i++){
        int status;
        wait(&status);
        pid[i] = fork();
        if (pid[i] == 0){
            working(i,entrada[i],situacao_entrada);
        }
    }
    

    // Finalizando todo processo
    for(i = 0; i < 4; i++){
        int status2;
        wait(&status2);
    }
    for(i = 0; i< tam; i++){
        soma = soma + situacao_entrada[i];
    }
    printf("%d\n",soma);
    return 0;
}
//Esta funcao verifica se um dado numero inteiro positivo n é primo 
    int primo(unsigned long int n){
    unsigned long int d,Primo;			
     if (n <= 1 || (n != 2 && n % 2 == 0)){
            Primo = 0;    /* nenhum numero inteiro <= 1 ou par > 2 e' primo */
     }
     else
            Primo = 1;		/* o numero e' primo ate que se prove o contrario */

     d = 3;
     while (Primo  && d <= n / 2) {
        if (n % d == 0)
            Primo = 0;

                d = d + 2;		/* testamos so' os  impares: 3, 5, 7... */
    }
    if (Primo)
        return 1;
    else 
        return 0;
}
// Funcao que trabalha pelo filho
void working(int indice, unsigned long int sera_primo,int *compartilhado){
    int i;
    i = primo(sera_primo);
    if(i){
        compartilhado[indice] = 1;
    }
    exit(0);
}
