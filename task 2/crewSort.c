#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./exportCrew.c"

#define N 12
#define P 3  // número de procesadores

struct arrSi {
    int *arr;
    int size;
};

struct maxLvl {
    int i;
    int j;
};

int comparar_enteros(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);  // Equivalente a x - y pero seguro
}

void imprimir_arreglo(const char* msg, int A[], int n) {
    printf("%s", msg);
    for (int i = 0; i < n; i++)
        printf("%4d", A[i]);
    printf("\n");
}

struct arrSi merge_arrSi(struct arrSi A, struct arrSi B) {
    struct arrSi C;
    C.size = A.size + B.size;
    C.arr = malloc(sizeof(int) * C.size);
    int i = 0, j = 0, k = 0;
    while (i < A.size) C.arr[k++] = A.arr[i++];
    while (j < B.size) C.arr[k++] = B.arr[j++];
    return C;
}

int main() {
    int S[N] = {15, 3, 20, 8, 5, 17, 12, 9, 30, 2, 25, 10};
    int block_size = N / P;
    int bloques[P][block_size];
    struct maxLvl maxLevel;
    int size = log2(N) + 1;
    struct arrSi Si[size][P];  // para evitar sobrescribir niveles previos
    int* Pi[size][P];

    printf("\n========== Paso 1: Division ==========\n");
    int left = 0;
    for (int i = 0; i < P; i++) {
        int tempSize = i == P-1 ? N - block_size* (P-1) : block_size;
        printf("Bloque %d (ancho %d): ", i+1, tempSize);
        for (int j = 0; j < tempSize; j++) {
            bloques[i][j] = S[left + j];
            printf("%4d", bloques[i][j]);
        }
        printf("\n");
        left += tempSize;
        Si[0][i].arr = malloc(tempSize * sizeof(int));
        Si[0][i].size = tempSize;
        for (int j = 0; j < tempSize; j++) {
            Si[0][i].arr[j] = bloques[i][j];
        }
        qsort(Si[0][i].arr, Si[0][i].size, sizeof(int), comparar_enteros );
        Pi[0][i] = malloc(sizeof(int));
        Pi[0][i][0] = i + 1;
    }

    printf("\n========== Paso 1: Ordenamiento ==========\n");
    for (int i = 0; i < P; i++) {
        printf("Procesador %d ordena: ", i+1);
        imprimir_arreglo("", Si[0][i].arr, Si[0][i].size);
    }
    printf("\nConjunto 0 de Pi: ");
    for (int i = 0; i < P; i++) {
        printf("%2d", Pi[0][i][0]);
    }
    printf("\n");

    // Step 2
    int u = 1, v = P;
    int curr_block_size = block_size;
    while (v > 1) {
        printf("\n========== Paso 2 - Etapa %d ==========\n", u);
        int hceil  = ceil((float)v/2.0);
        int hfloor = v/2;
        for (int m = 1; m <= hfloor; m++) {
            int left = 2 * m - 2;
            int right = 2 * m - 1;
            int* A = Pi[u-1][left];
            int* B = Pi[u-1][right];

            printf("\n--- Fusionando bloques %d y %d ---\n", left+1, right+1);
            printf("Pi izquierdo: ");
            imprimir_arreglo("", A, u);
            printf("Si izquierdo: ");
            imprimir_arreglo("", Si[u-1][left].arr, Si[u-1][left].size);
            printf("Pi derecho:  ");
            imprimir_arreglo("", B, u);
            printf("Si derecho:  ");
            imprimir_arreglo("", Si[u-1][right].arr, Si[u-1][right].size);

            // Merge Pi
            struct arrSi mergedPi = merge_arrSi(
                (struct arrSi){A, u},
                (struct arrSi){B, u}
            );
            Pi[u][m-1] = mergedPi.arr;

            // Merge Si
            int *tempArr = crew_Merge(
                Si[u-1][left].arr, Si[u-1][right].arr,
                Si[u-1][left].size, Si[u-1][right].size, 2
            );
            Si[u][m-1].arr = tempArr;
            Si[u][m-1].size = Si[u-1][left].size + Si[u-1][right].size;

            printf("Resultado de la fusion (Si): ");
            imprimir_arreglo("", Si[u][m-1].arr, Si[u][m-1].size);

            if (!(hceil > 1)) maxLevel = (struct maxLvl){u, m-1};
        }
        if (v % 2 != 0) {
            printf("\n--- Bloque impar pasa sin fusionar ---\n");
            Pi[u][hceil-1] = Pi[u-1][v-1];
            Si[u][hceil-1] = Si[u-1][v-1];
            printf("Pi impar: ");
            imprimir_arreglo("", Pi[u][hceil-1], u);
            printf("Si impar: ");
            imprimir_arreglo("", Si[u][hceil-1].arr, Si[u][hceil-1].size);
        }
        curr_block_size *= 2;
        v = hceil;
        u = u+1;
    }
    
    printf("\n========== Resultado final ==========\n");
    imprimir_arreglo("Arreglo final ordenado: ", Si[maxLevel.i][maxLevel.j].arr, N);

    // Liberar memoria
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < P; j++) {
            if (Si[i][j].arr) free(Si[i][j].arr);
            if (Pi[i][j]) free(Pi[i][j]);
        }
    }
    return 0;
}
