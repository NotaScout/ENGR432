#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000


// Function to generate a random matrix
void generateRandomMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 100; // Generates a random number between 0 and 99
        }
    }
}

// Function to multiply two matrices
void multiplyMatrices(int mat1[SIZE][SIZE], int mat2[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Function to print a matrix
void printMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int matrix1[SIZE][SIZE];
    int matrix2[SIZE][SIZE];
    int result[SIZE][SIZE];

    // Seed the random number generator
    srand(time(NULL));

    // Generate random matrices
    generateRandomMatrix(matrix1);
    generateRandomMatrix(matrix2);

    // Multiply the matrices
    multiplyMatrices(matrix1, matrix2, result);

    // Print the result
    // For such a large matrix, printing might take some time and may not be feasible in some environments.
    // Consider redirecting the output to a file for analysis.
    printMatrix(result);
	
	//scanf();

    return 0;
}
