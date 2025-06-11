#include <stdio.h>
#include <math.h> // Para usar log2

// Prototipo de la función para imprimir el arreglo
void imprimir_arreglo(const char* titulo, int arr[], int n);

int main() {
    // Definición del arreglo A según el documento 
    // Se garantiza que n es una potencia de 2, con k > 2 
    int A_inicial[] = {5, 3, -6, 2, 7, 10, -2, 8};
    int n = sizeof(A_inicial) / sizeof(A_inicial[0]);
    int k = log2(n);

    // Arreglo de trabajo para no modificar el original
    int A[n];
    for (int i = 0; i < n; i++) {
        A[i] = A_inicial[i];
    }

    printf("************************************************************\n");
    printf("* Punto 1: Algoritmo p500_Calculo_Prefijo.pdf         *\n");
    printf("************************************************************\n\n");
    
    imprimir_arreglo("Arreglo original A:", A, n);
    printf("k = log2(%d) = %d\n\n", n, k);

    // =================================================================
    // 1. PRIMER RECORRIDO (Up-sweep) 
    // =================================================================
    printf("============================================================\n");
    printf("      RESUMEN PRIMER RECORRIDO (Hacia arriba)\n");
    printf("============================================================\n");

    // Bucle sobre los niveles del árbol, de abajo hacia arriba
    for (int d = 0; d < k; d++) {
        printf("\n--- PASO d = %d ---\n", d);
        
        // Este bucle simula la ejecución en paralelo. 
        // En un sistema paralelo real, cada procesador j que cumpla la condición
        // se ejecutaría al mismo tiempo. 
        printf("Cambios realizados en este paso:\n");
        int stride = 1 << d; // Equivale a 2^d

        for (int j = 0; j < n; j++) {
            if ((j + 1) % (1 << (d + 1)) == 0) {
                int left_index = j - stride;
                // Se indica qué elementos se usaron para el cálculo 
                printf("  A[%d] = A[%d] + A[%d] (valores antes: %d + %d)\n", j, j, left_index, A[j], A[left_index]);
                A[j] = A[j] + A[left_index];
            }
        }
        imprimir_arreglo("Estado del arreglo al final del paso:", A, n);
    }


    // =================================================================
    // 2. SEGUNDO RECORRIDO (Down-sweep) 
    // =================================================================
    printf("\n\n============================================================\n");
    printf("      RESUMEN SEGUNDO RECORRIDO (Hacia abajo)\n");
    printf("============================================================\n");

    printf("\nInicialmente, se asigna 0 al último elemento del arreglo.\n");
    A[n - 1] = 0;
    imprimir_arreglo("Arreglo antes de iniciar el recorrido hacia abajo:", A, n);

    // Bucle sobre los niveles del árbol, de arriba hacia abajo
    for (int d = k - 1; d >= 0; d--) {
        printf("\n--- PASO d = %d ---\n", d);

        // De nuevo, este bucle simula la ejecución en paralelo 
        printf("Cambios realizados en este paso:\n");
        int stride = 1 << d; // Equivale a 2^d

        for (int j = 0; j < n; j++) {
            if ((j + 1) % (1 << (d + 1)) == 0) {
                int right_index = j;
                int left_index = j - stride;

                // Guardamos los valores antes de la modificación para mostrarlos claramente 
                int temp = A[left_index];
                int val_right_antes = A[right_index];

                printf("  Intercambio y suma con A[%d] y A[%d]:\n", left_index, right_index);
                printf("    - A[%d] toma el valor de A[%d] (que era %d).\n", left_index, right_index, val_right_antes);
                printf("    - A[%d] toma la suma de los valores anteriores de A[%d] y A[%d] (%d + %d).\n", right_index, left_index, right_index, temp, val_right_antes);

                // Realizamos la operación de swap y suma
                A[left_index] = A[right_index];
                A[right_index] = temp + A[right_index];
            }
        }
        imprimir_arreglo("Estado del arreglo al final del paso:", A, n);
    }
    
    printf("\n\n************************************************************\n");
    printf("* FIN DE LA EJECUCIÓN                     *\n");
    printf("************************************************************\n");
    imprimir_arreglo("Arreglo final (Suma prefija exclusiva):", A, n);


    return 0;
}

/**
 * @brief Imprime el contenido de un arreglo de enteros de forma legible.
 * * @param titulo Un mensaje para imprimir antes del arreglo.
 * @param arr El arreglo a imprimir.
 * @param n El número de elementos en el arreglo.
 */
void imprimir_arreglo(const char* titulo, int arr[], int n) {
    printf("%s\n  [", titulo);
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}