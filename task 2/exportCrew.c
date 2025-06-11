#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
typedef struct Step1Result
{
    int *Aprime;
    int *Bprime;
    int primeSize;
} Step1Result;

struct Step1Result step1(int A[], int B[], int *N, int *r, int *s)
{
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
    return V;
}

typedef struct Step3Element
{
    int Aidx;
    int Bidx;
} Step3Element;

Step3Element *step3(Step2Element *V, int *A, int *B, int r, int s, int N)
{
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
    return Q;
}

int* sort(Step3Element* Q, Step2Element* V,int* A, int* B, int r, int s, int N){
    int* C = malloc((r + s) * sizeof(int));

    for(int i = 1; i <= N; i++){
        int x = Q[i-1].Aidx ;
        int y = Q[i-1].Bidx ;
        int vIndex = (2*i) - 1 ;
        int printx = x;
        int printy = y;
        while( printx <= r){
            bool conditionalAB = i != N ? A[printx - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            printx++;
        }
        while(printy <= s){
            bool conditionalAB = i != N ? B[printy - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            printy++;
        }

        while( x<=r && y<=s ){
            int conditionalAB = i != N ? A[x-1] >= V[vIndex].num && B[y-1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            if(A[x-1] <= B[y-1]){
                C[x + y - 2] = A[x-1];
                x++;
            }else{
                C[x + y - 2] = B[y-1];
                y++;
            }
        }
        while(x<=r){
            int conditionalAB = i != N ? A[x - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            C[x + y - 2] = A[x - 1];
            x++;
        }
        while(y<=s){
            int conditionalAB = i != N ? B[y - 1] >= V[vIndex].num : false;
            if(conditionalAB) break;
            C[x + y - 2] = B[y - 1];
            y++;
        }
        

    }
    return C;
}
int* crew_Merge(int A[], int B[], int r, int s, int N)
{
    // Ensure r, s, and N are positive and valid; user must provide correct sizes
    if (r <= 0 || s <= 0) {
        fprintf(stderr, "Error: Array sizes r and s must be positive.\n");
        return NULL;
    }
    if (N <= 0) {
        N = (int)log2((r + s) / 2);
        if (N <= 0) N = 1;
    }
    Step1Result primes = step1(A, B, &N, &r, &s);
    Step2Element* V = step2(&primes, &N);
    Step3Element* Q = step3(V, A, B, r, s, N);
    int* C = sort(Q, V, A, B, r, s, N);
    return C;
}