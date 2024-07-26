// Змейка. 
// Для заданных чисел M и N создать матрицу по спирали с числами от 1 до M * N
// https://stepik.org/lesson/64657/step/11?unit=41549
#include <stdio.h>

void printSpiralMatrix(int M, int N, int matrix[M][N]) {
    int left = 0, right = N - 1, top = 0, bottom = M - 1, direction = 0, num = 1;

    while (num <= M * N) {
        if (direction == 0) {
            for (int i = left; i <= right; i++) {
                matrix[top][i] = num++;
            }
            top++;
            direction = 1;
        } else if (direction == 1) {
            for (int i = top; i <= bottom; i++) {
                matrix[i][right] = num++;
            }
            right--;
            direction = 2;
        } else if (direction == 2) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = num++;
            }
            bottom--;
            direction = 3;
        } else if (direction == 3) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = num++;
            }
            left++;
            direction = 0;
        }
    }
}

void printMatrix(int M, int N, int matrix[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int M = 8, N = 9;
    int matrix[M][N];

    printSpiralMatrix(M, N, matrix);
    printMatrix(M, N, matrix);

    return 0;
}
// Добавил изменения
