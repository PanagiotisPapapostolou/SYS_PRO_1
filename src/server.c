#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "string_array.h"
#define PIPE "ServerPipe"
#define PID_FILE "jobExecutorServer.txt"

volatile sig_atomic_t writer_pid = 0;

void signal_handler(int signum, siginfo_t *info, void *context) {
    writer_pid = info->si_pid;  // Capture the PID of the writer process
}



int main() {
    
    int concurnecy=1;
    int fd=open(PID_FILE, O_CREAT|O_WRONLY|O_RDONLY,0664);
    if(fd==-1){
        perror("fopen");
        printf("File was not created");
        exit(1);
    }
   
    dprintf(fd, "%d\n" ,getpid());
    close(fd);
    if (mkfifo(PIPE, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    } 
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_handler;
    sigemptyset(&sa.sa_mask);


    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    //Open the named pipe for reading
    fd = open(PIPE, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("Pipe oppened form server side\n");
    char buff[20];
    while (1) {
        ssize_t num_read = read(fd, buff, sizeof(buff));
        if (num_read > 0) {
           
            
            buff[num_read] = '\0'; // Null-terminate the string


            char *newline = strchr(buff, '\n');
            if (newline != NULL) {
                *newline = '\0';
            }
            printf("Debug: Received message '%s' from PID %d\n", buff, writer_pid);
            StringArray* command=string_array_init();
            string_array_add(command, buff, " ");

            if (strcmp(command->data[0], "exit")==0) {   
                if (remove(PID_FILE) != 0) {
                    perror("Error deleting file");
                }
                
                close(fd);
                unlink(PIPE);
               
                if (kill(writer_pid, SIGUSR2) == -1) {
                    perror("kill");
                }
                exit(1);
            }
            if (strcmp(command->data[0], "SetConcurency")==0)
            {
                concurnecy=atoi(command->data[1]);
                printf("Concurency set to %d\n", concurnecy);
            }
            
            
        }
        
        pause();
    }
    close(fd);
    return 0;
}