/Inclusao das bibliotecas 
//------------------------------------------------------------------------------ 
#include "simu.hpp"
using namespace std;

//------------------------------------------------------------------------------ 
//Programa Principal 
//------------------------------------------------------------------------------ 
int main()
{
	//srand(1000); // pega o numero 1000 como semente de numeros aleatorios
	srand(time(NULL)); // pega a hora do relogio como semente
	n = 25;
	b = 100;
	maxIter = 100;

	s = cria_vetor(n);
	s_star = cria_vetor(n);
	w = cria_vetor(n);
	p = cria_vetor(n);
	q = cria_vetor(n);
	inicializa_vetor(w,n);
	inicializa_vetor(p,n);

	strcpy(nomeTXT, "mochila_peso25.txt");
	le_arq_vetor_denso(nomeTXT, w);
	strcpy(nomeTXT, "mochila_beneficio25.txt");
	le_arq_vetor_denso(nomeTXT, p);
	strcpy(nomeTXT, "mochila_qtd25.txt");
	le_arq_vetor_denso(nomeTXT, q);
	penalidade = calcula_penalidade(w, n, q);
	fo_star = - INT_MAX;

	do
	{
		choice = menu_principal();
		switch (choice)
		{
			case 1: /* Geracao de uma solucao inicial */
					/* Geracao aleatoria de uma solucao inicial */
						constroi_solucao_aleatoria(s, n);
						printf("Solucao aleatoria:\n");
						imprime_solucao(s,n);
						peso_objetos = calcula_peso_objetos(s, n, w);
						inv = calcula_inviabilidade(peso_objetos, b);
						printf("peso dos objetos = %2d inviabilidade = %d \n",peso_objetos, inv);
						fo = calcula_fo(s, n, p, inv, penalidade,q);
						printf("funcao objetivo = %3d \n",fo);
			break;
			case 2: /* Simulated Anealing */
				if (fo != 0)
				{
					alfa = 0.999;
					SAmax = n; 
					temperatura_inicial = calcula_temperatura_inicial(n,s,p,w,b,penalidade,&peso_objetos,&fo,alfa,SAmax);
					temperatura_final = 0.1;
					SA(n,s,p,w,b,penalidade,&peso_objetos,&fo,&inv,alfa,temperatura_inicial,temperatura_final,SAmax);
				}
				else
					printf("\n ----- Gere uma solucao inicial -------\n");
			break;

		}
	} while (choice != 0);

	libera_vetor(s);
	libera_vetor(s_star);
	libera_vetor(w);
	libera_vetor(p);
	libera_vetor(q);
	return 0;
}

void inicializa_vetor(int *vetor, int tam)
{
	for (int j=0; j<tam; j++)
		vetor[j] = 0;
}

//------------------------------------------------------------------------------ 
//cria memoria para um vetor inteiro
//------------------------------------------------------------------------------ 
int *cria_vetor(int tam)
{
	int *vetor;

	vetor = (int *) malloc(tam*sizeof(int));
	if (!vetor)
	{
		printf("Falta memoria para alocar o vetor de ponteiros");
		exit(1);
	}
	return vetor;
}

//------------------------------------------------------------------------------ 
//libera memoria de um vetor 
//------------------------------------------------------------------------------ 
void libera_vetor(int *vetor)
{
	free(vetor);
}

//------------------------------------------------------------------------------ 
//le um arquivo no formato i, valor 
//------------------------------------------------------------------------------ 
void le_arq_vetor(char nomearq[], int *vetor)
{
	int j, valor;
	FILE *arquivo;

	arquivo = fopen(nomearq,"r");
	if (!arquivo)
	{
		printf("O Arquivo %s nao pode ser aberto.\n", nomearq);
		exit(1);
	}
	while (!feof(arquivo))
	{
		fscanf(arquivo, "%d, %d", &j, &valor);
		vetor[j] = valor;
	}
	fclose(arquivo);
}

//------------------------------------------------------------------------------ 
//Nesta leitura, vetor[j] = valor para todo j=0...n 
//------------------------------------------------------------------------------ 
void le_arq_vetor_denso(char nomearq[], int *vetor)
{
	int j, valor;
	FILE *arquivo;

	arquivo = fopen(nomearq,"r");
	if (!arquivo)
	{
		printf("O Arquivo %s nao pode ser aberto.\n", nomearq);
		exit(1);
	}
	j = 0;
  	while (!feof(arquivo))
	{
		fscanf(arquivo, "%d", &valor);
		vetor[j] = valor;
		j++;
	}
	fclose(arquivo);
}

//------------------------------------------------------------------------------ 
//constroi uma solucao inicial aleatoria 
//------------------------------------------------------------------------------ 
void constroi_solucao_aleatoria(int *s, int num_objetos)
{
	for (int j=0; j<num_objetos; j++)
		s[j] = rand() % q[j]; 
}

//------------------------------------------------------------------------------ 
//imprime a solucao 
//------------------------------------------------------------------------------ 
void imprime_solucao(int *s, int num_objetos)
{
	for (int j=0; j<num_objetos; j++)
		printf("s[%2d]=%d \n",j+1,s[j]);
}
//------------------------------------------------------------------------------ 
//calcula o peso dos objetos na mochila 
//------------------------------------------------------------------------------ 
int calcula_peso_objetos(int *s, int num_objetos, int *w)
{
	int j, peso = 0;

	for (j=0; j<num_objetos; j++)
			peso += s[j]*w[j];
	
	return peso;
}
//------------------------------------------------------------------------------ 
//calcula o excesso de peso na mochila 
//------------------------------------------------------------------------------ 
int calcula_inviabilidade(int peso_objetos, int capacidade)
{
	return MAX(0, peso_objetos - capacidade);
}
//------------------------------------------------------------------------------ 
//calcula a penalidade por exceder a capacidade da mochila 
//------------------------------------------------------------------------------ 
int calcula_penalidade(int *w, int num_objetos, int *q)
{
	int penalidade = 0;
	for (int j=0; j<num_objetos; j++)
		penalidade += w[j];
	return penalidade;
}
//------------------------------------------------------------------------------ 
//insere uma quantidade na mochila 
//------------------------------------------------------------------------------ 
void troca_bit(int *s, int j)
{
	int eq=s[j];
	srand(time(NULL));
	while(s[j]==eq){
	s[j] = rand() % (q[j]+1);
	}
}
//------------------------------------------------------------------------------ 
//calcula a funcao objetivo 
//------------------------------------------------------------------------------ 
int calcula_fo(int *s, int num_objetos, int *p, int inviabilidade, int penalidade, int *q)
{
	int j, beneficio = 0;

	for (j=0; j<num_objetos; j++)
			beneficio += s[j]*p[j];
			
	return beneficio - penalidade * inviabilidade;
}
//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------ 
void SA(int n, int *s, int *p, int *w, int b, int penalidade, int *peso_objetos, int *fo, int *inv, double alfa, double temperatura_inicial, double temperatura_final, int SAmax)
{
	double x;              // numero aleatorio entre ZERO e UM
	double temperatura;    // temperatura corrente
	double tempo_corrido;
	int iter;              // numero de iteracoes na temperatura corrente
	int num_mudancas_temp; // numero de mudancas de temperatura
	int posicao_escolhida; // objeto escolhido
	int delta;             // variacao de energia
	int fo_star;
	int *s_star;

	s_star = cria_vetor(n);
	inicializa_vetor(s_star,n);
	atualiza_melhor_solucao(s, s_star, n);
	fo_star = *fo;
	temperatura = temperatura_inicial;
	num_mudancas_temp = 0;
	time_t t_ini, t_fim;
	time(&t_ini);
	time(&t_fim);
	tempo_corrido= difftime(t_fim,t_ini);
	printf("Solucao inicial: fo corrente = %d   peso dos objetos = %2d \n",*fo,*peso_objetos);

	/* enquanto o sistema nao congelar faca */
	while (temperatura > temperatura_final)
	{
		iter = 0;
		/* enquanto o equilibrio termico nao for atingido faca */
		while (iter < SAmax)
		{
			iter++;
			/* escolha um vizinho qualquer */
			posicao_escolhida = rand()%(n);

			int qtd_anterior=s[posicao_escolhida];
			troca_bit(s, posicao_escolhida);
			if (s[posicao_escolhida])
				printf("Vou tentar incluir na mochila o objeto %d de peso %d \n", posicao_escolhida, w[posicao_escolhida]);
			else
				printf("Vou tentar excluir da mochila o objeto %d de peso %d \n", posicao_escolhida, w[posicao_escolhida]);
			/* calcule a variacao de energia */
			int peso_modificado= (*peso_objetos) +w[posicao_escolhida] *(s[posicao_escolhida]-qtd_anterior);
			delta = p[posicao_escolhida]*(s[posicao_escolhida]-qtd_anterior) + penalidade * calcula_inviabilidade((*peso_objetos), b)  -  penalidade * calcula_inviabilidade(peso_modificado,b);
			printf("variacao de energia = %3d \n",delta);

			/* se houver melhora, aceite o vizinho */
			if (delta > 0)
			{
				(*fo) += delta;
				(*peso_objetos) = (*peso_objetos) + (w[posicao_escolhida])*(s[posicao_escolhida]-qtd_anterior);
				printf("Solucao de melhora: fo corrente = %d   peso dos objetos = %2d \n",*fo,*peso_objetos);

				if (*fo > fo_star)
				{
					atualiza_melhor_solucao(s, s_star, n);
					fo_star = *fo;
					imprime_solucao(s_star,n);
					printf("********** fo_star = %3d \n",fo_star);
				}
			}
			else
			{
			/* se houver piora, aceite o vizinho com uma determinada probabilidade */
				x = randomico(0,1);
				if (x < exp(delta/temperatura))
				{
					(*fo) += delta;
					(*peso_objetos) = (*peso_objetos) + (w[posicao_escolhida])*(s[posicao_escolhida]-qtd_anterior);
					printf("Solucao de piora: fo corrente = %d   peso dos objetos = %2d \n",*fo,*peso_objetos);
				}
				else
				{
				/* Se o vizinho nao foi aceito, desfaca o movimento */
					s[posicao_escolhida] = qtd_anterior;
				}
			}
		}
		/* decresca a temperatura */
		temperatura *= alfa;
		printf("Temperatura atual = %10.3f\n", temperatura);
		num_mudancas_temp += 1;
	}

	time(&t_fim);
	tempo_corrido= difftime(t_fim,t_ini);
	printf("********* Melhor Solucao obtida pelo SA ************** \n");
	imprime_solucao(s_star,n);
	(*peso_objetos) = calcula_peso_objetos(s_star, n, w);
	(*inv) = calcula_inviabilidade(*peso_objetos, b);
	printf("peso dos objetos = %2d inviabilidade = %d \n",*peso_objetos, *inv);
	printf("funcao objetivo = %3d \n",fo_star);
	printf("Tempo decorrido: %0.f segundos:\n", tempo_corrido);
	printf("Num. max. de solucoes analisadas = %d \n",num_mudancas_temp*SAmax);
	printf("Numero de solucoes existentes = %.0f \n", exp(n*log(2)));
	printf("****************************************************** \n");
	atualiza_melhor_solucao(s_star, s, n);
	(*fo) = fo_star;
	libera_vetor(s_star);
}
//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------ 
double calcula_temperatura_inicial(int n, int *s, int *p, int *w, int b, int penalidade, int *peso_objetos, int *fo, double alfa, int SAmax)
{
	double x;              // numero aleatorio entre ZERO e UM
	double temperatura;    // temperatura corrente
	int iter;              // numero de iteracoes na temperatura corrente
	int posicao_escolhida; // objeto escolhido
	int delta;             // variacao de energia
	int aceitos, min_aceitos;

	temperatura = 30;
	printf("Solucao inicial: fo corrente = %d   peso dos objetos = %2d \n", *fo,*peso_objetos);

	aceitos = 0;
	min_aceitos = (int) (0.90*SAmax);
	while (aceitos < min_aceitos)
	{
		iter = 0;
		printf("temperatura inicial = %f \n",temperatura);
		while (iter < SAmax)
		{
			iter++;
			/* escolha um vizinho qualquer */
			posicao_escolhida = rand()%(n);
			int qtd_anterior=s[posicao_escolhida];
			troca_bit(s, posicao_escolhida);
			/* calcule a variacao de energia */
			int peso_modificado = (*peso_objetos)  + w[posicao_escolhida]*(s[posicao_escolhida]-qtd_anterior);
			delta = penalidade * calcula_inviabilidade((*peso_objetos), b) + p[posicao_escolhida]*(s[posicao_escolhida]-qtd_anterior) -  penalidade * calcula_inviabilidade(peso_modificado,b);

			/* se houver melhora, aceite o vizinho */
			if (delta > 0)
				aceitos++;
			else
			{
			/* se houver piora, aceite o vizinho com uma determinada probabilidade */
				x = randomico(0,1);
				if (x < exp(delta/temperatura))
					aceitos++;
			}
			s[posicao_escolhida]=qtd_anterior;
		}
		printf("aceitos = %d   min_aceitos = %d\n",aceitos, min_aceitos);
		if (aceitos < min_aceitos)
		{
			aceitos = 0;
			temperatura = temperatura * 1.1;
		}
	}
	printf("temperatura inicial = %f \n",temperatura);
	return temperatura;
}
//------------------------------------------------------------------------------ 
//Gera numero aleatorio entre min e max 
//------------------------------------------------------------------------------ 
double randomico(double min, double max)
{
	if (min == max)
		return min;
	return ((double) (rand()%10000/10000.0)*(max-min) + min);
}
//------------------------------------------------------------------------------ 
//atualiza a melhor solucao 
//------------------------------------------------------------------------------ 
void atualiza_melhor_solucao(int *s, int *s_star, int num_objetos)
{
	for (int j=0; j<num_objetos; j++)
		s_star[j] = s[j];
}
//------------------------------------------------------------------------------ 
//Menu principal
//------------------------------------------------------------------------------ 
int menu_principal(void)
{
	int escolha;

	do
	{
		printf("\n************************************************* \n");
		printf("         1. Gere solucao inicial \n");
		printf("         2. Simulated Anealing \n");
		printf("         0. Sair \n");
		printf("         Escolha: ");
		scanf("%d",&escolha);
	} while (escolha < 0 || escolha > 2);
	return escolha;
}
