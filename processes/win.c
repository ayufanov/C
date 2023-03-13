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
        LPSTR message;
        DWORD dwMesageLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                            NULL, 
                                            GetLastError(), 
                                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                            (LPSTR) &message,
                                            0,
                                            NULL);
        printf("Incorrect argument 'process number'\n%s", message);
        return -1;
    }

    HANDLE file1 = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (file1 == INVALID_HANDLE_VALUE){
        LPSTR message;
        DWORD dwMesageLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                            NULL, 
                                            GetLastError(), 
                                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                            (LPSTR) &message,
                                            0,
                                            NULL);
        printf("Source file Open Error:\n%s", message);
        HeapFree(GetProcessHeap(), 0, message);
        return -1;}

    if (argc != 3)
    {
        LPSTR message;
        DWORD dwMesageLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                            NULL, 
                                            GetLastError(), 
                                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                            (LPSTR) &message,
                                            0,
                                            NULL);
        printf("Invalid number of arguments:\n%s", message);
        return -1;
    }

    DWORD dwBytesRead = 1;
    DWORD dwBytesWritten = 0;
    int file_size = 0;


 while(TRUE){
        LPSTR read_buf = HeapAlloc(GetProcessHeap(), 0, LEN);
        
        if (!ReadFile(file1, read_buf, LEN, &dwBytesRead, NULL)){
            printf("Source file Read Error: %d\n", GetLastError());
            CloseHandle(file1);
            HeapFree(GetProcessHeap(), 0, read_buf);
            return 1;
        }
        file_size += dwBytesRead;
        if (dwBytesRead == 0){
            HeapFree(GetProcessHeap(), 0, read_buf);
            break;
        }
        
        HeapFree(GetProcessHeap(), 0, read_buf);
    }
    CloseHandle(file1);
    
    int bytesRead = file_size;
    FILE *file = fopen(argv[1], "r+");
    if (file == 0)
        {
            perror("Error while creating or opening file");
            return 2;
        }
    char *str_file_arr = (char *)malloc(sizeof(char) * file_size);
    if (fgets(str_file_arr, file_size, file) == NULL){
        printf("Error while reading file\n");
        return 2;
    }
    fclose(file);
    
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
    
    //printf("k = %d\n", k);
    if (k < 2){
        printf("low number of values in file\n");
        return 1;
    }
    int n_processes = atoi(argv[2]);
    printf("%d\n", n_processes);
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

            for (j = 0; j < k - (n_processes - 1)*num_in_split; j++)
            {
                int idx = i * num_in_split + j;
                char dig[file_size];
                sprintf(dig, "%d", arr[idx]);
                strcat(dig, " ");
                if (i == 2){
           
                }
               
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
    
    STARTUPINFO sti = {0};
    PROCESS_INFORMATION pi = {0};
    HANDLE* pis = malloc(sizeof(HANDLE) * n_processes);

    for (int i = 0; i < n_processes; i++){
        char * cmd = malloc (sizeof(char) * file_size);
        char * cmd1 = malloc (sizeof(char) * file_size);

        sprintf(cmd, "%d", i);
        strcat(cmd, " ");
        sprintf(cmd1, "%d", file_size);
        strcat(cmd, cmd1);
        
        if (!CreateProcess("po.exe",
                            cmd,
                            NULL,
                            NULL,
                            TRUE,
                            0,
                            NULL,
                            NULL,
                            &sti,
                            &pi

        )){
            printf("Error while creating process");
            free(cmd);
            free(cmd1);
            return 1;
        }
        pis[i] = pi.hProcess;

        free(cmd);
        free(cmd1);
   
    }
    
    WaitForMultipleObjects(n_processes, pis, TRUE, INFINITE);
           
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
        
        FILE *file1 = fopen(file_name, "w");
        fclose(file1);
        free(str);
        
    }
    printf("  result --------- = %f\n", result);
    
        
    // free
    return 0;
  }
