#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

// Function to generate random integers
int generateRandomInt() {
    return rand() % 100; // Generates random integers between 0 and 99
}

// Function to print a matrix (prints only first few elements of each row)
void printMatrix(int** matrix) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < 1000; j++) { // Print only the first 10 elements of each row
            printf("%d\t", matrix[i][j]);
        }
        //printf("..."); // Indicate that the row continues
		// FULL FAT
		printf("Row %d\n",i+1);
        printf("\n");
		printf("\n");
		printf("\n");
    }
    fflush(stdout); // Flush the output buffer
}

// Function to multiply two matrices
void multiplyMatrices(int** matrix1, int** matrix2, int** result) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main() {
    // Initialize random seed
    srand(time(NULL));

    // Allocate memory for matrices
    int** matrix1 = (int**)malloc(SIZE * sizeof(int*));
    int** matrix2 = (int**)malloc(SIZE * sizeof(int*));
    int** result = (int**)malloc(SIZE * sizeof(int*));
    for (int i = 0; i < SIZE; i++) {
        matrix1[i] = (int*)malloc(SIZE * sizeof(int));
        matrix2[i] = (int*)malloc(SIZE * sizeof(int));
        result[i] = (int*)malloc(SIZE * sizeof(int));
    }

    // Populate matrices with random integers
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix1[i][j] = generateRandomInt();
            matrix2[i][j] = generateRandomInt();
        }
    }

    // Print matrices
    printf("Matrix 1:\n");
    printMatrix(matrix1);
    printf("\nMatrix 2:\n");
    printMatrix(matrix2);

    // Multiply matrices
    printf("Multiplying matrices...\n");
    multiplyMatrices(matrix1, matrix2, result);
    printf("\nResult:\n");
    printMatrix(result);
    // Print result (uncomment if needed)
    //printf("\nResult:\n");
    //printMatrix(result);

    // Free dynamically allocated memory
    for (int i = 0; i < SIZE; i++) {
        free(matrix1[i]);
        free(matrix2[i]);
        free(result[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
