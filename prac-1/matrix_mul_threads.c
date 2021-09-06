//To compile and run: gcc matrix_mul_threads.c -pthread -o matrix_mul_threads && ./matrix_mul_threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Matrix multiplier using Threads
 * 
 * @author Juan Sebastián Díaz Osorio - juansedo
 * @author Santiago Hidalgo Ocampo - sanhidalgoo
 */

int N;
int K;
int M;

int *matrix_A; // N, K
int *matrix_B; // K, M
int *matrix_C; // N, M

typedef struct {
    int tid;
    int i;
    int j;
    int iterator;
} Position;

void printTitle();

void *calculatePosition(void *pos)
{
    Position *p = (Position *) pos;
    int sum = 0;
    int i = p->i;
    int j = p->j;
    for(p->iterator = 0; p->iterator < K; p->iterator++){
        sum += *((matrix_A+i*K)+p->iterator) * *((matrix_B+p->iterator*M)+j);
    }
    *((matrix_C+i*M)+j) = sum;

    pthread_exit(NULL);
}

void printMatrix(int *mat, int rows, int cols) {
    int i, j;
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            printf("%d ", *((mat+i*cols)+j));
        }
        printf("\n");
    }
}

void fillMatrix(int *mat, int rows, int cols) {
    int i, j;
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            printf("Input value in (row, col) - (%i, %i): ", i+1, j+1);
            scanf("%d", (mat+i*cols)+j);
            getchar();
        }
    }
}

void autofillMatrix(int *mat, int rows, int cols) {
    int i, j;
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            *((mat+i*cols)+j) = (int)(rand() % 10) + 1;
        }
    }
}

//Function that runs the threads
int main(int argc, char *argv[])
{
    printTitle();

    int Kaux;

    printf("This program multiplies two matrix (A and B) and print the result (C).\n");
    printf("You can give the width and height of A and B matrix. The values will be selected randomly.\n");
    printf("Please, input a number for rows in A: ");
    scanf("%d", &N);
    getchar();
    printf("Please, input a number for columns in A: ");
    scanf("%d", &K);
    getchar();
    printf("Please, input a number for rows in B: ");
    scanf("%d", &Kaux);
    getchar();
    if (K != Kaux) {
        printf("ERROR; A columns and B rows must be equal.\n\n");
        exit(EXIT_FAILURE);
    }
    printf("Please, input a number for columns in B: ");
    scanf("%d", &M);
    getchar();

    pthread_t threads[N*M];
    int rc, i, j, k;
    long th;
    Position pos_mat_C[N*M];

    // Setting random seed
    time_t t;
    clock_t init_time, final_time;
    srand((unsigned) time(&t));

    matrix_A = (int *) malloc(N * K * sizeof(int));
    matrix_B = (int *) malloc(K * M * sizeof(int));
    matrix_C = (int *) malloc(N * M * sizeof(int));

    // Fill matrix
    char op;
    printf("\n\nA Matrix\n");
    printf("Do you want to fill matrix automatically? (Y/n): ");
    op = (char) getchar();
    if (op == 'n') fillMatrix(matrix_A, N, K);
    else autofillMatrix(matrix_A, N, K);

    printf("\n\nB Matrix\n");
    printf("Do you want to fill matrix automatically? (Y/n): ");
    op = (char) getchar();
    if (op == 'n') fillMatrix(matrix_B, N, K);
    else autofillMatrix(matrix_B, N, K);

    init_time = clock();
    // Print Matrix A
    printf("\n\nA Matrix\n");
    printMatrix(matrix_A, N, K);
    
    // Print Matrix B
    printf("\n\nB Matrix\n");
    printMatrix(matrix_B, K, M);
    
    th = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            pos_mat_C[th].tid = th;
            pos_mat_C[th].i = i;
            pos_mat_C[th].j = j;
        
            rc = pthread_create(&threads[th], NULL, (void *)calculatePosition, (void *)&pos_mat_C[th]);
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(EXIT_FAILURE);
            }
            th++;
        }
    }
    
    for(i = 0; i < N*M; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\nMatriz C:\n");
    printMatrix(matrix_C, N, M);
    final_time = clock() - init_time;

    printf("\n\nDuration: %ld ms\n", final_time * 1000 / CLOCKS_PER_SEC);

    pthread_exit(NULL);
    return 0;
}

void printTitle() {
    puts(
    "  _  _       _ _   _       _       \n"
    " |  \\/  |     | | | (_)     | |      \n"
    " | \\  / |_   | | | _ _ _ | |   _ \n"
    " | |\\/| | | | | | _| | ' \\| | | | |\n"
    " | |  | | || | | || | |) | | || |\n"
    " ||  ||\\_,||\\|| ._/||\\__, |\n"
    "                      | |       __/ |\n"
    "  _  _       _      ||      |__/ \n"
    " |  \\/  |     | |      (_)           \n"
    " | \\  / | _ _| | _ _ __  __      \n"
    " | |\\/| |/ ` | _| '__| \\ \\/ /      \n"
    " | |  | | (| | || |  | |>  <       \n"
    " ||  ||\\_,|\\_||  |//\\_\\      \n\n"
    "_  _  _ _    _ _  _    _ _  _ __ __ __ _  __ \n"
    "|_] |  |  |     | |\\ |     |  || |/ |_ || |  \\ [_  \n"
    "|_] ||  |     | | \\|     |  |  | |  \\ |_ |  | |/ __] \n\n\n"
    );
}