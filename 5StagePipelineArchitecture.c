#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instruction structure to represent each instruction
typedef struct Instruction {
    char* instructionText;  //  allocated string
    int* pipelineStages;    //  allocated array for stages
    struct Instruction* next;  // Pointer to next instruction
} Instruction;

// Global variables
Instruction* head = NULL; //linked list
int instructionCount = 0;

// Fncs
void enterInstructions();
void printPipelineChart();
void printCycleCount();
void menu();
void clearInstructions();
void calculatePipelineStages();
void addInstruction(const char* text);

void menu() {
    printf("\nPipelining:\n");
    printf("\n----------------------------------------------------------------\n");
    printf("1) Enter instructions\n");
    printf("2) Print a chart of the pipelined stages of the instructions\n");
    printf("3) Print the total cycle count for the program\n");
    printf("4) Clear all instructions\n");
    printf("5) Quit\n");
}

int main() {
    int choice;
    while (1) {
        menu();
        printf("\nEnter options: ");
        scanf("%d", &choice);
        getchar();  // Consume newline
        switch(choice) {
            case 1:
                enterInstructions();
                break;
            case 2:
                printPipelineChart();
                break;
            case 3:
                printCycleCount();
                break;
            case 4:
                clearInstructions();
                printf("\nAll instructions cleared.\n");
                break;
            case 5:
                clearInstructions();
                printf("\nExiting...\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
    return 0;
}

void addInstruction(const char* text) {
    // Creates new instruction node
    Instruction* newInstruction = malloc(sizeof(Instruction));
    if (newInstruction == NULL) {
        fprintf(stderr, "\nMemory allocation failed!\n");
        exit(1);
    }
    // instruction text copied
    (*newInstruction).instructionText = malloc(strlen(text) + 1);
    if ((*newInstruction).instructionText == NULL) {
        fprintf(stderr, "\nMemory allocation failed!\n");
        free(newInstruction);
        exit(1);
    }
    strcpy((*newInstruction).instructionText, text);
    // Initialize pipeline stages
    (*newInstruction).pipelineStages = malloc(5 * sizeof(int));
    if ((*newInstruction).pipelineStages == NULL) {
        fprintf(stderr, "\nMemory allocation failed!\n");
        free((*newInstruction).instructionText); //free memory
        free(newInstruction);
        exit(1);
    }
    // Link the new instruction
    (*newInstruction).next = NULL;
    // Add to the end of the list
    if (head == NULL) {
        head = newInstruction; // list empty? set as first 
    } else {
        Instruction* current = head;
        while ((*current).next != NULL) {
            current = (*current).next;
        }
        (*current).next = newInstruction;
    }
    instructionCount++;
}

void calculatePipelineStages() {
    Instruction* current = head;
    int instructionIndex = 0;

    while(current != NULL) {
        // First instruction starts at cycle 1
        // Subsequent instructions are staggered 
        (*current).pipelineStages[0] = instructionIndex + 1;   // IF 
        (*current).pipelineStages[1] = instructionIndex + 2;   // ID
        (*current).pipelineStages[2] = instructionIndex + 3;   // EX
        (*current).pipelineStages[3] = instructionIndex + 4;   // ME
        (*current).pipelineStages[4] = instructionIndex + 5;   // WB
        //learned "->" method = (.)
        current = (*current).next;
        instructionIndex++;
    }
}

void enterInstructions() {
    int num_to_add;
    printf("\nEx: (RX = RY +/- RZ)\n");
    printf("\nEnter amount of instructions to add: ");
    scanf("%d", &num_to_add);
    getchar();  // Consume newline
    for(int i = 0; i < num_to_add; i++) {
        char instructionText[256];
        printf("Instruction %d : ", instructionCount + 1);
        fgets(instructionText, sizeof(instructionText), stdin);
        // Remove newline if present
        instructionText[strcspn(instructionText, "\n")] = 0;
        // Add instruction to linked list
        addInstruction(instructionText);
    }
    // Recalculate pipeline stages
    calculatePipelineStages();
    printf("%d instructions added successfully!\n", num_to_add);
}
//to reset because i kept putting R#'s wrong
void clearInstructions() {
    Instruction* current = head;
    while(current != NULL) {
        Instruction* temp = current;
        current = (*current).next;
        free((*temp).instructionText);
        free((*temp).pipelineStages);
        free(temp);
    }
    head = NULL;
    instructionCount = 0;
}

void printPipelineChart() {
    if(instructionCount == 0) {
        printf("\nNo instructions entered. Please enter instructions first.\n");
        return;
    }
    printf("\nPipeline Stages:\n");
    Instruction* current = head;
    int instructionIndex = 0;
    //stages of instruxtions printed
    while (current != NULL) {
        for (int j = 0; j < instructionIndex * 4; j++) {
            printf(" ");
        }
        printf("|IF |ID |EX |ME |WB |\n");
        current = (*current).next;
        instructionIndex++;
    }
}

void printCycleCount() {
    if(instructionCount == 0) {
        printf("\nNo instructions entered. Please enter instructions first.\n");
        return;
    }
    //last instruction
    Instruction* current = head;
    while ((*current).next != NULL) {
        current = (*current).next;
    }
    //total cycles
    int total_cycles = (*current).pipelineStages[4];
    printf("\nThe total cycle count is %d\n", total_cycles);
}
/*
1) R0 =R1 +R2
2) R3=R1- R4
3) R2=R0+R3
4) R0=R2+R3
*/