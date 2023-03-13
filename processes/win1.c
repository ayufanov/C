#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#define LEN 5

void q_arr(char *a, int size)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = 'q';
    }
}

void array_print(int **A, int n, int m)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%*d", 4, A[i][j]);
        }
        printf("\n");
    }
}

int **create_array(int n, int m)
{

    int **A = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        A[i] = (int *)malloc(m * sizeof(int));
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            A[i][j] = 0;
    }
    return A;
}

void free_arr(int **A, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
    }
    free(A);
}

int *strarr_to_int(char *str_file_arr, int bytesRead, int file_size)
{
    int *arr = (int *)malloc(sizeof(int) * file_size); // массив интов
    char *buf = (char *)malloc(sizeof(char) * file_size);
    q_arr(buf, file_size);
    
    int k = 0; // кол-во элементов в массиве интов.
    int j = 0;
    for (int i = 0; i < bytesRead; i++)
    {

        if (isdigit(str_file_arr[i]))
        {
            buf[j] = str_file_arr[i];
            j++;
            if (i + 1 == bytesRead)
            {
                arr[k] = atoi(buf);
                k++;
            }
        }
        else
        {

            j = 0;
            arr[k] = atoi(buf);
            q_arr(buf, LEN);
            k++;
        }
    }
    free(buf);
    return arr;
}

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("Incorrect num of arguments\n");
        return 2;
    }
    int file_size = atoi(argv[1]);
    char filename[20];
    strcpy (filename, argv[0]);
    strcat(filename, ".txt");

    FILE *file = fopen(filename, "r");
        if (file == 0)
        {
            perror("Error while creating or opening file\n");
            return 2;
        }
    char *str = (char *)malloc(sizeof(char) * file_size);
    fgets(str, file_size, file);
    //printf("%s\n", str);
    fclose(file);
    int *dig_arr = strarr_to_int(str, file_size, file_size);
    float res = 0;
    //printf("--%d\n", dig_arr[1]);
    for (int i = 0; i < sizeof(dig_arr); i++)
        {
            res += sqrt(dig_arr[i]);
            //printf("res: %f\n", res);
        }
    //printf("proc_num = %s\n", argv[0]);
    //printf("-----------%f\n", res);
    //printf("dig 1 = %d\n", dig_arr[0]);
    //printf("dig 2 = %d\n", dig_arr[1]);
    free(str);
    free(dig_arr);
    FILE *file1 = fopen(filename, "w");
    char dig[file_size];
    sprintf(dig, "%f", res);
    int bytesWritten = fprintf(file1, dig);
            if (bytesWritten < 0)
            {
                perror("Error while writing in file");
                return 2;
            }
    //Sleep(300000);
    
    ExitProcess(1);
}
