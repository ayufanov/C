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
#include <sys/wait.h>
#include <math.h>

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
    if (atoi(argv[2]) < 1){
        printf("Invalid argument 'process_num'\n");
        return 1;
    }

    int file = open(argv[1], O_RDONLY);

    if (argc != 3)
    {
        printf("%s\n", "invalid num of arguments");
        return 1;
    }

    int file_size = 0;
    int bytesRead = 1;

    while (bytesRead != 0)
    {
        char *r_buf = (char *)malloc(LEN);
        bytesRead = read(file, r_buf, LEN);
        file_size = file_size + bytesRead;
        if (bytesRead < 0)
        {
            perror("error while reading file");
            close(file);
            free(r_buf);
            return 2;
        }
        free(r_buf);
    }

    close(file);
    file = open(argv[1], O_RDONLY);
    char *str_file_arr = (char *)malloc(sizeof(char) * file_size);
    bytesRead = read(file, str_file_arr, file_size);
    close(file);

    int bytesWritten;
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
    free(str_file_arr);
    if (k < 2){
        printf("low number of values in file\n");
        return 1;
    }
    int n_processes = atoi(argv[2]);
    if (n_processes > k/2){
        printf("number of processes reduced to %d\n", k/2);
    }
    n_processes = k/2;
    int num_in_split = k / n_processes;

    char file_name[20];
    for (int i = 0; i < n_processes; i++)
    {
        sprintf(file_name, "%d", i);
        strcat(file_name, ".txt");
        FILE *file = fopen(file_name, "a+");
        if (file == 0)
        {
            perror("Error while creating or opening file");
            return 2;
        }

        if (i + 1 == n_processes)
        {

            for (j = 0; j < 1 + k - (n_processes - 1)*num_in_split; j++)
            {
                int idx = i * num_in_split + j;
                char dig[file_size];
                sprintf(dig, "%d", arr[idx]);
                strcat(dig, " ");
                bytesWritten = fprintf(file, dig);
                if (bytesWritten < 0)
                {
                    perror("Error while writing in file");
                    return 2;
                }
            }


        }
        else
        {
            for (j = 0; j < num_in_split; j++)
            {
                int idx = i * num_in_split + j;
                char dig[file_size];
                sprintf(dig, "%d", arr[idx]);
                strcat(dig, " ");
                bytesWritten = fprintf(file, dig);
                if (bytesWritten < 0)
                {
                    perror("Error while writing in file");
                    return 2;
                }
            }
        }

        fclose(file);
    }
    free(arr);
    free(buf);
    int *pids = (int *)malloc(sizeof(int) * (n_processes));

    for (int i = 0; i < n_processes; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("Error with creating process\n");
            return 1;
        }
        if (pids[i] == 0)
        {
            // child proc
            sprintf(file_name, "%d", i);
            strcat(file_name, ".txt");
            FILE *file = fopen(file_name, "r");
            char *str = (char *)malloc(sizeof(char) * file_size);
            fgets(str, file_size, file);
            
            if (bytesRead < 0)
            {
                perror("Error while reading file");
                return 2;
            }
            fclose(file);
            //printf("%s\n", str);
            int *dig_arr = strarr_to_int(str, bytesRead, file_size);
            float res = 0;
            for (int i = 0; i < sizeof(dig_arr); i++)
            {

                res += sqrt(dig_arr[i]);
            }
            FILE *file1 = fopen(file_name, "w");
            char dig[file_size];
            sprintf(dig, "%f", res);

            bytesWritten = fprintf(file1, dig);
            if (bytesWritten < 0)
            {
                perror("Error while writing in file");
                return 2;
            }
            free(str);
            free(dig_arr);
            exit(EXIT_SUCCESS);
            
            break;
        }
    }
    // MAIN
    getchar();
    for (int i = 0; i < n_processes; i++)
    {
        wait(NULL);
    }
    float result = 0;

    for (int i = 0; i < n_processes; i++)
        {
        sprintf(file_name, "%d", i);
        strcat(file_name, ".txt");
        FILE *file = fopen(file_name, "a+");
        if (file == 0)
        {
            perror("Error while creating or opening file");
            return 2;
        }
        char *str = (char *)malloc(sizeof(char) * file_size);
        fgets(str, file_size, file);
        result += atof(str);
        fclose(file);
        free(str);
        
    }
    
        printf("result --------- = %f\n", result);
    
        for (int i = 0; i < n_processes; i++){
            sprintf(file_name, "%d", i);
            strcat(file_name, ".txt");
            FILE *file = fopen(file_name, "w");

            fclose(file);

        }
        
    free(pids);
    return 0;
}
//gcc -Wall -g 2.c -lm