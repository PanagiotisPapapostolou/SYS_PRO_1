typedef struct triplet{
    char* JobID;
    char* job;
    int pos;
}Triplet;



typedef struct queue {
    Queue* head;
    Queue* next;
    Queue* tail;
    Triplet* data;
}Queue;
