#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100


// Majd Hasan Khader Maree
// 1192226
// Section 2

// defining a pointer to the stack
typedef struct node* ptr;
typedef ptr Stack;
// creating the stack structure
struct node
{
    // defining the content of the stack ( string ) and pointer (next)
    char data[MAX];
    ptr Next;
};
// a function that checks if the stack is empty
int isEmpty(Stack st)
{
    return st->Next == NULL;
}
// a pop function that removes the deletes element from the stack
void pop(Stack st)
{
    ptr firstCell;
    if(isEmpty(st))
        printf("Empty stack");
    else
    {
        firstCell=st->Next;
        st->Next=st->Next->Next;
        free(firstCell);
    }
}

// a function that creates the stack
Stack createStack()
{
    Stack st;
    st = (Stack) malloc(sizeof(struct node));
    if(st==NULL)
        printf("Out of space!");
    st->Next =NULL;
    return st;
}
// a string that is equal to the last element of the stack
char * top(Stack st)
{
    if(!isEmpty(st))
        return st->Next->data;
    printf("Empty stack");
    return 0;
}
// a function that inserts a new string to the stack
void push(char X[], Stack st )
{
    ptr temp;
    temp = (Stack)malloc( sizeof( struct node ) );
    if(temp == NULL)
        printf("Out of space!");
    else{
        strcpy(temp->data,X);
        temp->Next = st->Next;
        st->Next = temp;
    }
}
// a function that prints the stack in a recursive way ( from bottom to top )
void printS(Stack st)
{
    if(isEmpty(st))
        return;
    char temp[MAX];
    strcpy(temp,top(st));
    pop(st);
    printS(st);
    printf("%s \n", temp);
    push(temp, st);
}
// creating a FILE variable called output for the output file
FILE *out;
void saveS(Stack st)
{
    if(isEmpty(st))
        return;
    char temp[MAX];
    strcpy(temp,top(st));
    pop(st);
    saveS(st);
    fprintf(out, "%s \n", temp);
    push(temp, st);
}

int main()
{
    // opening the file output and setting it as reading file.
    out = fopen("output", "w");
    printf("MyCommand > ");
    // creating a string to store the string that the user enters
    char s[MAX];
    gets(s);
    // checking if the first string he enters is an operation so the program shows a warning the he has to enter data before doing any operation
    while(strcmp("print", s)==0 || strcmp("undo", s)==0 || strcmp("redo", s)==0 || strcmp("save", s)==0){
        printf("Please enter data before doing any operation \nMyCommand > ");
        gets(s);
    }
    // creating a temporarily string called temp to save the top string in it when doing undo
    char temp[MAX];
    Stack st= createStack();
    // setting two flags to make sure that the user does not do redo operation before he does undo operation and to make sure he does not save the same data twice in a row
    int ok=0, save=1;
    // if the string is equal to quit then the program quits and ends.
    while(strcmp("quit", s)!=0){
        // print operation
        if(strcmp("print", s)==0){
            printf("\nresult > \n");
            // calling the function that prints the whole stack
            printS(st);
        }
        // undo operation
        else if(strcmp("undo", s)==0){
            // saving the last string in the stack to the temp string so we can push it back again in redo operation
            strcpy(temp,top(st));
            ok=1;
            pop(st);
            printf("\nresult > \n");
            printS(st);
            save=1;
        }
        // redo operation
        else if(strcmp("redo", s)==0){
            // checking if the undo operation happened before the redo or not
            if(ok){
                // pushing the temp string that is equal to the string we popped in undo operation
                push(temp, st);
                printf("\nresult > \n");
                printS(st);
            }
            else{
                printf("Please make \"undo\" operation before doing \"redo\"");
            }
            ok=0;
            save=1;
        }
        // saving operation
        else if(strcmp("save", s)==0){
            if(save){
                fprintf(out, "\nresult > \n");
                // calling the function saveS that saves the data in the output.txt file
                saveS(st);
                save=0;
            }
            else{
                printf("You have already saved the data");
            }

        }
        // if the string entered was none of the operation then it's considered as data and it's pushed to the stack
        else{
            push(s,st);
            save=1;
        }
        // letting the user enters another string
        printf("\nMyCommand > ");
        gets(s);
    }
    // saving the data after the user ends the program
    if(save){
        fprintf(out, "\nresult > \n");
        saveS(st);
        save=0;
    }
    printf("result > GGGGGGood bye");
    return 0;
}
