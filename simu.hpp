/------------------------------------------------------------------------------ 
//Inclusao das bibliotecas 
//------------------------------------------------------------------------------ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>

//------------------------------------------------------------------------------ 
//Declaracao das Constantes
//------------------------------------------------------------------------------ 
#define MAX(x,y) ((x)<(y) ? (y) : (x))

//------------------------------------------------------------------------------ 
//Declaracao das Variaveis Globais
//------------------------------------------------------------------------------ 
struct lista
{
	int posicao;
  	struct lista *proximo;  /* ponteiro para a próxima entrada */
  	struct lista *anterior; /* ponteiro para o registro anterior */
};

struct lista_restrita
{
	int objeto;
        double beneficio_relativo;
        int fo;
  	struct lista_restrita *proximo;  /* ponteiro para a próxima entrada */
  	struct lista_restrita *anterior; /* ponteiro para o registro anterior */
};

int n;           // numero de objetos
int b;           // capacidade da mochila
int *s;          // vetor solucao corrente
int *s_star;     // vetor melhor solucao
int *w;          // vetor de peso de cada objeto
int *p;          // vetor de beneficio de cada objeto
int *q;
int fo;          // funcao objetivo corrente
int fo_star;     // melhor funcao objetivo
int inv;         // quantidade de inviabilidade da solucao corrente
int penalidade;  // penalidade por superar a capacidade da mochila
int peso_objetos;// peso dos objetos na solucao corrente
int choice;

int maxIter;//Número máximo de iterações para procurar um  melhor vizinho

int delta;                 // variacao de energia
double temperatura_inicial;// temperatura inicial de partida do sistema
double temperatura_final;  // temperatura de congelamento do sistema
double alfa;               // taxa de resfriamento
int SAmax;                 // numero maximo de iteracoes em uma dada temperatura



struct lista_restrita *inicio_LC, *final_LC, *candidato;
int tamanho_LC;     // tamanho da lista de candidatos
int tamanho_LRC;    // tamanho da lista restrita de candidatos



char nomeTXT[50];

//------------------------------------------------------------------------------ 
//Declaracao das Funcoes
//------------------------------------------------------------------------------ 
int *cria_vetor(int tam);
double *cria_vetor_double(int tam);
int **cria_matriz(int nlinhas, int ncolunas);
void libera_vetor(int *vetor);
void libera_matriz(int **matriz, int nlinhas);
void le_arq_vetor(char nomearq[], int *vetor);
void le_arq_vetor_denso(char nomearq[], int *vetor);

void constroi_solucao_aleatoria(int *s, int num_objetos);
void imprime_solucao(int *s, int num_objetos);
int calcula_fo(int *s, int num_objetos, int *p, int inviabilidade, int penalidade, int *q);
void troca_bit(int *s, int j);
int calcula_peso_objetos(int *s, int num_objetos, int *w);
int calcula_inviabilidade(int peso_objetos, int capacidade);
double randomico(double min, double max);
void atualiza_melhor_solucao(int *s, int *s_star, int num_objetos);
int calcula_penalidade(int *w, int num_objetos, int *q);
void imprime_solucao(int *s, int num_objetos);
void inicializa_vetor(int *vetor, int tam);

void imprime_fo(char nomearq[], float tempo, int fo, int iteracao);
void limpa_arquivo(char nomearq[]);

void SA(int n, int *s, int *p, int *w, int b, int penalidade, int *peso_objetos, int *fo, int *inv, double alfa, double temperatura_inicial, double temperatura_final, int SAmax);
void repara_inviabilidade(int n, int *s, int *p, int *w, int b, int penalidade, int *peso_objetos, int *fo, int *inv);

int menu_principal(void);

void embaralha_vetor(int *vetor, int n);

double calcula_temperatura_inicial(int n, int *s, int *p, int *w, int b, int penalidade, int *peso_objetos, int *fo, double alfa, int SAmax);
