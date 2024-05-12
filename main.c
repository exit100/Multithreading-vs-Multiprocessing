#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void printOptions();
void printTitle();

void handleSignal(int sig){
   printf("\033[91mReceived a signal to exit. Exiting gracefully.\n");
    printf("\033[0m");
   exit(0);
}
void handleSigint(int sig){
    system("clear");
    exit(0);
}

int main() {
    int choice=-1;
    signal(SIGUSR1,handleSignal);
    signal(SIGINT,handleSigint);

while(1){
    printOptions();
    int ch=-1;
    // Prompt user for choice
    printf("\033[92mEnter your choice (1-4): ");
    printf("\033[0m");
    scanf("%d", &choice);

    // Perform action based on user input

    
    switch (choice) {
        case 1:
            printf("Enter (n), the dimensions of the nxn matrix: ");
            char inputs[10];
            scanf("%s",inputs);
           // fgets(inputs,10,stdin);
            char cmd[100]="./multiplication/process ";
            char cmd2[100]="./multiplication/threads ";
            strcat(cmd,inputs);
            strcat(cmd2,inputs);

            system(cmd); // Replace "arg1 arg2" with your actual arguments
            system(cmd2); // Replace "arg1 arg2" with your actual arguments
            break;
        case 2:

            system("./search/process"); // Replace "arg1 arg2 arg3" with your actual arguments
            system("./search/threads"); // Replace "arg1 arg2 arg3" with your actual arguments
            break;
        case 3:

            printf("Enter (n), the size of array. (Max 1024): ");
            char inputsMerge[10];
            scanf("%s",inputsMerge);

            char cmdMerge[100]="./merge/process ";
            char cmd2Merge[100]="./merge/threads ";
            strcat(cmdMerge,inputsMerge);
            strcat(cmd2Merge,inputsMerge);

            system(cmdMerge); // Replace "arg1 arg2" with your actual arguments
            system(cmd2Merge); // Replace "arg1 arg2" with your actual arguments

            break;
        case 4:
            printf("Exiting program.\n");
            exit(0);
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            goto afterwhile;
            break;
    }
    printf("\033[92mPress 1 to go back to menu, press 2 to exit the program: ");
    printf("\033[0m");
    scanf("%d",&ch);
    if(ch==2)  
        kill(getpid(),SIGUSR1);
    else {
        system("clear");
    }    
    afterwhile:
}

    return 0;
}

void printOptions(){
    printf("\033[92m");
    printTitle();
    printf("\033[94mWelcome to the Matrix Multiplication and Sorting Project!\n");
    printf("Choose an option:\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Linear Search\n");
    printf("3. Merge Sort\n");
    printf("4. Exit\n");
    printf("\033[0m");
}

void printTitle(){
    printf(" _____ _____   ______          _           _   \n");
    printf("|  _  /  ___|  | ___ \\        (_)         | |  \n");
    printf("| | | \\ `--.   | |_/ / __ ___  _  ___  ___| |_ \n");
    printf("| | | |`--. \\  |  __/ '__/ _ \\| |/ _ \\/ __| __|\n");
    printf("\\ \\_/ /\\__/ /  | |  | | | (_) | |  __/ (__| |_ \n");
    printf(" \\___/\\____/   \\_|  |_|  \\___/| |\\___|\\___|\\__|\n");
    printf("                             _/ |              \n");
    printf("                            |__/               \n");
}
