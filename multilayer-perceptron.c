#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/* DADOS DO PROBLEMA */
#define nBias 1
#define nEnt 6 // VALOR DE ENTRADA
#define qtdDadosParaTreina  64

/* DADOS A SEREM MODIFICADOS PARA MELHORAR O DESEMPENHO */
#define txApr 0.002
#define momento 0.08
#define camadas 3
#define nOc 10 // NUMERO DE OCULTAS
#define nS 1 // NUMERO DE SAIDAS
const int numDeEpocas = 100000;
double erroPretendido = 0.000001;
/*
1 - 12
#define txApr 0.0018
#define momento 0.008
#define camadas 2
#define nOc 12 // NUMERO DE OCULTAS
#define nS 1 // NUMERO DE SAIDAS
const int numDeEpocas = 150000;
// tempo: 271.000000 segundos

*/

/* VARIAVEIS AUXILIARES */
FILE *fp;
FILE *fp1;
FILE *fp2;
char ch[1] = "";
char s[15] = "texto_braille";
char s1[15] = "erro";
char s2[15] = "epocas";
int numDoPadrao;
double erroDestePadrao = 0.0;
double saidaAtual[nS];

double erroGeral = 0.0;
double errorTemp = 0.0;

/*
	- vn: VALOR DO NEUROUNIO
	- pen: PESO ENTRADA DO NEURONIO
	- psn: PESO SAIDO DO NEURONIO
	- een: ERRO NA ENTRADA DO NEURONIO
	- esn: ERRO NA SAIDO DO NEURONIO
*/

/* FUNCOES */
void iniciaValoresBiasOcultas();
void iniciaPesos();
void treinaRede();
void altPesoSaidaOculta();
void altPesoEntOculta(int numDoPadrao); 
void calculaErroGeral();
void mostraResultado();
double sigmoid(double sinal);
double getRand();
int hexval(char c);

int soma(int *entradas);
int degrau(int s);


/* NEURONIO COMO SE FOSSE OBJETO */
double vn[camadas][nOc+nBias];
double pen[camadas][nOc][nEnt+nBias+nS];
double psn[camadas][nOc+nBias][nEnt+nBias+nS];
double een[camadas][nOc+nBias][nEnt+nBias+nS];
double esn[camadas][nOc+nBias][nEnt+nBias+nS];

int entradaTreino[qtdDadosParaTreina][nEnt+nBias] = {
	
	{0,0,0,0,0,0},//(space) => 0
	{0,1,1,1,0,1},//the => 29
	{0,0,0,0,1,0},//(contraction) => 2
	{0,0,1,1,1,1},//(number prefix) => 15
	{1,1,0,1,0,1},//ed => 53
	{1,0,0,1,0,1},//sh => 37
	{1,1,1,1,0,1},//and => 61
	{0,0,1,0,0,0},//' => 8
	{1,1,1,0,1,1},//of => 59
	{0,1,1,1,1,1},//with => 31
	{1,0,0,0,0,1},//ch => 33
	{0,0,1,1,0,1},//ing => 13
	{0,0,0,0,0,1},//(uppercase prefix)  => 1
	{0,0,1,0,0,1},//- => 9
	{0,0,0,1,0,1},//(italic prefix) => 5
	{0,0,1,1,0,0},//st => 12
	{0,0,1,0,1,1},//" => 11
	{0,1,0,0,0,0},//, => 16
	{0,1,1,0,0,0},//; => 24
	{0,1,0,0,1,0},//: => 18
	{0,1,0,0,1,1},//. => 19
	{0,1,0,0,0,1},//en => 17
	{0,1,1,0,1,0},//! => 26
	{0,1,1,0,1,1},//( or ) => 27
	{0,1,1,0,0,1},//“ or ? => 25
	{0,0,1,0,1,0},//in => 10
	{1,0,0,0,1,1},//wh => 35
	{0,0,0,0,1,1},//(letter prefix) => 3
	{1,1,0,0,0,1},//gh => 49
	{1,1,1,1,1,1},//for => 63
	{0,0,1,1,1,0},//ar => 14
	{1,0,0,1,1,1},//th => 39
	{0,0,0,1,0,0},//(accent prefix) => 4
	
	{1,0,0,0,0,0},//a => 32
	{1,1,0,0,0,0},//b => 48
	{1,0,0,1,0,0},//c => 36
	{1,0,0,1,1,0},//d => 38
	{1,0,0,0,1,0},//e => 34
	{1,1,0,1,0,0},//f => 52
	{1,1,0,1,1,0},//g => 54
	{1,1,0,0,1,0},//h => 50
	{0,1,0,1,0,0},//i => 20
	{0,1,0,1,1,0},//j => 22
	{1,0,1,0,0,0},//k => 40
	{1,1,1,0,0,0},//l => 56
	{1,0,1,1,0,0},//m => 44
	{1,0,1,1,1,0},//n => 46
	{1,0,1,0,1,0},//o => 42
	{1,1,1,1,0,0},//p => 60
	{1,1,1,1,1,0},//q => 62
	{1,1,1,0,1,0},//r => 58
	{0,1,1,1,0,0},//s => 28
	{0,1,1,1,1,0},//t => 30
	{1,0,1,0,0,1},//u => 41
	{1,1,1,0,0,1},//v => 57
	{0,1,0,1,1,1},//w => 23
	{1,0,1,1,0,1},//x => 45
	{1,0,1,1,1,1},//y => 47
	{1,0,1,0,1,1},//z => 43
	{0,1,0,1,0,1},//ow => 21
	{1,1,0,0,1,1},//ou => 51
	{1,1,0,1,1,1},//er => 55
	{0,0,0,1,1,0},//(currency prefix) => 6
	{0,0,0,1,1,1}//(contraction) => 7*/
	
};
float saidaTreino[qtdDadosParaTreina][nS] = {
	{32},
	{33},
	{34},
	{35},
	{36},
	{37},
	{38},
	{39},
	{40},
	{41},
	{42},
	{43},
	{44},
	{45},
	{46},
	{47},
	{48},
	{49},
	{50},
	{51},
	{52},
	{53},
	{54},
	{55},
	{56},
	{57},
	{58},
	{59},
	{60},
	{61},
	{62},
	{63},
	{64},
	{65},
	{66},
	{67},
	{68},
	{69},
	{70},
	{71},
	{72},
	{73},
	{74},
	{75},
	{76},
	{77},
	{78},
	{79},
	{80},
	{81},
	{82},
	{83},
	{84},
	{85},
	{86},
	{87},
	{88},
	{89},
	{90},
	{91},
	{92},
	{93},
	{94},
	{95}
};
																
int main(void){	
	time_t inicio, fim;
	inicio = time(NULL);
	int entradas[nEnt];
	int numero_erro;
	int numero_epocas;
	iniciaValoresBiasOcultas();
	iniciaPesos();
	//iniciaDados();
	fp1 = fopen(s1,"w");
   fp2 = fopen(s2,"w");
   int j, i, k, c, e, o, s, passa;
	/* TREINA REDE */
	fputs("a = [",fp1);
	fputs("b = [",fp2);
	for(j = 0;j <= numDeEpocas;j++){
		if(numDeEpocas!=0){
			for(i = 0;i<qtdDadosParaTreina;i++){
				treinaRede(i);			
				altPesoSaidaOculta();
				altPesoEntOculta(i);
			}
		}
		/* MOSTRARA O ERRO DA REDE */
     	calculaErroGeral();
     	/*if(erroGeral <= erroPretendido){
     		printf("epoca = %d Erro geral = %f\n", j, erroGeral);
     		break;
     	}*/
     	if(j > (numDeEpocas - 10) || ((j%10000)==0)){
     		if(j == numDeEpocas){
     			for(c = 0;c<camadas;c++){
	     			for(o = 0;o<nOc+nBias;o++){
						for(s = 0;s<nS;s++){
							/*printf(
								"\tpsn[%d][%d][%d] = %lf;\n\tesn[%d][%d][%d] = %lf;\n", c, o, s, psn[c][o][s], 
								c, o, s, esn[c][o][s]
							);*/
						}
					}
					for(o = 0;o<nOc;o++){
						for(e = 0;e<nEnt+nBias;e++){
				    		/*printf(
				    			"\tpen[%d][%d][%d] = %lf;\n\teen[%d][%d][%d] = %lf;\n", c, e, o, pen[c][o][e],
				    			c, o, e, een[c][o][e]
				    		);*/
				   	}
				  	}
				}
     		}
     		printf("epoca = %d Erro geral = %lf\n", j, erroGeral);
     	}
     	
     	//putw(round(erroGeral),fp1);
     	//passa=(int)round(erroGeral);
     	fprintf(fp1, "%.2f", erroGeral);
     	putc(32,fp1);
     	fprintf(fp2, "%d", j);
     	putc(32,fp2);
     	//putw(j,);
		
     	
	}
	fputs("];",fp1);
	fputs("];",fp2);
	fclose(fp1);
   fclose(fp2);
	mostraResultado();
	//while (1) {
		/*
		int i = 0;
		for(i =0; i<(nEnt-nBias); i++) {
			printf("Entrada %d............: ", (i+1));
			scanf("%d", &entradas[i]);
		}
		printf("Resultado............: %d = %c\n\n", degrau(soma(entradas)), soma(entradas));
		*/
		/*for(e = 0;e<nOc;e++){
			for(c = 0;c<(nEnt+nBias);c++){
				printf("\tENTRADA[%d][%d] = ENTRADA: %f \n", e, c, vn[c][e][c]);
			}
		}*
		
		fp = fopen(s,"r");
		// Verifica se a abertura ocorreu com sucesso
		if(fp==NULL){
			printf("Impossivel abrir arquivo %s",s);
		}else{
			printf("Arquivo %s aberto com sucesso \n\n",s);			
			printf("\n");
			while((ch[0] = getc(fp) ) != EOF){
				if(ch[0]!='\n'){
					//printf("Resultado..::%c \n",ch[0]);
					if(ch[0]!='\n'){
						entradas[t] = hexval(ch[0]);
					}else{
						entradas[t] = 1;
					}
				}
				if(t == (nEnt-nBias)){
					//printf("Entrada: %d == Resultado..: %d \n", t, entradas[t]);
					printf("%c", soma(entradas));
					t = 0;
				}else{
					t++;
				}
			};
			fclose(fp);
			printf("\n");
			break;
		}
		break;
		*/
	//}
	fim = time(NULL);
	printf("Programa executado em %f segundos\n", difftime(fim, inicio));
	return 0;
}

double sigmoid(double sinal){
	return 1 / (1 + exp((double) -sinal));
}

int hexval(char c){    
   int n;
   if ((c>='0') && (c<='9')){
      n = c - '0';
   }else if ((c>='A') && (c<='F')){
      n = c - 'A' + 10;
   }else if ((c>='a') && (c<='f')){
      n = c - 'a' + 10;
   }else{
      n = 0;
   }
   return n;
}

int soma(int *entradas){	

	int i, j, c, d, e, o, s;
	/* PESOS DAS OCULTAS */
	float resultado[nS];
	for(e = 0;e<(0);e++){
		for(i = 0;i<nOc;i++){   	
	  		for(c = 0;c<nS;c++){
		   	vn[c][o] = 0.0;
		   	for(j = 0;j<(nEnt);j++){	
					vn[c][o] = vn[c][o] + (txApr * entradas[j] * pen[e][i][j]);
					//printf("Entrada: %d \n", entradas[j]);
				}
				vn[c][o] = sigmoid(vn[c][o]);
			}
		}
	}
	for(s = 0;s<nS;s++){
		resultado[s] = 0.0;
	}
	/* SAIDA DA REDE */
	/*for(c = 0;c<(camadas);c++){
		for(o = 0;o<nOc;o++){
			for(s = 0;s<nS;s++){
				resultado[s] = resultado[s] + (vn[c][o] * psn[c][o][s]);
			}
	   }
	}*/
	
	for(o = 0;o<(nOc+nBias);o++){
		for(s = 0;s<nS;s++){
			resultado[s] = resultado[s] + (vn[0][o] * psn[0][o][s]);
		}
   }
	
	int a[nS];
   int total = 0;
   for(c = 0;c<nS;c++){
   	a[c] = (int)round(resultado[c]);
   	//printf("SAIDA: %d = %f \n", c, resultado[c]);
   	//printf("SAIDA: %d\n", a[c]);
	}
	/*total = a[0] * 10;
	total = total + a[1];
	printf("SAIDA: %d\n", total);*/
	if(nS==1){
		//printf("SAIDA: %s\n", a[0]);
		return a[0];
	}else{
		return total;
	}
}

int degrau(int s){
  return s;
}

void treinaRede(numDoPadrao){
	/* CALCULO DAS SAIDAS DOS OCULTOS */	
   int i, j, c, e, o, s, o2;   
   if(camadas>1){
   	/* ENTRADA PARA OCULTA */
	   for(c = 0;c<(1);c++){
	   	for(o = 0;o<nOc;o++){
				vn[c][o] = 0.0;
				for(e = 0;e<(nEnt+nBias);e++){
					vn[c][o] = vn[c][o] + (txApr * entradaTreino[numDoPadrao][e] * pen[c][o][e]);
				}
				vn[c][o] = sigmoid(vn[c][o]);
			}
		}
		/* OCULTA PARA OCULTA */
		for(c = 1;c<(camadas);c++){
		   for(o = 0;o<(nOc);o++){
				vn[c][o] = 0.0;
				for(o2 = 0;o2<(nOc);o2++){
					vn[c][o] = vn[c][o] + (txApr * vn[c-1][o2] * pen[c][o][o2]);
				}
				vn[c][o] = sigmoid(vn[c][o]);
			}
		}
	}else{
		/* ENTRADA PARA SAIDA */
		for(o = 0;o<(nOc);o++){ // BIAS NAO POSSUE ENTRADA, SOMENTE SAIDA
			vn[0][o] = 0.0;
			for(e = 0;e<(nEnt+nBias);e++){
				vn[0][o] = vn[0][o] + (txApr * entradaTreino[numDoPadrao][e] * pen[0][o][e]);
			}
			vn[0][o] = sigmoid(vn[0][o]);
		}
	}
   /* CALCULO DA SAIDA DA REDE */
   for(s = 0;s<nS;s++){
   	saidaAtual[s] = 0.0;
   }
   if(camadas>1){
   	/* ENTRADA PARA OCULTA */
   	for(s = 0;s<nS;s++){
	   	for(c = 0;c<(camadas-1);c++){
			   for(o = 0;o<(nOc+nBias);o++){ //BIAS
					for(o2 = 0;o2<(nOc);o2++){
						saidaAtual[s] = saidaAtual[s] + (vn[c][o] * psn[c][o][o2]);
					}
				}
		   }
		}
		/* OCULTA PARA OCULTA */
		for(s = 0;s<nS;s++){
			for(c = (camadas-1);c<(camadas);c++){
			   for(o = 0;o<(nOc+nBias);o++){ //BIAS
					saidaAtual[s] = saidaAtual[s] + (vn[c][o] * psn[c][o][s]);
				}
		   }
		}
   }else{
   	/* ENTRADA PARA SAIDA */
   	for(s = 0;s<nS;s++){
   		for(o = 0;o<(nOc+nBias);o++){ //BIAS
   		//for(o = 0;o<(nOc);o++){
				saidaAtual[s] = saidaAtual[s] + (vn[0][o] * psn[0][o][s]);
			}
	   }
   }
   for(s = 0;s<nS;s++){
   	erroDestePadrao = saidaAtual[s] - saidaTreino[numDoPadrao][s];
   }
   //printf("Erro deste padrao: - %d V: %f \n", numDoPadrao, erroDestePadrao);
}

void altPesoSaidaOculta(void){
	int e, o, s, c, o2;
	if(camadas>1){
		/* 1 CAMADA ENTRADA PARA OCULTA */
		for(c = 0;c<(camadas-1);c++){
		   for(o = 0;o<(nOc+nBias);o++){ // COM BIAS
		   	for(o2 = 0;o2<nOc;o2++){ // NAO EXISTE SAIDA PARA BIAS
			   	errorTemp = psn[c][o][o2];
					psn[c][o][o2] = psn[c][o][o2] - 
							(txApr * erroDestePadrao * vn[c][o]) + (momento * (psn[c][o][o2] - esn[c][o][o2]));
					esn[c][o][o2] = errorTemp;
				}
		   }
		}
		/* 2 CAMADA OCULTA PARA SAIDA */
		for(c = (camadas-1);c<(camadas);c++){
		   for(o = 0;o<(nOc+nBias);o++){ // COM BIAS
		   	for(s = 0;s<nS;s++){
			   	errorTemp = psn[c][o][s];
					psn[c][o][s] = psn[c][o][s] - 
							(txApr * erroDestePadrao * vn[c][o]) + (momento * (psn[c][o][s] - esn[c][o][s]));
					esn[c][o][s] = errorTemp;
				}
		   }
		}
	}else{
		/* ENTRADA PARA SAIDA */
		for(o = 0;o<(nOc+nBias);o++){ // COM BIAS
	   	for(s = 0;s<nS;s++){
		   	errorTemp = psn[0][o][s];
		   	psn[0][o][s] = psn[0][o][s] - 
						((double)txApr * erroDestePadrao * vn[0][o]) + (momento * (psn[0][o][s] - esn[0][o][s]));
				esn[0][o][s] = errorTemp;
			}
	   }
	}
}

void altPesoEntOculta(int numDoPadrao){
	int c, e, o, s, o2, o3;
	if(camadas>1){
		/* ENTRADA PARA OCULTA */
		for(c = 0;c<(1);c++){
			for(o = 0;o<(nOc);o++){ // BIAS NAO POSSUE ENTRADA
				for(o2 = 0;o2<(nOc);o2++){
					for(e = 0;e<(nEnt+nBias);e++){
						errorTemp = pen[c][o][e];
			    		pen[c][o][e] = pen[c][o][e] - ((1 - (vn[c][o] * vn[c][o])) * 
			    					psn[c][o][o2] * erroDestePadrao * (double)txApr * entradaTreino[numDoPadrao][e]
			    				) + (momento * (pen[c][o][e] - een[c][o][e]));
			    		een[c][o][e] = errorTemp;
			   	}
			   }
		  	}
		}
		/* OCULTA PARA OCULTA */
		if(camadas>2){
			for(c = 1;c<(camadas-1);c++){
				for(o = 0;o<(nOc);o++){ // BIAS NAO POSSUE ENTRADA
					for(o2 = 0;o2<(nOc);o2++){
						for(o3 = 0;o3<(nOc+nBias);o3++){
							errorTemp = pen[c][o][o3];
				    		pen[c][o][o3] = pen[c][o][o3] - ((1 - (vn[c][o] * vn[c][o])) * 
				    					psn[c][o][o2] * erroDestePadrao * (double)txApr * vn[c-1][o3]
				    				) + (momento * (pen[c][o][o3] - een[c][o][o3]));
				    		een[c][o][o3] = errorTemp;
					   }
					}
			  	}
			}
		}
		/* OCULTA PARA SAIDA */
		for(c = (camadas-1);c<(camadas);c++){
			for(o = 0;o<(nOc);o++){ // BIAS NAO POSSUE ENTRADA
				for(s = 0;s<nS;s++){
					for(o2 = 0;o2<(nOc+nBias);o2++){
						errorTemp = pen[c][o][o2];
			    		pen[c][o][o2] = pen[c][o][o2] - ((1 - (vn[c][o] * vn[c][o])) * 
			    					psn[c][o][s] * erroDestePadrao * (double)txApr * vn[c-1][o2]
			    				) + (momento * (pen[c][o][o2] - een[c][o][o2]));
			    		een[c][o][o2] = errorTemp;
				   }
			  	}
			}
		}
  	}else{
  		for(o = 0;o<(nOc);o++){ // BIAS NAO POSSUE ENTRADA
			for(s = 0;s<nS;s++){
				for(e = 0;e<(nEnt+nBias);e++){
					errorTemp = pen[0][o][e];
		    		pen[0][o][e] = pen[0][o][e] - ((1 - (vn[0][o] * vn[0][o])) * 
		    					psn[0][o][s] * erroDestePadrao * (double)txApr * entradaTreino[numDoPadrao][e]
		    				) + (momento * (pen[0][o][e] - een[0][o][e]));
		    		een[0][o][e] = errorTemp;
		   	}
		   }
	  	}
  	}
}

double getRand(void){
	return ((double)rand()) / (double)RAND_MAX;
	//return 0;
}

void iniciaValoresBiasOcultas(void){
	int e, k, o, s, c;
	if(camadas>1){
	   for(c = 0;c<(camadas);c++){
	   	for(o = nOc;o<(nOc+nBias);o++){
		   	vn[c][o] = 1;
		   	printf("\tCAMADA:[%d]-OCULTA[%d] = VALOR: %f \n", c, o, vn[c][o]);
			}
	   }
		
	}else{
		for(o = nOc;o<(nOc+nBias);o++){
		   vn[0][o] = 1;
		   //printf("\tSAIDA VALOR: CAMADA[%d]-OCULTA[%d] = VALOR: %f \n", (0), (o), vn[0][o]);
	   }
	}
	// BIAS DA ENTRADA
	for(k = 0;k<(qtdDadosParaTreina);k++){
		for(e = nEnt;e<(nEnt+nBias);e++){
		  	entradaTreino[k][e] = 1;
	  		//printf("\tENTRADA[%d][%d] = ENTRADA: %d \n", k, e, entradaTreino[k][e]);
   	}
   }
}

void iniciaPesos(void){
	int c, o, o2, o3, e, s;
	if(camadas>1){
		for(c = 0;c<(camadas);c++){
			for(o = 0;o<(nOc+nBias);o++){
				for(o2 = 0;o2<(nOc+nBias);o2++){
					psn[c][o][o2] = (getRand() - 0.5)/2;
					psn[c][o][o2] = (getRand() - 0.5)/2;
					for(o3 = 0;o3<(nOc);o3++){
						pen[c][o][o3] = (getRand() - 0.5)/10;
						pen[c][o][o3] = (getRand() - 0.5)/10;
					}
				}
			}
		}
	}else{
		for(o = 0;o<(nOc+nBias);o++){
			for(s = 0;s<(nS+nBias);s++){
				psn[0][o][s] = (getRand() - 0.5)/2;
				//printf("\tpsn[c][%d][%d] = VALOR: %f \n", o, s, psn[0][o][s]);
				for(e = 0;e<(nEnt+nBias);e++)
				{
					pen[0][o][e] = (getRand() - 0.5)/10;
					//pen[0][o][e] = 0.02;
					//printf("\tpen[c][%d][%d] = VALOR: %f \n", o, e, pen[0][o][e]);
				}
			}
		}
	}
}

void mostraResultado(void){
	int i, c, e;
	for(i = 0;i<qtdDadosParaTreina;i++){
  		treinaRede(i);
  		//for(e = 0;e<camadas;e++){
	  		for(c = 0;c<nS;c++){
		   	if(round(saidaAtual[c]) == saidaTreino[i][c]){
	  				//printf("SAIDA: %d - ATUAL: %f - SAIDA ATUAL: %f CERTO\n", i+1, saidaTreino[i][c], saidaAtual[c]);
	  			}else{
	  				printf("SAIDA: %d - ATUAL: %f - SAIDA ATUAL: %f ERRADO <-----------\n", i+1, saidaTreino[i][c], saidaAtual[c]);
	  			}
	  		}
  		//}
 	}
}

void calculaErroGeral(void){
	int i, s;
   erroGeral = 0.0;
   for(i = 0;i<qtdDadosParaTreina;i++){
      treinaRede(i);
      for(s = 0;s<nS;s++){
      	erroGeral = erroGeral + (erroDestePadrao * erroDestePadrao);
      }
   }
   erroGeral = erroGeral/qtdDadosParaTreina;
   erroGeral = sqrt(erroGeral);
}
