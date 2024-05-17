#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#define PIPE "ServerPipe"
#define PID_FILE "jobExecutorServer.txt"


void signal_handler(int signum){
    if (signum==SIGUSR2)
    {
        printf("JobExecutorServer Terminated\n");
    }
    
}

const char *message;

int main(int argc, char* argv[]) {
    signal(SIGUSR2, signal_handler);

    pid_t pid;    
    int pid_fd = open(PID_FILE, O_RDONLY);
    
    if(pid_fd!=-1){
        printf("File does exist\n");
        close(pid_fd);
        sleep(1);
    } 
    else {
        printf("File does not exist\n");
        
        pid=fork();
        if (pid==-1) {
            perror("fork");
        }
        else if(pid==0){
            printf("Me child\n");
            execl("./bin/jobExecutorServer","jobExecutorServer" ,  NULL);
        }
        else{ 
            sleep(1);
        }
    }
    
    
    pid_fd = open(PID_FILE, O_RDONLY);
    if (pid_fd == -1) {
        perror("open pid file");
        exit(EXIT_FAILURE);
    }
    char pid_buffer[20];
    ssize_t num_read = read(pid_fd, pid_buffer, sizeof(pid_buffer) - 1);
    if (num_read == -1) {
        perror("read pid file");
        close(pid_fd);
        exit(EXIT_FAILURE);
    }

   
    pid_buffer[num_read] = '\0';
    close(pid_fd);

    pid_t reader_pid = atoi(pid_buffer);
    if (reader_pid == 0) {
        fprintf(stderr, "Invalid PID read from file\n");
        exit(EXIT_FAILURE);
    }
        int fd = open(PIPE, O_WRONLY);
        printf("pipe oppened from user side\n");
        if (fd == -1) {
            perror("open");
            exit(1);
        }      
    size_t total_length = 0;
    for (int i = 1; i < argc; i++) {
        total_length += strlen(argv[i]);
    }

 
    char *message = (char *)malloc(total_length + argc);  

    if (message == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    
    message[0] = '\0';  
    for (int i = 1; i < argc; i++) {
        strcat(message, argv[i]);
        if (i < argc - 1) {
            strcat(message, " ");  
        }
    };
    size_t message_len = strlen(message);
    char *message_with_newline = malloc(message_len + 2);
    if (!message_with_newline) {
        perror("malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

  
    strcpy(message_with_newline, message);
    message_with_newline[message_len] = '\n';
    message_with_newline[message_len + 1] = '\0';

    
    if (write(fd, message_with_newline, message_len + 2) == -1) {
        perror("write");
        free(message_with_newline);
        close(fd);
        exit(EXIT_FAILURE);
    }

    free(message_with_newline);
    close(fd);
    if (kill(reader_pid, SIGUSR1) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    
        
    return 0;
}