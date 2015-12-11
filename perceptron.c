#include <stdio.h>

#define ENTRADA 2
#define VLR_BIAS 1
#define TX_APR 1
#define QTD_DADOS_TREINAMENTOS 4
#define EPOCAS 100

const int dadosTreinamento[4][3] = {{0,0,0}, {0,1,0}, {1,0,0}, {1,1,1} };
float soma(int e1, int e2, float* sinapses);
int degrau(float s);
float regraDelta(int erro, int entrada, float sinapse);
void treinarRede(float *sinapses);

int main()
{
	float sinapses[ENTRADA+1] = {0,0,0};
	int e1, e2;

	treinarRede(sinapses);
	while(1){
		printf("Entrada 1 .................: ");
		scanf("%d", &e1);
		printf("Entrada 2 .................: ");
		scanf("%d", &e2);
		printf("Resultado............: %d \n", degrau(soma(e1, e2, sinapses)));
	}

	return 0;
}

float soma(int e1, int e2, float* sinapses)
{
	float resultado = 0;
	resultado = resultado + sinapses[0] * VLR_BIAS;
	resultado = resultado + sinapses[1] * e1;
	resultado = resultado + sinapses[2] * e2;
	return resultado;
}

int degrau(float s)
{
	if(s > 0) return 1;
	return 0;
}

float regraDelta(int erro, int entrada, float sinapse)
{
	printf("erro: %d - Entrada: %d - SAIDA: %f\n", erro, entrada, (sinapse + (TX_APR * erro * entrada)));
	return (sinapse + (TX_APR * erro * entrada));
	
}

void treinarRede(float * sinapses)
{
	int i, j, resposta, erro;
	// NUMERO DE EPOCAS
	for(i = 0; i < EPOCAS; i++){
		// CONJUNTO DE TREINAMENTOS
		for(j = 0; j < QTD_DADOS_TREINAMENTOS; j++){
			resposta = degrau(soma(dadosTreinamento[j][0], dadosTreinamento[j][1], sinapses));
			erro = dadosTreinamento[j][2] - resposta;
			if(erro){
				sinapses[0] = regraDelta(erro, VLR_BIAS, sinapses[0]);
				sinapses[1] = regraDelta(erro, dadosTreinamento[j][0], sinapses[1]);
				sinapses[2] = regraDelta(erro, dadosTreinamento[j][1], sinapses[2]);
			}
		}
	}
	printf("RESULTADO TREINAMENTO");
	printf("w[bias] = %f\n", sinapses[0]);
	printf("w[e1] = %f\n", sinapses[1]);
	printf("w[e2] = %f\n", sinapses[2]);	
}
