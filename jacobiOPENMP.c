#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
#include<sys/time.h>

#define N 10000

#define LIMITE 1000
static double ERRO = 0.0000000001;

double tempoCorrente(void){
     struct timeval tval;
     gettimeofday(&tval, NULL);
     return (tval.tv_sec + tval.tv_usec/1000000.0);
}

double norma_vetor(double *x){
	double soma = 0;
	int i;
	for(i=0;i<N;i++){
		soma += pow(x[i],2);
	}
	return sqrt(soma);
}

void main(int argc, char **argv){

int threads;
//n = atoi(argv[1]);
threads = atoi(argv[1]);

int i,j,id,iter;
double ti,tf;
double *a = (double *)malloc(N*N*sizeof(double));
double *b = (double *)malloc(N*sizeof(double));
//double input[N*N] = {10., -1., 2., 0. ,-1., 11., -1., 3. ,2., -1., 10., -1. ,0.0, 3., -1., 8.};
//double input[N*N] = {6,0,0,0,0,0,0,1,4,1,0,0,0,0,0,1,4,1,0,0,0,0,0,1,4,1,0,0,0,0,0,1,4,1,0,0,0,0,0,1,4,1,0,0,0,0,0,0,6};
//double vetor[N] = {6., 25., -11., 15.};
//double vetor[N]= {0,1,2,-6,2,1,0};


double *x = (double *)malloc(N*sizeof(double));
double *novo_x = (double *)malloc(N*sizeof(double));

/*for(i=0;i<N*N;i++){
    a[i]=input[i];
}*/

for(i=0;i<N;i++){
    //b[i]=vetor[i];
	b[i]=1;
    x[i]=0;
    novo_x[i]=0;
}

for(i=0;i<N*N;i+=N+1){
    if(i==0 || i==N*N-1){
        a[i]=6;
    }
	else{
        a[i-1]=1;
        a[i]=4;
        a[i+1]=1;
    }
}

int flag=0;
double novoX=0,soma=0;
iter = 0;
ti = tempoCorrente();
omp_set_num_threads(threads);

#pragma omp parallel private(id,j,i,soma)
{
id = omp_get_thread_num();
while (iter < LIMITE ){
	for(i=id;i<N;i+=threads){
		for(j=0;j<N;j++){
			if(i!=j){
				soma += a[i*N+j] * x[j]; 
			}
		}
		novo_x[i]=(b[i] - soma) / a[i*N+i];
        soma=0;
	}

    if(id==0){
	    if(fabs(norma_vetor(x) - norma_vetor(novo_x)) < ERRO){
            iter=LIMITE;
	    }
	    else{
            for(j=0;j<N;j++){
			    x[j]=novo_x[j];
		    }
	    }
    }

    #pragma omp barrier
    iter++;
    }
}

tf = tempoCorrente();

    printf("\n------------------RESPOSTA--------------------\n");
    /*if(flag==1){
        for(i=0;i<N;i++){
	        printf("%.13lf ",novo_x[i]);
        } 
    }
    else{
        for(i=0;i<N;i++){
	        printf("%.13lf ",x[i]);
        } 
    }
    printf("\n");*/
    printf("Tempo = %lf\n", tf - ti );

free(x);
free(novo_x);
free(a);
free(b);

}