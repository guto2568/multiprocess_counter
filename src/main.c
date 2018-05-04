/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <unistd.h> /* fork() */
#include <stdio.h> /*printf() e scanf()*/
#include <sys/mman.h> /*mmap()*/

#define TRUE 1
#define FALSE 0
#define N_FILHOS 4/*Numero de filhos permitido*/

typedef unsigned long long ll;

/*Verifica se um numero eh primo*/
/*Retorna 1 caso seja primo, e 0 caso nao seja*/
int primo(ll num){
	ll i;

	if(num == 2)
		return TRUE;

	if(num == 1 || num%2 == 0)
		return FALSE;

	for(i = 3; i*i <= num; i+=2){
		if(num%i == 0)
			return FALSE;
	}

	return TRUE;
}

int main() {
	/*Endereços dos processos filhos*/
	pid_t pid[N_FILHOS];
	
	/*Endereco para alocar contador compartilhado*/
	ll *contador;

	/*Vetor de entrada entrada*/
	ll entrada[200];
	ll tam_entrada = 0;

	int filho_atual = 0;

	int i = 0;
	int j = 0;

	/* Definir flags de protecao e visibilidade de memoria */
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	/* Criar area de memoria compartilhada */
	contador = (ll*) mmap(NULL, sizeof(ll), protection, visibility, 0, 0);


	/*Verifica se eh a primeira vez que estamos passando pelo filho*/
	int primeira_passada = 1;

	/*Le numeros*/
	while(scanf("%llu", &entrada[tam_entrada]) == 1) 
		tam_entrada++;
	
	/*Faz cada filho verificar numeros correspondentes a ele*/
	for(i = 0; i < N_FILHOS; i++){
		pid[i] = fork();
		/*Filho verifica primalidade de todos os numeros pelo qual eh responsavel*/
		if(pid[i] == 0){
			for(j = i; j < tam_entrada; j += N_FILHOS)
				if(primo(entrada[j]))
					(*contador)++;
			exit(0);
		}
	}

	/*Espera todos os filhos terminarem*/
	for(i = 0; i < N_FILHOS; i++)
		waitpid(pid[i], NULL, 0);

	printf("%llu\n", *contador);

 	return 0;
}
