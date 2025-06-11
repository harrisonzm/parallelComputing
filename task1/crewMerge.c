#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
/*
POR FAVOR LEER LAS CONSIDERACIONES EN MAIN ANTES DE LEER EL CODIGO
condiciones del trabajo :
    se garantiza estar ordenados ascendentemente y no tener elementos repetidos
    r y s  10-100 elementos y r<=s

    arreglo A de tamano r
    arreglo B de tamano s
    variable N entre 2-32  entero

    se debe imprimir los siguientes pasos
        1. arreglo A y B prima
        2. arreglo V
        3. arreglo Q
        4. los arreglos que le tocaria mergear al i-esimo procesador y su resultado
        5. arreglo final mergeado
*/
typedef struct Step1Result
{
    int *Aprime;
    int *Bprime;
    int primeSize;
} Step1Result;

struct Step1Result step1(int A[], int B[], int *N, int *r, int *s)
{
    printf("---------encuentra los pivotes en A y en B-----------\n");
    printf("---------Si el programa se ejecutase en paralelo, entonces los arreglos A' y B' quedarian asi -----------\n");
    int primeSize = *N - 1;
    int *Aprime = malloc(primeSize * sizeof(int));
    int *Bprime = malloc(primeSize * sizeof(int));
    struct Step1Result result = {Aprime, Bprime, primeSize};
    for (int i = 1; i < *N; i++)
    {
        int indexa = i * ceil((*r) / (*N));
        int indexb = i * ceil((*s) / (*N));
        Aprime[i - 1] = A[indexa - 1];
        Bprime[i - 1] = B[indexb - 1];
    }
    printf("El arreglo A' quedaria asi:\n A' = [ ");
    for (int i = 0; i < primeSize; i++)
    {
        printf("%d ", Aprime[i]);
    }
    printf("]\n");

    printf("El arreglo B' quedaria asi:\n B' = [ ");
    for (int i = 0; i < primeSize; i++)
    {
        printf("%d ", Bprime[i]);
    }
    printf("]\n");
    return result;
}

int binarySearch(int arr[], int size, int val, char operand)
{
    int left = 0, right = size - 1, mid, res = -1;
    while (left <= right)
    {
        mid = (left + right) / 2;
        bool conditional = false;
        if (operand == '>')
        {
            conditional = arr[mid] > val;
        }
        else
        {
            conditional = arr[mid] < val;
        }
        if (conditional)
        {
            res = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return res;
}

typedef struct Step2Element
{
    int num;
    int indx;
    char set;
} Step2Element;

struct Step2Element *step2(struct Step1Result *obj, int *N)
{
    printf("----- junta los pivotes -------\n");
    Step2Element *V = malloc((obj->primeSize * 2) * sizeof(Step2Element));
    int vIndex = 0;
    for (size_t i = 0; i < *N - 1; i++)
    {
        int index = binarySearch(obj->Bprime, *N, obj->Aprime[i], '>');
        vIndex = i;
        vIndex += index >= 0 ? index : *N;
        Step2Element temp = {obj->Aprime[i], i + 1, 'A'};
        V[vIndex] = temp;
    }
    for (size_t i = 0; i < *N - 1; i++)
    {
        int index = binarySearch(obj->Aprime, *N, obj->Bprime[i], '>');
        vIndex = i;
        vIndex += index >= 0 ? index : *N - 1;
        Step2Element temp = {obj->Bprime[i], i + 1, 'B'};
        V[vIndex] = temp;
    }
    printf("El arreglo V quedaria asi:\n V = [ ");
    for (size_t i = 0; i < obj->primeSize * 2; i++)
    {
        printf(" ( %d, %d, %c) ", V[i].num, V[i].indx, V[i].set);
    }
    printf(" ]\n");
    return V;
}

typedef struct Step3Element
{
    int Aidx;
    int Bidx;
} Step3Element;

Step3Element *step3(Step2Element *V, int *A, int *B, int r, int s, int N)
{
    printf("--------- entra a la definicion de los indices para el mergin de cada procesador ---------\n");
    Step3Element *Q = malloc(N * sizeof(Step3Element));
    Q[0] = (Step3Element){1, 1};
    for (size_t i = 2; i <= N; i++)
    {
        int vIndex = 2 * i - 2;
        if (V[vIndex - 1].set == 'A')
        {
            int index = binarySearch(B, s, V[vIndex - 1].num, '>');
            if (index < 0)
                index = s ;
            Q[i - 1] = (Step3Element){V[vIndex - 1].indx * ceil(r / N), index + 1};
        }
        else
        {
            int index = binarySearch(A, r, V[vIndex - 1].num, '>');
            if (index < 0)
                index = r ;
            Q[i - 1] = (Step3Element){index + 1, V[vIndex - 1].indx * ceil(s / N)};
        }
    }
    printf("El arreglo Q quedaria asi:\n Q = [ ");
    for (size_t i = 0; i < N; i++)
    {
        printf("( %d, %d )", Q[i].Aidx, Q[i].Bidx);
    }
    printf(" ]\n");
    return Q;
}

int* sort(Step3Element* Q, Step2Element* V,int* A, int* B, int r, int s, int N){
    printf("---------- entra al mergin --------------\n");
    int* C = malloc((r + s) * sizeof(int));

    for(int i = 1; i <= N; i++){
        int x = Q[i-1].Aidx ;
        int y = Q[i-1].Bidx ;
        int vIndex = (2*i) - 1 ;
        int printx = x;
        int printy = y;
        printf("++++ Al procesador P%d le corresponderia mezclar los siguientes elementos \n\t\t A%d = [ ", i,i);
        while( printx <= r){
            bool conditionalAB = i != N ? A[printx - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            printf(" %d", A[printx - 1]);
            printx++;
        }
        printf(" ]\n\ty en B los\n\t\t B%d = [ ", i);
        while(printy <= s){
            bool conditionalAB = i != N ? B[printy - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            printf(" %d", B[printy - 1]);
            printy++;
        }

        printf("]\n\tY hubiera generado :\n\t\t C%d = [ ", i);
        while( x<=r && y<=s ){
            int conditionalAB = i != N ? A[x-1] >= V[vIndex].num && B[y-1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            if(A[x-1] <= B[y-1]){
                C[x + y - 2] = A[x-1];
                printf(" %d", A[x-1]);
                x++;
            }else{
                C[x + y - 2] = B[y-1];
                printf(" %d", B[y-1]);
                y++;
            }
        }
        while(x<=r){
            int conditionalAB = i != N ? A[x - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            C[x + y - 2] = A[x - 1];
            printf(" %d", A[x - 1]);
            x++;
        }
        while(y<=s){
            int conditionalAB = i != N ? B[y - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            C[x + y - 2] = B[y - 1];
            printf(" %d", B[y - 1]);
            y++;
        }
        printf(" ]\n\n");

    }
    return C;
}
int main()
{
    /* consideraciones,
    1- en el codigo todo -1 que se vea a los indices
        se hace por que las operaciones del pseudocodigo devuelven numeros
        entre 1 y el tamaño del array en especifico
        y los arrays inician desde 0 hasta size-1
        por lo mismo en la formula x+y-1 se volvio x+y-2
    2- el binarysearch devuelve indices y no numero de elemento, por esto
        se le suma 1 cuando se va a guardar
    3-  en el mergeo de los arrays se usa un condicional para verificar
        si estamos en el ultimo procesador, en caso de que lo estemos
        mergeamos lo que queda de A y B, si estamos en otro procesador
        rompemos el ciclo cuando se encuentra un numero mayor o igual
        al pivote
    */
    int A[] = {2, 3, 4, 6, 11, 12, 13, 15, 16, 20, 22, 24};
    int B[] =  {1, 5, 7, 8, 9, 10, 14, 17, 18, 19, 21, 23};
    int N = 4;
    int r = 12;
    int s = 12;

    // Declare the step1 function prototype
    Step1Result primes = step1(A, B, &N, &r, &s);
    Step2Element* V = step2(&primes, &N);
    Step3Element* Q = step3(V, A, B, r, s, N);
    int* C = sort( Q, V, A, B, r, s, N);
    printf("tamano = %d  resultado C = [ ", r+s);
    for (size_t i = 0; i < (r+s); i++)
    {
        printf(" %d",C[i] );
    }
    printf("]\n----------------------- fin del programa ----------------------------");
    return 0;
};