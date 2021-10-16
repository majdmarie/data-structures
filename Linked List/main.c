#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 1000


// Majd Hasan Khader Maree
// 1192226
// Section 2

FILE *input, *output;
int z,mx;

struct Node;
typedef struct Node * ptr;
typedef ptr List;
typedef ptr Position;

// creating the linked list structure
struct Node{
    // defining the linked list variables and pointers ( next and prev ) because it's a doubly linked list
    float co;
    int pow;
    ptr next;
    ptr prev;
};


// creating a function that prints the linked list as an equation
void printList (List L)
{
    Position P = L;
    int first=1;
    while(P->next!=NULL){
        P=P->next;
        if(P->co==0)
            continue;
        if(first) first=0;
        else if(P->co>0){
            printf("+");
        }
        if(P->pow==0){
            printf("%.1f", P->co);
            continue;
        }
        if(P->co!=1)
            printf("%.1f", P->co);
        printf("x");
        if(P->pow!=1)
            printf("^%d", P->pow);
    }
    printf("\n");
}

// a function that inserts a new node to the linked list
void insert(List L,Position P, float x, int y)
{
    Position temp;
    temp = (Position) malloc(sizeof(struct Node));
    if(temp!=NULL){
        temp -> co = x;
        temp -> pow = y;
        temp -> next = P -> next;
        P ->next = temp;
        temp -> prev = P;
    }
    else{
        printf("sorry the memory is full");
    }

}
// create a function to delete the linked list
void deleteList(List L){
    Position P, temp;
    P = L->next;
    L->next = NULL;
    while(P != NULL){
        temp = P->next;
        free(P);
        P=temp;
    }
}
// creating a frequency array to use it later
float a[1000];

// creating a function to print the equation in its simplest expression
void printEq(){
    int first=1;
    for(int i=0; i<=mx; i++){
        if(a[i]==0)
            continue;
        if(first) first=0;
        else if(a[i]>0){
            printf("+");
        }
        if(i==0){
            printf("%.1f", a[i]);
            continue;
        }
        if(a[i]!=1)
            printf("%.1f", a[i]);
        printf("x");
        if(i!=1)
            printf("^%d", i);
    }
    printf("\n");
}
// creating a function to save the equation in results.txt output file
void printOnFile(){
    int first=1;
    for(int i=0; i<=mx; i++){
        if(a[i]==0)
            continue;
        if(first) first=0;
        else if(a[i]>0){
            fprintf(output,"+");
        }
        if(i==0){
            fprintf(output,"%.1f", a[i]);
            continue;
        }
        if(a[i]!=1)
            fprintf(output,"%.1f", a[i]);
        fprintf(output,"x");
        if(i!=1)
            fprintf(output,"^%d", i);
    }
    fprintf(output,"\n");
}
// creating a function to do the addition equations operation
void add(List lists[z]){
    memset(a,0,sizeof(a));
    mx=0;
    for(int i=0; i<z; i++){
        Position P = lists[i];
        while(P->next!=NULL){
            P=P->next;
            a[P->pow]+=(P->co);
            if((P->pow)> mx)
                mx=(P->pow);
        }
    }
    fprintf(output,"Result of addition: ");
}
// creating a function to do the subtraction equations operation
void sub(List lists[z]){
    memset(a,0,sizeof(a));
    mx=0;
    Position P = lists[0];
    while(P->next!=NULL){
        P=P->next;
        a[P->pow]+=(P->co);
        if((P->pow)> mx)
            mx=(P->pow);
    }
    for(int i=1; i<z; i++){
        P = lists[i];
        while(P->next!=NULL){
            P=P->next;
            a[P->pow]-=(P->co);
            if((P->pow)> mx)
                mx=(P->pow);
        }
    }
    fprintf(output,"Result of subtraction: ");
}
// creating a function to do the multiplication operation
void mul(List lists[z]){
    memset(a,0,sizeof(a));
    mx=0;
    Position P;
    //creating a temp linked list to store the result of the multiplication of each two equations one after one
    List temp1;
    temp1=(List)malloc(sizeof(struct Node));
    temp1 -> next = NULL;
    List temp2 = (List)malloc(sizeof(struct Node));
    temp2 -> next = NULL;
    Position tail = temp1;
    P = lists[0];
    while(P->next!=NULL){
        P=P->next;
        insert(temp1, tail, P->co, P->pow);
        tail=tail->next;
    }
    // creating a loop to pass on all the linked lists
    for(int i=1; i<z; i++){
        P=temp1;
        tail = temp2;
        while(P->next!=NULL){
            P=P->next;
            Position Q=lists[i];
            while(Q->next!=NULL){
                Q=Q->next;
                insert(temp2,tail,(Q->co)*(P->co),(Q->pow)+(P->pow));
                tail = tail->next;
            }
        }
        deleteList(temp1);
        P=temp2;
        tail = temp1;
        while(P->next!=NULL){
            P=P->next;
            insert(temp1, tail, P->co, P->pow);
            tail = tail->next;
        }
        deleteList(temp2);
    }
    P = temp1;
    while(P->next!=NULL){
        P=P->next;
        a[P->pow]+=(P->co);
        if((P->pow)> mx)
            mx=(P->pow);
    }

    fprintf(output,"Result of multiplication: ");
}
// creating a function to calculate the result of the substitution of the variable value entered by the user
void var(){
    double x=0;
    printf("Please enter a value to substitute: ");
    scanf("%lf", &x);
    double sum=0;
    for(int i=0; i<=mx; i++){
            sum+=(a[i]*pow(x,i));
    }
    printf("Result of substitution = %.1lf\n", sum);
    fprintf(output,"Result of substitution = %.1lf\n", sum);
}
// a function to show the menu for the user ( a menu that shows the available operations for the user )
void showMenu(List lists[z]){
    printf("\nPlease select the number of the operation you want to apply on the equations: \n1- Read equations from the file.\n2- Show equations. \n3- Add\n4- Subtract\n5- Multiply\n6- Store the results of the operations in the file (results.txt).\n7- Exit the program.\n");
}
// a function that does the three mathematical operations and save the results in the output file
void save(List lists[z]){
    add(lists);
    printOnFile();
    sub(lists);
    printOnFile();
    mul(lists);
    printOnFile();
}
// a function that reads the equations from equations.txt file and saving them in linked lists array
void read(List lists[z]){
    char line[MAX];
    z=0;
    // saving each line(equation) in a string called line.
    while(fgets(line, MAX, input) != NULL){
        char c='c';
        List eq;
        eq=(List)malloc(sizeof(struct Node));
        eq -> next = NULL;
        // creating a tale so we can store the new node from the last in O(1)
        Position tail = eq;
        float a=0;
        int b=0,neg=0,cnt=1;
        // creating a loop that passes on all the characters in the equation
        for(int i=0; i<strlen(line); i++){
            if(line[i]==' ')
                continue;
            int x=(int)(line[i]-'0');
            if(line[i]=='+' || line[i]=='-'){
                if(line[i]=='-')
                    neg=1;
                if(c=='c')
                    continue;
                if(c=='x'){
                    if(b==0)
                        b++;
                    if(a==0)
                        a++;
                }
                // inserting a new node to the current linked list with coefficient = a and power = b
                insert(eq,tail,a,b);
                tail = tail->next;
                a=0;
                b=0;
                cnt=1;
                c=line[i];
            }
            else if(line[i]=='^')
                c='^';
            else if(line[i]=='x'){
                neg=0;
                c='x';
            }
            else if(line[i]=='.')
                c='.';
            else if(x>=0){
                if(neg)
                    x=(0-x);
                if(c=='c' || c=='-' || c=='+' ){
                    a*=10;
                    a+=x;
                }
                else if(c=='^'){
                    b*=10;
                    b+=x;
                }
                else if(c=='.'){
                    a+=(x/pow(10,cnt));
                    cnt++;
                }
            }
        }
        // inserting the last node
        insert(eq,tail,a,b);
        tail = tail->next;
        lists[z]=eq;
        z++;
    }
}
int main()
{
    // opening and setting file types and each variable value
    input = fopen("equations.txt", "r");
    output = fopen("results", "a");
    if(input == NULL){
      printf("This file does not exist.\n");
      return 0;
    }
    // creating an array of linked lists
    List lists[MAX];
    printf("Welcome to my program: \n");
    showMenu(lists);
    int c;
    scanf("%d", &c);
    // checking if the user enters any operation before equations are read.
    while(c!=1){
        printf("\nPlease select 1 to read the equations before doing any operation. \n");
        printf("\n1- Read equations from the file.\n2- Show equations. \n3- Add\n4- Subtract\n5- Multiply\n6- Store the results of the operations in the file (results.txt).\n7- Exit the program.\n");
        scanf("%d", &c);
    }
    // checking if the number entered by the user is equal to 7 ( the number of the end program operation ) so the loop ends and the program ends
    while(c!=7){
        // creating a switch to do the right operation based on the user choice
        switch(c){
            // calling the function that reads the equations
            case 1: {
                read(lists);
                break;
            }
            // printing all the equations in the array of linked lists we created
            case 2: {
                    for(int i=0; i<z; i++){
                        printList(lists[i]);
                    }
                    break;
            }
            // addition operation
            case 3: {
                printf("Result of addition: ");
                // calling the functions required for the addition operation
                add(lists);
                printEq();
                printOnFile();
                var();
                break;
            }
            // subtraction operation
            case 4: {
                printf("Result of subraction: ");
                // calling the functions required for the subtraction operation
                sub(lists);
                printEq();
                printOnFile();
                var();
                break;
            }
            // multiplication operation
            case 5: {
                printf("Result of multiplication: ");
                // calling the functions required for the multiplication operation
                mul(lists);
                printEq();
                printOnFile();
                var();
                break;
            }
            // saving the results of all the operations in results.txt file
            case 6: {
                // calling save function
                save(lists);
                printf("Saved Successfully ! \n");
                break;
            }
            // if the user entered any number but the above numbers then the following warning shows up
            default: printf("You entered invalid number. \n"); break;
        }
        // showing the operation menu again
        showMenu(lists);
        scanf("%d", &c);
    }
    printf("Thank you for choosing Majd Maree <3");
    // closing the files
    fclose(input);
    fclose(output);
}
