#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./exportCrew.c"

#define N 8
#define P 4  // número de procesadores

int comparar_enteros(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);  // Equivalente a x - y pero seguro
}

void imprimir_arreglo(const char* msg, int A[], int n) {
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
        printf("%4d", A[i]);
    printf("\n");
}

int* merge(int* A, int* B) {
    int n1 = sizeof(A)/sizeof(int), n2 = sizeof(B)/sizeof(int);
    int* C = malloc( sizeof(int) * (n1 + n2));
    int i = 0, j = 0, k = 0;
    while (i < n1) C[k++] = A[i++];
    while (j < n2) C[k++] = B[j++];
    return C;
}

int main() {
    int S[N] = {25, 12, 7, 9, 18, 3, 30, 15};
    int block_size = N / P;
    int bloques[P][block_size];
    int merged1[N / 2];
    int final[N];

    int* Si[P][block_size];
    int* Pi[P][P]; // Para simular conjunto de procesadores

    printf("\n--- Step 1: División y Ordenamiento ---\n");
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < block_size; j++){
            bloques[i][j] = S[i * block_size + j];
        }
        // Copiar el bloque a Si[0][i]
        for (int j = 0; j < block_size; j++) {
            // Asignar memoria para cada bloque en Si[0][i] si es necesario
            if (j == 0) {
                Si[0][i] = malloc(block_size * sizeof(int));
            }
            Si[0][i][j] = bloques[i][j];
        }
        qsort(Si[0][i], block_size,sizeof(int), comparar_enteros );
        printf("Procesador %d ordena: ", i);
        Pi[0][i] = malloc(sizeof(int));
        *Pi[0][i] = i + 1;
        printf(" Pi = %d", Pi[0][i][0]);
        for (int j = 0; j < block_size; j++)
            printf("%4d", bloques[i][j]);
        printf("\n");
    }

    // Mostrar resultado del paso 1
    printf("--- Step 1 ---\n");
    for (int i = 0; i < P; i++) {
        printf("P%d -> ", i);
        for (int j = 0; j < block_size; j++) {
            printf("%4d", Si[0][i][j]);
        }
        printf("\n");
    }
    //mostrar el arreglo Pi
    for (int i = 0; i < P; i++) {
        printf("Pi %d\n",Pi[0][i][0]);
    }
    // Step 2
    int u = 1, v = P;
    int merged[N][N]; // Max almacenamiento
    for (int i = 0; i < P; i++)
        for (int j = 0; j < N; j++)
            merged[i][j] = Si[0][i][j]; // Copia inicial

    while (v > 1) {
        printf("\n--- Step 2 - Stage %d ---\n", u);
        int half  = ceil(v/2);
        for (int m = 1; m <= half; m++) {
            int left = 2 * m - 2;
            int right = 2 * m - 1;
            int* A = Pi[u-1][left];
            int* B = Pi[u-1][right];
            Pi[u][m-1]= merge( A, B );
            imprimir_arreglo("Pi",Pi[u][m-1],u);
            Si[u][m-1] = crew_Merge(Si[u-1][2*m-2], Si[u-1][2*m-1], block_size, block_size,-1);
            printf("Merged P%d and P%d -> ", left, right);
            printf("\n");
        }
        if (v % 2 != 0) {
            Pi[u][half] = Pi[u-1][v-1];
            Si[u][half] = Si[u-1][v-1];
        }
        block_size *= 2;
        v = half;
        u++;
    }
    return 0;
}