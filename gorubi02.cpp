//------------------------------------------------------------------------------ 
//Inclusao das bibliotecas 
//------------------------------------------------------------------------------ 
#include "gurobi02.hpp"

//------------------------------------------------------------------------------ 
//Programa principal 
//------------------------------------------------------------------------------ 
int main()
{
	int escolha;
	do{
		escolha=menu();
		switch (escolha)
		{
			case 1:
				exemplo01();
			break;
			case 2:
				mochila_binaria();
			break;
			case 3:
				vsbinpacking();
			break;
			case 4:
				corte01();
			break;	
			case 5:
				corte02();
			break;
			case 6:
				nao_linear_01();
			break;	
			case 7:
				nao_linear_02();
			break;	
			case 8:
				nao_linear_03();
			break;
			case 9:
				lerarquivo05();
				gerarcolunasppv();
				gurobippv();
			break;
			case 10:
				linear_01();
			break;	
		}		
	}while(escolha);
	return 0;
}

//------------------------------------------------------------------------------ 
//menu: menu de escolha
//------------------------------------------------------------------------------ 
int menu()
{
	int choice;
	do{
		cout<<"\n-------------------\n";
		cout<<"1- Exemplo 01\n";
		cout<<"2- Problema da Mochila Binaria\n";
		cout<<"3- Problema de Bin-packing com frota heterogenea\n";
		cout<<"4- Problema de Corte Unidimensional - Modelo de Kantorovich\n";
		cout<<"5- Problema de Corte Unidimensional - Modelo de Gilmore/Gomory\n";
		cout<<"6- Exemplo 01 Programacao Nao Linear\n";
		cout<<"7- Exemplo 02 Programacao Nao Linear\n";
		cout<<"8- Exemplo 03 Programacao Nao Linear\n";
		cout<<"9- Problema de Programacao de Veiculos - PPV\n";
		cout<<"10- Exemplo 08 Programacao Linear\n";
		cout<<"0- Sair.\n";
		cout<<"-------------------\n";
		cin>>choice;
	}while(choice<0||choice>10);	
	return choice;
}

//------------------------------------------------------------------------------ 
// 	[FO]max= 3*x1+3*x2; funcao objetivo do problema
//	2*x1+4*x2<=12;  resticao
//	6*x1+4*x2<=24; restricao
//	x1>=0; restricao
//	x2>=0; restricao
//	x1, x2: inteiros;
//------------------------------------------------------------------------------ 
void exemplo01()
{
	try
	{
		GRBEnv env = GRBEnv(); //sempre usar

		GRBModel model = GRBModel(env); //sempre usar

		//Criando as variaveis
		GRBVar x1 = model.addVar(0, 1000, 0, GRB_INTEGER, "x1");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x2 = model.addVar(0, 1000, 0, GRB_INTEGER, "x2");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER

		//Adicionando novas variaveis
		model.update();

		//Funcao Objetivo: max= 3*x1+3*x2
		model.setObjective(3*x1 + 3*x2, GRB_MAXIMIZE);// sempre utiliza para a FO

		//primeira restricao: 2*x1+4*x2<=12
		model.addConstr(2*x1 + 4*x2 <= 12, "r1");

 		//segunda restricao: 6*x1+4*x2<=24
		model.addConstr(6*x1 + 4*x2 <= 24, "r2");

		//terceira restricao: x2<=1
		//model.addConstr(x2 <= 1, "r3");

		//quarta restricao: x1<=3
		//model.addConstr(x1 <= 3, "r4");

		//Resolvendo o modelo
 		model.optimize();

		cout << x1.get(GRB_StringAttr_VarName) << " = "<< x1.get(GRB_DoubleAttr_X) << endl;
		cout << x2.get(GRB_StringAttr_VarName) << " = "<< x2.get(GRB_DoubleAttr_X) << endl;
		
		cout << "Obj = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e)
	{
		cout << "Erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...)
	{
		cout << "Erro durante a otimizacao" << endl;
	}
}

//--------------------------------------------------------------------------
//funcao leraquivo01: leitura do arquivo de entrada gurobi02_entrada01.txt
//--------------------------------------------------------------------------
void lerarquivo01()
{
	txt= fopen("gurobi02_entrada01.txt", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada01.txt nao pode ser aberto.\n");
	erro= fscanf(txt, "p_j=");
	for(int j=0; j<8; j++) 
	{ 
		erro= fscanf(txt, "\t%d", &p[j]); 
		if (erro!=1) 
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada01.txt.\n"); 
	}
	erro= fscanf(txt, "\na_j=");
	for(int j=0; j<8; j++) 
	{ 
		erro= fscanf(txt, "\t%d", &a[j]); 
		if (erro!=1) 
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada01.txt.\n"); 
	}
	printf( "\nArquivo gurobi02_entrada01.txt lido com sucesso.\n"); 
	fclose(txt);
}

//------------------------------------------------------------------------------ 
// 	max sum from j=1 to n pj* xj 
//	sum from j=1 to n aj*xj <= b
//	x binario 
//------------------------------------------------------------------------------ 
void mochila_binaria()
{
	int resposta[8];

	//--------------------------------------------------------------------------
	//leitura do arquivo de entrada
	//--------------------------------------------------------------------------
	lerarquivo01();

	//--------------------------------------------------------------------------
	//GUROBI
	//--------------------------------------------------------------------------
	time(&t_ini);
	GRBEnv* env = 0;
	GRBVar* x = 0;
	
	try
	{
		printf("\n--------------------------------\n\n");
		printf("\nIniciando resolucao do problema.\n\n");
		env = new GRBEnv(); //inicia ambiente gurobi
		GRBModel model = GRBModel(*env);//cria um modelo
		model.set(GRB_StringAttr_ModelName, "Mochila_0-1");
		
		//--------------------------------------------------------------------------
		//Dados de entrada
		//--------------------------------------------------------------------------
		x = model.addVars(8, GRB_BINARY);
		model.update();
		for (int j=0; j<8; j++)
		{
   			x[j].set(GRB_DoubleAttr_Obj, p[j]);
			x[j].set(GRB_StringAttr_VarName, "x");
		}
		
		
		//--------------------------------------------------------------
		//funcao objetivo
		//--------------------------------------------------------------
		model.set(GRB_IntAttr_ModelSense, 0);	//0 para maximizar ou 1 para minimizar
		model.update();				//atualiza o modelo
		
		//--------------------------------------------------------------
		//restricao 1
		//--------------------------------------------------------------
		GRBLinExpr r1= 0;
		for (int j=0; j<8; j++)
			r1+= a[j]*x[j];
		model.addConstr(r1<=100, "r1");

		//--------------------------------------------------------------
		//inicia a resolucao do modelo
		//--------------------------------------------------------------
		model.update();
		model.optimize();
		
		//--------------------------------------------------------------
		//Exporta a solucao
		//--------------------------------------------------------------
		FO= model.get(GRB_DoubleAttr_ObjVal);
		printf("\nFO= %d\n", FO);
		for(int j=0; j<8; j++)
		{
			resposta[j]= x[j].get(GRB_DoubleAttr_X);
			printf("x[%d]= %d\n", j+1, resposta[j]);
		}
		printf("\n");
	}catch(GRBException e)
	{
		cout << "Gurobi - Codigo do erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}catch(...)
	{
		cout << "Gurobi - Erro durante otimizacao" << endl;
	}
	delete[] x;
	delete env;//termina ambiente gurobi*/
	
	time(&t_fim);
	tempo= difftime(t_fim,t_ini);
	printf("O problema foi resolvido em %.3lf segundos.\n", tempo);
	printf("\n--------------------------------\n\n");
}

//--------------------------------------------------------------------------
//funcao leraquivo02: leitura do arquivo de entrada gurobi02_entrada02.txt
//--------------------------------------------------------------------------
void lerarquivo02()
{
	int cont= 0;
	txt= fopen("gurobi02_entrada02.txt", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada02.txt nao pode ser aberto.\n"); 
	erro = fscanf(txt, "%d\t%d\n", &nu_item, &nu_bin);
	if (erro!=2)
		printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada02.txt.\n");
	for (int i=0; i<(nu_bin*nu_item); i++)
	{
		erro = fscanf(txt, "%f\t%f\t", &bin[i].capacidade, &bin[i].custo);
		if (erro!=2)
			printf("\n\nErro na leitura dos dados do gurobi02_entrada02.txt.\n");
		for (int j=i; j< (i+nu_item); j++)
		{
			bin[j].capacidade= bin[i].capacidade;
			bin[j].custo= bin[i].custo;
		}
		i+= nu_item-1;
	}
	erro = fscanf(txt, "\n");
	for (int i=0; i<nu_item; i++)
	{
		erro = fscanf(txt, "%f\n", &item[i].tamanho);
		if (erro!=1)
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada02.txt.\n");
	}
	printf( "\nArquivo gurobi02_entrada02.txt lido com sucesso.\n"); 
	fclose(txt);
}

//------------------------------------------------------------------------------ 
//Funcao variable sized binpacking: modelo matematico para o problema de bin-packing da P1 de POII 2012-2
//	min sum from i=1 to n y_i*c_i 
//	sum from j=1 to n x_{ij} =1, forall i
//	sum from i=1 to n w_i*x_{ij} <= b_i*y_i, forall j
//	x in 0,1
//------------------------------------------------------------------------------ 
void vsbinpacking()
{
	int respostay[N_ITEM*N_BIN], respostax[N_ITEM][N_ITEM*N_BIN];
	//--------------------------------------------------------------------------
	//leitura do arquivo de entrada
	//--------------------------------------------------------------------------
	lerarquivo02();

	//--------------------------------------------------------------------------
	//GUROBI
	//--------------------------------------------------------------------------
	time(&t_ini);
	GRBEnv* env = 0; 
	GRBVar* y = 0;
	GRBVar** x = 0;
	
	try
	{
		printf("\n--------------------------------\n\n");
		printf("\nIniciando resolucao do problema.\n\n");
		env = new GRBEnv(); //inicia ambiente gurobi
		GRBModel model = GRBModel(*env);//cria um modelo
		model.set(GRB_StringAttr_ModelName, "Bin-packing");
		
		//--------------------------------------------------------------------------
		//Dados de entrada
		//--------------------------------------------------------------------------
		y = model.addVars(nu_item*nu_bin, GRB_BINARY);
		model.update();
		for (int i= 0; i< nu_item*nu_bin; i++)
		{
   			y[i].set(GRB_DoubleAttr_Obj, bin[i].custo);
			y[i].set(GRB_StringAttr_VarName, "y");
		}
		
		x = new GRBVar* [nu_item];
		for(int i= 0; i< nu_item; i++)
		{
			x[i] = model.addVars(nu_item*nu_bin, GRB_BINARY);
			model.update();
			for (int j= 0; j< nu_item*nu_bin; j++)
			{
				x[i][j].set(GRB_DoubleAttr_Obj, 0);
				x[i][j].set(GRB_StringAttr_VarName, "x");
			}
		}

		//--------------------------------------------------------------
		//funcao objetivo
		//--------------------------------------------------------------
		model.set(GRB_IntAttr_ModelSense, 1);	//0 para maximizar ou 1 para minimizar
		model.update();				//atualiza o modelo
		
		//--------------------------------------------------------------
		//restricao 1 : cada item somente pode ser alocado a um unico bin
		//--------------------------------------------------------------
		for (int i= 0; i< nu_item; i++)//para todo i
		{
			GRBLinExpr r1= 0;//cria a expressao para a restricao 1
			for (int j= 0; j< nu_item*nu_bin; j++)//somatorio em j
				r1+= x[i][j];
			model.addConstr(r1==1, "r1"); // adicionando restricao 1
		}

		//--------------------------------------------------------------
		//restricao 2 : cada item deve respeitar a capacidade do bin
		//--------------------------------------------------------------
		for (int j=0; j< nu_item*nu_bin; j++)//para todo j
		{
			GRBLinExpr r2= 0;//cria a expressao para a restricao 2
			for (int i=0; i< nu_item; i++)//somatorio em i
				r2+= item[i].tamanho*x[i][j];
			model.addConstr(r2<= bin[j].capacidade*y[j], "r2"); // adicionando restricao 2
		}

		//--------------------------------------------------------------
		//inicia a resolucao do modelo
		//--------------------------------------------------------------
		model.update();
		model.optimize();
		
		//--------------------------------------------------------------
		//Exporta a solucao
		//--------------------------------------------------------------
		FOf= model.get(GRB_DoubleAttr_ObjVal);
		for (int j= 0; j< nu_item*nu_bin; j++)
			respostay[j]= y[j].get(GRB_DoubleAttr_X);
		for (int i= 0; i< nu_item; i++)
			for (int j= 0; j< nu_item*nu_bin; j++)
				respostax[i][j]= x[i][j].get(GRB_DoubleAttr_X);
		printf("\n");
	}catch(GRBException e)
	{
		cout << "Gurobi - Codigo do erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}catch(...)
	{
		cout << "Gurobi - Erro durante otimizacao" << endl;
	}
	delete[] y;
 	for (int i= 0; i< nu_item; i++)
    		delete[] x[i];
  	delete[] x;
  	delete env;//termina ambiente gurobi*/
	
	time(&t_fim);
	tempo= difftime(t_fim,t_ini);
	
	printf("O problema foi resolvido em %.3lf segundos.\n", tempo);
	printf("\n--------------------------------\n");
	printf("\nFO= %.2f\nYi:\n", FOf);
	for (int j= 0; j< nu_item*nu_bin; j++)
		printf("%d ", respostay[j]);
	printf("\nXij:\n");
	for (int i=0; i< nu_item; i++)
	{
		for (int j= 0; j< nu_item*nu_bin; j++)
			printf("%d ", respostax[i][j]);
		printf("\n");
	}
	printf("\n--------------------------------\n");
}

//--------------------------------------------------------------------------
//funcao leraquivo03: leitura do arquivo de entrada gurobi02_entrada03.txt
//--------------------------------------------------------------------------
void lerarquivo03()
{
	int cont= 0;
	txt= fopen("gurobi02_entrada03.txt", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada03.txt nao pode ser aberto.\n"); 
	erro = fscanf(txt, "nbarra=\t%d\ncbarra=\t%d\nnitens=\t%d\nb_j=", &nbarra, &cbarra, &nitens);
	if (erro!=3)
		printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada03.txt.\n");
	for (int j=0; j<nitens; j++)
	{
		erro = fscanf(txt, "\t%d", &b[j]);
		if (erro!=1)
			printf("\n\nErro na leitura dos dados do gurobi02_entrada03.txt.\n");
	}
	erro = fscanf(txt, "\nl_j=");
	for (int j=0; j<nitens; j++)
	{
		erro = fscanf(txt, "\t%f", &l[j]);
		if (erro!=1)
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada03.txt.\n");
	}
	printf( "\nArquivo gurobi02_entrada03.txt lido com sucesso.\n"); 
	fclose(txt);
}

//------------------------------------------------------------------------------ 
//Funcao corte01: modelo matematico para o Problema de Corte Unidimensional - Modelo de Kantorovich
//	min sum from i=1 to n y_i 
//	sum from i=1 to n x_{ij} >= b_j, forall j
//	sum from j=1 to m l_j*x_{ij} <= L*y_i, forall i
//	y in 0,1
//	x in setZ
//------------------------------------------------------------------------------ 
void corte01()
{
	//--------------------------------------------------------------------------
	//leitura do arquivo de entrada
	//--------------------------------------------------------------------------
	lerarquivo03();
	int respostay[nbarra], respostax[nbarra][nitens];

	//--------------------------------------------------------------------------
	//GUROBI
	//--------------------------------------------------------------------------
	time(&t_ini);
	GRBEnv* env = 0; 
	GRBVar* y = 0;
	GRBVar** x = 0;
	
	try
	{
		printf("\n--------------------------------\n\n");
		printf("\nIniciando resolucao do problema.\n\n");
		env = new GRBEnv(); //inicia ambiente gurobi
		GRBModel model = GRBModel(*env);//cria um modelo
		model.set(GRB_StringAttr_ModelName, "Corte Kantorovich");
		
		//--------------------------------------------------------------------------
		//Dados de entrada
		//--------------------------------------------------------------------------
		y = model.addVars(nbarra, GRB_BINARY);
		model.update();
		for (int i= 0; i< nbarra; i++)
		{
   			y[i].set(GRB_DoubleAttr_Obj, 1);
			y[i].set(GRB_StringAttr_VarName, "y");
		}
		
		x = new GRBVar* [nbarra];
		for(int i= 0; i<nbarra; i++)
		{
			x[i] = model.addVars(nitens, GRB_INTEGER);
			model.update();
			for (int j= 0; j< nitens; j++)
			{
				x[i][j].set(GRB_DoubleAttr_Obj, 0);
				x[i][j].set(GRB_StringAttr_VarName, "x");
			}
		}

		//--------------------------------------------------------------
		//funcao objetivo: min sum from i=1 to n y_i 
		//--------------------------------------------------------------
		model.set(GRB_IntAttr_ModelSense, 1);	//0 para maximizar ou 1 para minimizar
		model.update();				//atualiza o modelo
		
		//--------------------------------------------------------------
		//restricao 1 : sum from i=1 to n x_{ij} >= b_j, forall j
		//--------------------------------------------------------------
		for (int j= 0; j< nitens; j++)//para todo j
		{
			GRBLinExpr r1= 0;//cria a expressao para a restricao 1
			for (int i= 0; i< nbarra; i++)//somatorio em i
				r1+= x[i][j];
			model.addConstr(r1>=b[j], "r1"); // adicionando restricao 1
		}

		//--------------------------------------------------------------
		//restricao 2 : sum from j=1 to m l_j*x_{ij} <= L*y_i, forall i
		//--------------------------------------------------------------
		for (int i=0; i< nbarra; i++)//para todo i
		{
			GRBLinExpr r2= 0;//cria a expressao para a restricao 2
			for (int j=0; j< nitens; j++)//somatorio em j
				r2+= l[j]*x[i][j];
			model.addConstr(r2<= cbarra*y[i], "r2"); // adicionando restricao 2
		}

		//--------------------------------------------------------------
		//inicia a resolucao do modelo
		//--------------------------------------------------------------
		model.update();
		model.optimize();
		
		//--------------------------------------------------------------
		//Exporta a solucao
		//--------------------------------------------------------------
		FO= model.get(GRB_DoubleAttr_ObjVal);
		for (int i= 0; i< nbarra; i++)
			respostay[i]= y[i].get(GRB_DoubleAttr_X);
		for (int i= 0; i< nbarra; i++)
			for (int j= 0; j< nitens; j++)
				respostax[i][j]= x[i][j].get(GRB_DoubleAttr_X);
		printf("\n");
	}catch(GRBException e)
	{
		cout << "Gurobi - Codigo do erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}catch(...)
	{
		cout << "Gurobi - Erro durante otimizacao" << endl;
	}
	delete[] y;
 	for (int i= 0; i< nbarra; i++)
    		delete[] x[i];
  	delete[] x;
  	delete env;//termina ambiente gurobi*/
	
	time(&t_fim);
	tempo= difftime(t_fim,t_ini);
	
	printf("O problema foi resolvido em %.3lf segundos.\n", tempo);
	printf("\n--------------------------------\n");
	printf("\nFO= %d\nYi:\n", FO);
	for (int i= 0; i< nbarra; i++)
		printf("%d ", respostay[i]);
	printf("\nXij:\n");
	for (int i=0; i< nbarra; i++)
	{
		for (int j= 0; j< nitens; j++)
			printf("%d ", respostax[i][j]);
		printf("\n");
	}
	printf("\n--------------------------------\n");
}

//--------------------------------------------------------------------------
//funcao leraquivo04: leitura do arquivo de entrada gurobi02_entrada04.txt
//--------------------------------------------------------------------------
void lerarquivo04()
{
	int cont= 0;
	txt= fopen("gurobi02_entrada04.txt", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada04.txt nao pode ser aberto.\n"); 
	erro = fscanf(txt, "ncorte=\t%d\nnitens=\t%d\nb_i=", &ncorte, &nitens);
	if (erro!=2)
		printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada04.txt.\n");
	for (int i=0; i<nitens; i++)
	{
		erro = fscanf(txt, "\t%d", &b[i]);
		if (erro!=1)
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada04.txt.\n");
	}
	erro = fscanf(txt, "\n");
	for (int i=0; i<nitens; i++)
	{
		for (int j=0; j<ncorte; j++)
		{
			erro = fscanf(txt, "%d\t", &acorte[j][i]);
			if (erro!=1)
				printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada04.txt.\n");
		}
		erro = fscanf(txt, "\n");
	}
	printf( "\nArquivo gurobi02_entrada04.txt lido com sucesso.\n"); 
	fclose(txt);
}

//------------------------------------------------------------------------------ 
//Funcao corte02: modelo matematico para o Problema de Corte Unidimensional - Modelo de Gilmore/Gomory
//	min sum from j=1 to n x_j
//	sum from j=1 to n a_{j}*x_{j} = b_i, forall i
//	x in setZ
//------------------------------------------------------------------------------ 
void corte02()
{
	//--------------------------------------------------------------------------
	//leitura do arquivo de entrada
	//--------------------------------------------------------------------------
	lerarquivo04();
	int respostax[ncorte];

	//--------------------------------------------------------------------------
	//GUROBI
	//--------------------------------------------------------------------------
	time(&t_ini);
	GRBEnv* env = 0; 
	GRBVar* x = 0;
	
	try
	{
		printf("\n--------------------------------\n\n");
		printf("\nIniciando resolucao do problema.\n\n");
		env = new GRBEnv(); //inicia ambiente gurobi
		GRBModel model = GRBModel(*env);//cria um modelo
		model.set(GRB_StringAttr_ModelName, "Corte Gilmore/Gomory");
		
		//--------------------------------------------------------------------------
		//Dados de entrada
		//--------------------------------------------------------------------------
		x= model.addVars(ncorte, GRB_INTEGER);
		model.update();
		for (int j= 0; j<ncorte; j++)
		{
			x[j].set(GRB_DoubleAttr_Obj, 1);
			x[j].set(GRB_StringAttr_VarName, "x");
		}
		
		//--------------------------------------------------------------
		//funcao objetivo: min sum from i=1 to n x_i
		//--------------------------------------------------------------
		model.set(GRB_IntAttr_ModelSense, 1);	//0 para maximizar ou 1 para minimizar
		model.update();				//atualiza o modelo
		
		//--------------------------------------------------------------
		//restricao 1 : sum from j=1 to n a_{j}*x_{j} >= b_i, forall i
		//--------------------------------------------------------------
		for (int i= 0; i< nitens; i++)//para todo i
		{
			GRBLinExpr r1= 0;//cria a expressao para a restricao 1
			for (int j= 0; j< ncorte; j++)//somatorio em j
				r1+= acorte[j][i]*x[j];
			model.addConstr(r1==b[i], "r1"); // adicionando restricao 1
		}

		//--------------------------------------------------------------
		//inicia a resolucao do modelo
		//--------------------------------------------------------------
		model.update();
		model.optimize();
		
		//--------------------------------------------------------------
		//Exporta a solucao
		//--------------------------------------------------------------
		FO= model.get(GRB_DoubleAttr_ObjVal);
		for (int j= 0; j< ncorte; j++)
			respostax[j]= x[j].get(GRB_DoubleAttr_X);
		printf("\n");
	}catch(GRBException e)
	{
		cout << "Gurobi - Codigo do erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}catch(...)
	{
		cout << "Gurobi - Erro durante otimizacao" << endl;
	}
	delete[] x;
  	delete env;//termina ambiente gurobi
	
	time(&t_fim);
	tempo= difftime(t_fim,t_ini);
	
	printf("O problema foi resolvido em %.3lf segundos.\n", tempo);
	printf("\n--------------------------------\n");
	printf("\nFO= %d\n", FO);
	printf("\nXj:\n");
	for (int j=0; j< ncorte; j++)
		printf("%d ", respostax[j]);
	printf("\n--------------------------------\n");
}

//------------------------------------------------------------------------------ 
// 	[FO]max= 2*x1+8*x2;
//	x1<=5;
//	4*x1^2+9*x2^2<=144;
//	x1>=0;
//	x2>=0;
//------------------------------------------------------------------------------ 
void nao_linear_01()
{
	try
	{
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		//Criando as variaveis
		GRBVar x1 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x1");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x2 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x2");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER

		//Adicionando novas variaveis
		model.update();

		//Funcao Objetivo: max= 2*x1 + 8*x2
		model.setObjective(2*x1 + 8*x2, GRB_MAXIMIZE);

		//primeira restricao: x1 <= 5
		model.addConstr(x1 <= 5, "r1");

 		//segunda restricao: 4*x1*x1 + 9*x2*x2 <= 144
		model.addQConstr(4*x1*x1 + 9*x2*x2 <= 144, "r2");

		//Resolvendo o modelo
 		model.optimize();

		cout << x1.get(GRB_StringAttr_VarName) << " = "<< x1.get(GRB_DoubleAttr_X) << endl;
		cout << x2.get(GRB_StringAttr_VarName) << " = "<< x2.get(GRB_DoubleAttr_X) << endl;
		
		cout << "Obj = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e)
	{
		cout << "Erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...)
	{
		cout << "Erro durante a otimizacao" << endl;
	}
}

//------------------------------------------------------------------------------ 
//	[FO]max= 784*x1-49*x1^2+576*x2-36*x2^2;
//	x1<=5;
//	2*x2<=14;
//	3*x1+2*x2<=17;
//	x1>=0;
//	x2>=0;
//------------------------------------------------------------------------------ 
void nao_linear_02()
{
	try
	{
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		//Criando as variaveis
		GRBVar x1 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x1");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x2 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x2");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER

		//Adicionando novas variaveis
		model.update();

		//Funcao Objetivo: max= 784*x1-49*x1*x1+576*x2-36*x2*x2
		model.setObjective(784*x1-49*x1*x1+576*x2-36*x2*x2, GRB_MAXIMIZE);

		//primeira restricao: x1 <= 5
		model.addConstr(x1 <= 5, "r1");

 		//segunda restricao: 2*x2<=14
		model.addConstr(2*x2<=14, "r2");

		//terceira restricao: 3*x1+2*x2<=17
		model.addConstr(3*x1+2*x2<=17, "r2");

		//Resolvendo o modelo
 		model.optimize();

		cout << x1.get(GRB_StringAttr_VarName) << " = "<< x1.get(GRB_DoubleAttr_X) << endl;
		cout << x2.get(GRB_StringAttr_VarName) << " = "<< x2.get(GRB_DoubleAttr_X) << endl;
		
		cout << "Obj = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e)
	{
		cout << "Erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...)
	{
		cout << "Erro durante a otimizacao" << endl;
	}
}

//------------------------------------------------------------------------------ 
//	[FO]max= 64*x1-16*x1^2+36*x2-9*x2^2;
//	x1<=5;
//	2*x2<=14;
//	3*x1+2*x2<=17;
//	x1>=0;
//	x2>=0;
//------------------------------------------------------------------------------ 
void nao_linear_03()
{
	try
	{
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		//Criando as variaveis
		GRBVar x1 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x1");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x2 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x2");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER

		//Adicionando novas variaveis
		model.update();

		//Funcao Objetivo: max= 64*x1-16*x1^2+36*x2-9*x2^2
		model.setObjective(64*x1-16*x1*x1+36*x2-9*x2*x2, GRB_MAXIMIZE);

		//primeira restricao: x1 <= 5
		model.addConstr(x1 <= 5, "r1");

 		//segunda restricao: 2*x2<=14
		model.addConstr(2*x2<=14, "r2");

		//terceira restricao: 3*x1+2*x2<=17
		model.addConstr(3*x1+2*x2<=17, "r2");

		//Resolvendo o modelo
 		model.optimize();

		cout << x1.get(GRB_StringAttr_VarName) << " = "<< x1.get(GRB_DoubleAttr_X) << endl;
		cout << x2.get(GRB_StringAttr_VarName) << " = "<< x2.get(GRB_DoubleAttr_X) << endl;
		
		cout << "Obj = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e)
	{
		cout << "Erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...)
	{
		cout << "Erro durante a otimizacao" << endl;
	}
}

//--------------------------------------------------------------------------
//funcao leraquivo05: leitura do arquivo de entrada gurobi02_entrada05.txt
//--------------------------------------------------------------------------
void lerarquivo05()
{
	int lixo;
	txt= fopen("gurobi02_entrada05.bss", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada05.bss nao pode ser aberto.\n"); 
	erro = fscanf(txt, "[TRIP]\n");
	for (int i=0; i<N_VIAG; i++)
	{
		erro = fscanf(txt, "Trip%d\t%d\t%d\t%d\t%d\n", &lixo, &viagem[i].hini, &viagem[i].pini, &viagem[i].hfim, &viagem[i].pfim);
		if (erro!=5)
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada05.bss.\n");
	}
	erro = fscanf(txt, "----------------------------------\nTotal de Viagens: %d", &lixo);
	//for (int i=0; i<N_VIAG; i++)
	//	printf("Trip%d\t%d\t%d\t%d\t%d\n", i, viagem[i].hini, viagem[i].pini, viagem[i].hfim, viagem[i].pfim);
	printf( "\nArquivo gurobi02_entrada05.bss lido com sucesso.\n"); 
	fclose(txt);
}

//--------------------------------------------------------------------------
//funcao gerarcolunasppv: gera as colunas para o PPV
//--------------------------------------------------------------------------
void gerarcolunasppv()
{
	int custo, coluna=0;
	txt= fopen("gurobi02_entrada06.txt", "w"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada06.txt nao pode ser aberto.\n"); 
	//--------------------------------------------------------------------------
	//Uma viagem
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
	{
		for(int a2=0; a2<N_VIAG; a2++)
			if(a1==a2)
				fprintf(txt,"1 ");
			else
				fprintf(txt,"0 ");
		custo= PESO_USO_VEICULO;
		fprintf(txt,"%d\n", custo);
		coluna++;
	}
	//--------------------------------------------------------------------------
	//Duas viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini))
			{
				custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL;
				for(int i=0; i<N_VIAG; i++)
					if ((i==a1)||(i==a2))
						fprintf(txt,"1 ");
					else
						fprintf(txt,"0 ");
				fprintf(txt,"%d\n", custo);
				coluna++;
			}
	//--------------------------------------------------------------------------
	//Tres viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
					(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini))
				{
					custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
						+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL;
					for(int i=0; i<N_VIAG; i++)
						if ((i==a1)||(i==a2)||(i==a3))
							fprintf(txt,"1 ");
						else
							fprintf(txt,"0 ");
					fprintf(txt,"%d\n", custo);
					coluna++;
				}
	//--------------------------------------------------------------------------
	//Quatro viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
						(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
						(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini))
					{
						custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
							+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
							+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL;
						for(int i=0; i<N_VIAG; i++)
							if ((i==a1)||(i==a2)||(i==a3)||(i==a4))
								fprintf(txt,"1 ");
							else
								fprintf(txt,"0 ");
						fprintf(txt,"%d\n", custo);
						coluna++;
					}
	//--------------------------------------------------------------------------
	//Cinco viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
							(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
							(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
							(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini))
						{
							custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
								+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
								+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
								+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL;
							for(int i=0; i<N_VIAG; i++)
								if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5))
									fprintf(txt,"1 ");
								else
									fprintf(txt,"0 ");
							fprintf(txt,"%d\n", custo);
							coluna++;
						}
	//--------------------------------------------------------------------------
	//Seis viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						for(int a6=a5+1; a6<N_VIAG; a6++)
							if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
								(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
								(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
								(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini)&&
								(viagem[a5].hfim<=viagem[a6].hini)&&(viagem[a5].pfim==viagem[a6].pini))
							{
								custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a6].hini-viagem[a5].hfim)*PESO_TEMPO_TERMINAL;
								for(int i=0; i<N_VIAG; i++)
									if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5)||(i==a6))
										fprintf(txt,"1 ");
									else
										fprintf(txt,"0 ");
								fprintf(txt,"%d\n", custo);
								coluna++;
							}
	//--------------------------------------------------------------------------
	//Sete viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						for(int a6=a5+1; a6<N_VIAG; a6++)
							for(int a7=a6+1; a7<N_VIAG; a7++)
							if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
								(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
								(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
								(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini)&&
								(viagem[a5].hfim<=viagem[a6].hini)&&(viagem[a5].pfim==viagem[a6].pini)&&
								(viagem[a6].hfim<=viagem[a7].hini)&&(viagem[a6].pfim==viagem[a7].pini))
							{
								custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a6].hini-viagem[a5].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a7].hini-viagem[a6].hfim)*PESO_TEMPO_TERMINAL;
								for(int i=0; i<N_VIAG; i++)
									if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5)||(i==a6)||(i==a7))
										fprintf(txt,"1 ");
									else
										fprintf(txt,"0 ");
								fprintf(txt,"%d\n", custo);
								coluna++;
							}
	//--------------------------------------------------------------------------
	//Oito viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						for(int a6=a5+1; a6<N_VIAG; a6++)
							for(int a7=a6+1; a7<N_VIAG; a7++)
								for(int a8=a7+1; a8<N_VIAG; a8++)
							if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
								(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
								(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
								(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini)&&
								(viagem[a5].hfim<=viagem[a6].hini)&&(viagem[a5].pfim==viagem[a6].pini)&&
								(viagem[a6].hfim<=viagem[a7].hini)&&(viagem[a6].pfim==viagem[a7].pini)&&
								(viagem[a7].hfim<=viagem[a8].hini)&&(viagem[a7].pfim==viagem[a8].pini))
							{
								custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a6].hini-viagem[a5].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a7].hini-viagem[a6].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a8].hini-viagem[a7].hfim)*PESO_TEMPO_TERMINAL;
								for(int i=0; i<N_VIAG; i++)
									if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5)||(i==a6)||(i==a7)||
										(i==a8))
										fprintf(txt,"1 ");
									else
										fprintf(txt,"0 ");
								fprintf(txt,"%d\n", custo);
								coluna++;
							}
	//--------------------------------------------------------------------------
	//Nove viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						for(int a6=a5+1; a6<N_VIAG; a6++)
							for(int a7=a6+1; a7<N_VIAG; a7++)
								for(int a8=a7+1; a8<N_VIAG; a8++)
									for(int a9=a8+1; a9<N_VIAG; a9++)
							if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
								(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
								(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
								(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini)&&
								(viagem[a5].hfim<=viagem[a6].hini)&&(viagem[a5].pfim==viagem[a6].pini)&&
								(viagem[a6].hfim<=viagem[a7].hini)&&(viagem[a6].pfim==viagem[a7].pini)&&
								(viagem[a7].hfim<=viagem[a8].hini)&&(viagem[a7].pfim==viagem[a8].pini)&&
								(viagem[a8].hfim<=viagem[a9].hini)&&(viagem[a8].pfim==viagem[a9].pini))
							{
								custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a6].hini-viagem[a5].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a7].hini-viagem[a6].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a8].hini-viagem[a7].hfim)*PESO_TEMPO_TERMINAL;
								for(int i=0; i<N_VIAG; i++)
									if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5)||(i==a6)||(i==a7)||
										(i==a8)||(i==a9))
										fprintf(txt,"1 ");
									else
										fprintf(txt,"0 ");
								fprintf(txt,"%d\n", custo);
								coluna++;
							}
	//--------------------------------------------------------------------------
	//Dez viagens
	//--------------------------------------------------------------------------
	for(int a1=0; a1<N_VIAG; a1++)
		for(int a2=a1+1; a2<N_VIAG; a2++)
			for(int a3=a2+1; a3<N_VIAG; a3++)
				for(int a4=a3+1; a4<N_VIAG; a4++)
					for(int a5=a4+1; a5<N_VIAG; a5++)
						for(int a6=a5+1; a6<N_VIAG; a6++)
							for(int a7=a6+1; a7<N_VIAG; a7++)
								for(int a8=a7+1; a8<N_VIAG; a8++)
									for(int a9=a8+1; a9<N_VIAG; a9++)
										for(int a10=a9+1; a10<N_VIAG; a10++)
							if((viagem[a1].hfim<=viagem[a2].hini)&&(viagem[a1].pfim==viagem[a2].pini)&&
								(viagem[a2].hfim<=viagem[a3].hini)&&(viagem[a2].pfim==viagem[a3].pini)&&
								(viagem[a3].hfim<=viagem[a4].hini)&&(viagem[a3].pfim==viagem[a4].pini)&&
								(viagem[a4].hfim<=viagem[a5].hini)&&(viagem[a4].pfim==viagem[a5].pini)&&
								(viagem[a5].hfim<=viagem[a6].hini)&&(viagem[a5].pfim==viagem[a6].pini)&&
								(viagem[a6].hfim<=viagem[a7].hini)&&(viagem[a6].pfim==viagem[a7].pini)&&
								(viagem[a7].hfim<=viagem[a8].hini)&&(viagem[a7].pfim==viagem[a8].pini)&&
								(viagem[a8].hfim<=viagem[a9].hini)&&(viagem[a8].pfim==viagem[a9].pini)&&
								(viagem[a9].hfim<=viagem[a10].hini)&&(viagem[a9].pfim==viagem[a10].pini))
							{
								custo= PESO_USO_VEICULO+(viagem[a2].hini-viagem[a1].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a3].hini-viagem[a2].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a4].hini-viagem[a3].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a5].hini-viagem[a4].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a6].hini-viagem[a5].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a7].hini-viagem[a6].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a8].hini-viagem[a7].hfim)*PESO_TEMPO_TERMINAL
									+(viagem[a9].hini-viagem[a8].hfim)*PESO_TEMPO_TERMINAL;
								for(int i=0; i<N_VIAG; i++)
									if ((i==a1)||(i==a2)||(i==a3)||(i==a4)||(i==a5)||(i==a6)||(i==a7)||
										(i==a8)||(i==a9)||(i=a10))
										fprintf(txt,"1 ");
									else
										fprintf(txt,"0 ");
								fprintf(txt,"%d\n", custo);
								coluna++;
							}
	printf( "\nArquivo gurobi02_entrada06.txt criado com sucesso. %d colunas foram geradas.\n", coluna);
	total_colunas= coluna; 
	fclose(txt);
}

//--------------------------------------------------------------------------
//funcao leraquivo06: leitura do arquivo de entrada gurobi02_entrada06.txt
//--------------------------------------------------------------------------
void lerarquivo06()
{
	txt= fopen("gurobi02_entrada06.txt", "r"); 
	if (!txt) 
		printf("\n\nArquivo gurobi02_entrada06.txt nao pode ser aberto.\n"); 
	for (int j=0; j<total_colunas; j++)
	{
		for (int i=0; i<N_VIAG; i++)
		{
			erro = fscanf(txt, "%d ", &col[i][j]);
			if (erro!=1)
				printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada06.txt.\n");
		}
		erro = fscanf(txt, "%d\n", &custo[j]);
		if (erro!=1)
			printf("\n\nErro na leitura dos dados do arquivo gurobi02_entrada06.txt.\n");
	}
	/*for (int j=0; j<total_colunas; j++)
	{
		for (int i=0; i<N_VIAG; i++)
			printf("%d ", col[i][j]);
		printf("%d\n", custo[j]);
	}*/
	printf( "\nArquivo gurobi02_entrada06.txt lido com sucesso.\n"); 
	fclose(txt);
}

//--------------------------------------------------------------------------
//funcao gurobippv: resolucao do ppv utilizando o gurobi
//--------------------------------------------------------------------------
void gurobippv()
{
	//--------------------------------------------------------------------------
	//leitura do arquivo de entrada
	//--------------------------------------------------------------------------
	lerarquivo06();
	int respostax[N_COLUNAS];
	
	//--------------------------------------------------------------------------
	//GUROBI
	//--------------------------------------------------------------------------
	time(&t_ini);
	GRBEnv* env = 0;
	GRBVar* x = 0;
	
	try
	{
		printf("\n--------------------------------\n\n");
		printf("\nIniciando resolucao do problema.\n\n");
		env = new GRBEnv(); //inicia ambiente gurobi
		GRBModel model = GRBModel(*env);//cria um modelo
		model.set(GRB_StringAttr_ModelName, "PPV com geracao de colunas");
		
		//--------------------------------------------------------------------------
		//Dados de entrada
		//--------------------------------------------------------------------------
		x = model.addVars(total_colunas, GRB_BINARY);
		model.update();
		for (int j=0; j<total_colunas; j++)
		{
   			x[j].set(GRB_DoubleAttr_Obj, custo[j]);
			x[j].set(GRB_StringAttr_VarName, "x");
		}

		//--------------------------------------------------------------
		//funcao objetivo: min sum from i=1 to n x_i
		//--------------------------------------------------------------
		model.set(GRB_IntAttr_ModelSense, 1);	//0 para maximizar ou 1 para minimizar
		model.update();				//atualiza o modelo
		
		//--------------------------------------------------------------
		//restricao 1 
		//--------------------------------------------------------------
		for (int i=0; i<N_VIAG; i++)
		{
			GRBLinExpr r1= 0;
			for (int j=0; j<total_colunas; j++)
				r1+= (col[i][j]*x[j]);
			model.addConstr(r1==1, "r1");
		}


		//--------------------------------------------------------------
		//inicia a resolucao do modelo
		//--------------------------------------------------------------
		model.update();
		model.optimize();
		
		//--------------------------------------------------------------
		//Exporta a solucao
		//--------------------------------------------------------------
		FO= model.get(GRB_DoubleAttr_ObjVal);
		for (int j= 0; j< total_colunas; j++)
			respostax[j]= x[j].get(GRB_DoubleAttr_X);
		printf("\n");
	}catch(GRBException e)
	{
		cout << "Gurobi - Codigo do erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}catch(...)
	{
		cout << "Gurobi - Erro durante otimizacao" << endl;
	}
	delete[] x;
  	delete env;//termina ambiente gurobi
	
	time(&t_fim);
	tempo= difftime(t_fim,t_ini);
	
	printf("O problema foi resolvido em %.3lf segundos.\n", tempo);
	printf("\n--------------------------------\n");
	printf("\nFO= %d\n", FO);
	printf("\nXj:\nAs colunas selecionadas foram:\n");
	for (int j= 0; j< total_colunas; j++)
		if(respostax[j])
			printf("%d, ", j);
	printf("\n--------------------------------\n");
	for (int j= 0; j< total_colunas; j++)
		if(respostax[j])
		{
			printf("\n\tColuna %d:\n", j);
			for (int i=0; i< N_VIAG; i++)
				if(col[i][j])
					printf("Trip%d\t%d\t%d\t%d\t%d\n", i, viagem[i].hini, viagem[i].pini, viagem[i].hfim, viagem[i].pfim);
		}
	printf("\n--------------------------------\n");
}

//------------------------------------------------------------------------------ 
// 	[FO]min= 5.5*x1+6.1*x2+7.9*x3+7.01*x4+4.82*x5;
//	10*x1+8*x2+19*x3+21*x4+20*x5+26*x6<=20*(x1+x2+x3+x4+x5+x6);
//	16*x1+6*x2+14*x3+13*x4+45*x5+72*x6<=13*(x1+x2+x3+x4+x5+x6);
//	12*x1+15*x2+7*x3+9*x4+16*x5+54*x6<=10*(x1+x2+x3+x4+x5+x6);
//	29*x1+20*x2+26*x3+24*x4+30*x5+8*x6>=25*(x1+x2+x3+x4+x5+x6);
//	x1+x2+x3+x4+x5+x6==100;
//	x1, x2, x3, x4, x5, x6: >=0
//------------------------------------------------------------------------------ 
void linear_01()
{
	try
	{
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		//Criando as variaveis
		GRBVar x1 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x1");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x2 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x2");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x3 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x3");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x4 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x4");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x5 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x5");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER
		GRBVar x6 = model.addVar(0, 1000, 0, GRB_CONTINUOUS, "x6");//GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER		

		//Adicionando novas variaveis
		model.update();

		//Funcao Objetivo: min= 5.5*x1+6.1*x2+7.9*x3+7.01*x4+4.82*x5
		model.setObjective(5.5*x1+6.1*x2+7.9*x3+7.01*x4+4.82*x5, GRB_MINIMIZE);

		//primeira restricao: 10*x1+8*x2+19*x3+21*x4+20*x5+26*x6<=20(x1+x2+x3+x4+x5+x6)
		model.addConstr(10*x1+8*x2+19*x3+21*x4+20*x5+26*x6<=20*(x1+x2+x3+x4+x5+x6), "r1");

		//segunda restricao: 16*x1+6*x2+14*x3+13*x4+45*x5+72*x6<=13(x1+x2+x3+x4+x5+x6)
		model.addConstr(16*x1+6*x2+14*x3+13*x4+45*x5+72*x6<=13*(x1+x2+x3+x4+x5+x6), "r2");

		//terceira restricao: 12*x1+15*x2+7*x3+9*x4+16*x5+54*x6<=10(x1+x2+x3+x4+x5+x6)
		model.addConstr(12*x1+15*x2+7*x3+9*x4+16*x5+54*x6<=10*(x1+x2+x3+x4+x5+x6), "r3");

		//quarta restricao: 29*x1+20*x2+26*x3+24*x4+30*x5+8*x6>=25(x1+x2+x3+x4+x5+x6)
		model.addConstr(29*x1+20*x2+26*x3+24*x4+30*x5+8*x6>=25*(x1+x2+x3+x4+x5+x6), "r4");

		//quinta restricao: x1+x2+x3+x4+x5+x6==100
		model.addConstr(x1+x2+x3+x4+x5+x6==100, "r5");

		//Resolvendo o modelo
 		model.optimize();

		cout << x1.get(GRB_StringAttr_VarName) << " = "<< x1.get(GRB_DoubleAttr_X) << endl;
		cout << x2.get(GRB_StringAttr_VarName) << " = "<< x2.get(GRB_DoubleAttr_X) << endl;
		cout << x3.get(GRB_StringAttr_VarName) << " = "<< x3.get(GRB_DoubleAttr_X) << endl;
		cout << x4.get(GRB_StringAttr_VarName) << " = "<< x4.get(GRB_DoubleAttr_X) << endl;
		cout << x5.get(GRB_StringAttr_VarName) << " = "<< x5.get(GRB_DoubleAttr_X) << endl;
		cout << x6.get(GRB_StringAttr_VarName) << " = "<< x6.get(GRB_DoubleAttr_X) << endl;
		
		cout << "Obj = " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e)
	{
		cout << "Erro = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...)
	{
		cout << "Erro durante a otimizacao" << endl;
	}
}
