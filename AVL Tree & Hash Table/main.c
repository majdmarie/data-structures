#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXs 100
#define MAXl 1000

// defining global variables for the files
FILE *input,*output;

// defining structs
struct AVLnode;
typedef struct AVLnode *AVL_T;

int tableSize, patientSize;
struct patientHash;
typedef struct patientHash* hash;

AVL_T root;
hash hashTable;
// creating the struct avlnode
struct AVLnode{
    char name[MAXs];
    char gender[MAXs];
    char dateOfAdmission[MAXs];
    char dateOfBirth[MAXs];
    char illness[MAXs];
    char address[MAXs];
    char blood[MAXs];
    AVL_T left;
    AVL_T right;
    int height;
};

// creating the struct patient
struct patientHash{
    int st;
    char name[MAXs];
    char gender[MAXs];
    char dateOfAdmission[MAXs];
    char dateOfBirth[MAXs];
    char illness[MAXs];
    char address[MAXs];
    char blood[MAXs];
};



int max(int a, int b){
    return (a > b)? a : b;
}

// a function that returns the height of the node
int height(AVL_T N){
    if (N == NULL)
        return 0;
    return N->height;
}

// a function that returns the node that minimum value
AVL_T minValueNode(AVL_T node){
    AVL_T current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// a get balance function
int getBalanceFactor(AVL_T N){
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// a function single rotatation for right
AVL_T singleRightRotate(AVL_T y){
    AVL_T x = y->left;
    AVL_T T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    return x;
}

// a function single rotatation for left
AVL_T singleLeftRotate(AVL_T x)
{
    AVL_T y = x->right;
    AVL_T T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    return y;
}

// a function for a double left rotation
AVL_T doubleLeftRotate(AVL_T K3) {
    K3->left  = singleLeftRotate(K3->left);
    return singleRightRotate(K3);
}

// a function for a double right rotation
AVL_T doubleRightRotate(AVL_T K3) {
    K3->right  = singleRightRotate(K3->right);
    return singleLeftRotate(K3);
}

// a function to insert a new node in the avl tree
AVL_T insert (AVL_T t, char name[], char gender[], char dateOfAdmission[], char dateOfBirth[], char illness[], char address[], char blood[]){
    if(t==NULL){
        t=malloc(sizeof (struct AVLnode));
        strcpy(t->name,name);
        strcpy(t->gender,gender);
        strcpy(t->dateOfAdmission,dateOfAdmission);
        strcpy(t->dateOfBirth,dateOfBirth);
        strcpy(t->illness,illness);
        strcpy(t->address,address);
        strcpy(t->blood,blood);
        t->height=0;
        t->left= t->right=NULL;
    }
    else if(strcasecmp(name,t->name)<0){
        t->left=insert(t->left, name, gender, dateOfAdmission, dateOfBirth, illness, address, blood);
        if(height(t->left) - height(t->right) >= 2){
            if(strcasecmp(name,t->left->name)<0)
                t=singleRightRotate(t);
            else
                t=doubleLeftRotate(t);
        }
    }
    else if(strcasecmp(name,t->name)>0){
        t->right = insert(t->right, name, gender, dateOfAdmission, dateOfBirth, illness, address, blood);
        if(height(t->right) - height(t->left) >= 2){
            if(strcasecmp(name,t->right->name)>0)
                t=singleLeftRotate(t);
            else
                t=doubleRightRotate(t);
        }
    }

    t->height = 1 + max(height(t->right) , height(t->left));
    return t;
}

// a function that deletes a node from the avl tree
AVL_T deleteNode(AVL_T root, char name[]){
    if(root == NULL)
        return root;
    if(strcasecmp(name,root->name)<0)
        root->left = deleteNode(root->left,name);
    else if(strcasecmp(name,root->name)>0)
        root->right = deleteNode(root->right, name);
    else{
        if((root->left == NULL) || (root->right == NULL)){
            AVL_T temp = root->left? root->left: root->right;
            if (temp == NULL){
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else{
            AVL_T temp = minValueNode(root->right);
            strcpy(root->name, temp->name);
            root->right = deleteNode(root->right, temp->name);
        }
    }
    if (root == NULL)
        return root;

    root->height = max(height(root->left), height(root->right))+1;

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return singleRightRotate(root);

    if (balance > 1 && getBalanceFactor(root->left) < 0){
        root->left = singleLeftRotate(root->left);
        return singleRightRotate(root);
    }

    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return singleLeftRotate(root);

    if (balance < -1 && getBalanceFactor(root->right) > 0){
        root->right = singleRightRotate(root->right);
        return singleLeftRotate(root);
    }

    return root;
}

// a function that reads data from input file and store it in the avl tree
void read(){
    input = fopen("patients.txt", "r");
    if(input == NULL){
      printf("This file does not exist.\n");
      return;
    }
    char line[MAXl];
    while(fgets(line, MAXl, input) != NULL){
        char *n = strtok(line, "#");
        char *g = strtok(NULL, "#");
        char *d = strtok(NULL, "#");
        char *db = strtok(NULL, "#");
        char *ill = strtok(NULL, "#");
        char *add = strtok(NULL, "#");
        char *bl = strtok(NULL, "\n");
        root = insert(root,n,g,d,db,ill,add,bl);
    }
    fclose(input);
}

// a function to insert a new patient in the avl tree
void insertPatient(AVL_T t){
    char n[MAXs];
    char g[MAXs];
    char d[MAXs];
    char db[MAXs];
    char ill[MAXs];
    char add[MAXs];
    char bl[MAXs];
    printf("Please enter the following patient information: \n");
    printf("Name: ");
    fflush(stdin);
    gets(n);
    printf("Gender: ");
    fflush(stdin);
    gets(g);
    printf("Date of Admission: ");
    fflush(stdin);
    gets(d);
    printf("Date of Birth: ");
    fflush(stdin);
    gets(db);
    printf("Illness: ");
    fflush(stdin);
    gets(ill);
    printf("Address: ");
    fflush(stdin);
    gets(add);
    printf("Blood Type: ");
    fflush(stdin);
    gets(bl);
    root = insert(root,n,g,d,db,ill,add,bl);
}

// a function that finds a certain patient entered by the user
void findPatient(AVL_T t){
    char name[MAXs];
    printf("Please enter the name of the patient you are searching for: ");
    fflush(stdin);
    gets(name);
    AVL_T se = root;
    int found=0;
    while(se!=NULL){
        if(strcasecmp(se->name, name)==0){
            found=1;
            break;
        }
        else if(strcasecmp(se->name, name)<0)
            se = se->right;
        else
            se = se->left;
    }
    if(!found){
        printf("The patient cannot be found \n");
        return;
    }
    printf("Do you would like to update the information of the patient? (Yes/No)\n");
    char up[MAXs];
    scanf("%s", up);
    if(strcasecmp(up,"no")==0)
        return;
    if(strcasecmp(up,"yes")==0){
        root = deleteNode(root, name);
        insertPatient(root);
    }
    else{
        printf("Invalid choice");
        return;
    }
}

// a function to print the avl tree in lexicographically order
void printInOrder(AVL_T t){
    if(t!= NULL){
        printInOrder(t->left);
        printf("Name: %s  Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",t->name,t->gender,t->dateOfAdmission,t->dateOfBirth,t->illness,t->address,t->blood);
        printInOrder(t->right);
    }
}

// a function to print the list of patients that have a certain illness entered by the user
void sameIllness(AVL_T t, char illness[]){
    if(t!= NULL){
        sameIllness(t->left, illness);
        if(strcasecmp(t->illness, illness)==0)
            printf("Name: %s  Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",t->name,t->gender,t->dateOfAdmission,t->dateOfBirth,t->illness,t->address,t->blood);
        sameIllness(t->right, illness);
    }
}

// a function that deletes a certain patient entered by the user from the avl tree
void deletePatient(AVL_T t){
    printf("Please enter the name of the patient you want to delete: ");
    char na[MAXs];
    fflush(stdin);
    gets(na);
    root = deleteNode(root, na);
}

// a function that stores the patients in an output file
void savePatients(AVL_T t){
    if(t!= NULL){
        savePatients(t->left);
        fprintf(output,"Name: %s - Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",t->name,t->gender,t->dateOfAdmission,t->dateOfBirth,t->illness,t->address,t->blood);
        patientSize++;
        savePatients(t->right);
    }

}

// a function which checks whether the number is prime or not
int isPrime(int n){
    if(n<2)
        return 0;
    int f=1;
    for(int i=2; i*i<=n; i++){
        if(n%i==0){
            f=0;
            break;
        }
    }
    return f;
}

// a function that inserts a new patient in the hash table
void insertHash(hash hashTable, char name[], char gender[], char dateOfAdmission[], char dateOfBirth [], char illness[], char address [], char blood []){
    int s=0;
    for(int i=0; i<strlen(name); i++){
        s+=name[i];
    }
    s%=tableSize;
    while(hashTable[s].st==1){
        s++;
        s%=tableSize;
    }
    hashTable[s].st=1;
    strcpy(hashTable[s].name,name);
    strcpy(hashTable[s].gender,gender);
    strcpy(hashTable[s].dateOfAdmission,dateOfAdmission);
    strcpy(hashTable[s].dateOfBirth,dateOfBirth);
    strcpy(hashTable[s].illness,illness);
    strcpy(hashTable[s].address,address);
    strcpy(hashTable[s].blood,blood);
    if((((double)patientSize+1.0)/(double)tableSize)>0.75){
        reHash();
    }
}

// a function that set a new size for the hash table
void reHash(){
    int newSize=2*tableSize;
    int f=0;
    while(!f){
        f=1;
        for(int i=2;i*i<=newSize;i++){
            if(newSize%i==0){
                f=0;
            }
        }
        if(!f)
            newSize++;
    }
    hash newHash;
    newHash = (hash)malloc(sizeof(struct patientHash)*newSize);
    for(int i=0;i<newSize;i++){
        newHash[i].st=0;
    }
    int old_size=tableSize;
    tableSize=newSize;
    for(int i=0;i<old_size;i++){
        if(hashTable[i].st == 1)
            insertHash(newHash,hashTable[i].name,hashTable[i].gender,hashTable[i].dateOfAdmission,hashTable[i].dateOfBirth,hashTable[i].illness,hashTable[i].address,hashTable[i].blood);
    }
    hashTable = newHash;
}

// a function to fill the hash table from the avl tree
void fillHash(AVL_T t){
    if(t!= NULL){
        fillHash(t->left);
        insertHash(hashTable, t->name, t->gender, t->dateOfAdmission, t->dateOfBirth, t->illness, t->address, t->blood);
        fillHash(t->right);
    }
}

// a function that prints the hash table
void printHash(){
    for(int i=0; i<tableSize; i++){
        if(hashTable[i].st==0){
            printf("empty \n");
            continue;
        }
        if(hashTable[i].st==2){
            printf("deleted \n");
            continue;
        }
        printf("Name: %s  Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",hashTable[i].name,hashTable[i].gender,hashTable[i].dateOfAdmission,hashTable[i].dateOfBirth,hashTable[i].illness,hashTable[i].address,hashTable[i].blood);
    }
}
// a function to add let the user add a new patient to the hash table
void addHash(){
    char n[MAXs];
    char g[MAXs];
    char d[MAXs];
    char db[MAXs];
    char ill[MAXs];
    char add[MAXs];
    char bl[MAXs];
    printf("Please enter the following patient information: \n");
    printf("Name: ");
    fflush(stdin);
    gets(n);
    printf("Gender: ");
    fflush(stdin);
    gets(g);
    printf("Date of Admission: ");
    fflush(stdin);
    gets(d);
    printf("Date of Birth: ");
    fflush(stdin);
    gets(db);
    printf("Illness: ");
    fflush(stdin);
    gets(ill);
    printf("Address: ");
    fflush(stdin);
    gets(add);
    printf("Blood Type: ");
    fflush(stdin);
    gets(bl);
    insertHash(hashTable,n,g,d,db,ill,add,bl);
}
// a function to search for a patient in the hash table
void findHash(){
    char name [MAXs];
    printf("Please enter the name of the patient you are searching for: \n");
    fflush(stdin);
    gets(name);
    for(int i=0; i<tableSize; i++){
        if(strcasecmp(hashTable[i].name,name)==0 && hashTable[i].st!=2){
            printf("Patient is found and this is his information: \n");
            printf("Name: %s  Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",hashTable[i].name,hashTable[i].gender,hashTable[i].dateOfAdmission,hashTable[i].dateOfBirth,hashTable[i].illness,hashTable[i].address,hashTable[i].blood);
            return;
        }
    }
    printf("Patient cannot be found \n");
}

// a function that deletes a patient if found from the hash table
void deleteHash(){
    char name [MAXs];
    printf("Please enter the name of the patient you want to delete: \n");
    fflush(stdin);
    gets(name);
    for(int i=0; i<tableSize; i++){
        if(strcasecmp(hashTable[i].name,name)==0){
            hashTable[i].st=2;
            patientSize--;
            return;
        }
    }
    printf("Patient cannot be found\n");
}

// a function that stores the hash table in an output file called patients_hash.data.txt
void saveHash(){
    output = fopen("patients_hash.data.txt", "w");
    for(int i=0; i<tableSize; i++){
        if(hashTable[i].st==0){
            fprintf(output,"empty \n");
            continue;
        }
        if(hashTable[i].st==2){
            fprintf(output,"deleted \n");
            continue;
        }
        fprintf(output,"Name: %s  Gender: %s - Date of Admission: %s - Date of Birth: %s - Illness: %s - Address: %s - Blood Type: %s\n",hashTable[i].name,hashTable[i].gender,hashTable[i].dateOfAdmission,hashTable[i].dateOfBirth,hashTable[i].illness,hashTable[i].address,hashTable[i].blood);
    }
    fclose(output);
}
// a function to show the hash menu
void hashMenu(){
    tableSize=ceil(patientSize/0.7);
    while(!isPrime(tableSize))
        tableSize++;
    hashTable = (hash)malloc(sizeof(struct patientHash)*tableSize);
    for(int i=0; i<tableSize; i++){
        hashTable[i].st=0;
    }
    fillHash(root);
    printf("Welcome to my hash menu: \n");
    printf("\nPlease choose the number of the operation you want to do:\n1- Print hashed table. \n2- Print out table size. \n3- Print out the used hash function.\n4- Insert a new record into the hash table.\n5- Search for a specific patient.\n6- Delete a specific record.\n7- Save hash table back to file\n8- Close the program\n");
    int c;
    scanf("%d", &c);
    while(c!=8){
        switch(c){
            case 1: {
                printHash();
                break;
            }
            case 2: {
                printf("Table size = %d", tableSize);
                break;
            }
            case 3: {
                printf("The hashing function is: h[i](key) = [h(key) + i] %% tableSizen");
                break;
            }
            case 4: {
                addHash();
                patientSize++;
                break;
            }
            case 5: {
                findHash();
                break;
            }
            case 6: {
                deleteHash();
                break;
            }
            case 7: {
                saveHash();
                break;
            }

        }
        printf("\nPlease choose the number of the operation you want to do:\n1- Print hashed table. \n2- Print out table size. \n3- Print out the used hash function.\n4- Insert a new record into the hash table.\n5- Search for a specific patient.\n6- Delete a specific record.\n7- Save hash table back to file\n8- Close the program\n");
        scanf("%d", &c);
    }
}

// main function
int main()
{   system("COLOR F4");
    printf("Welcome to my program: \n");
    printf("\nPlease choose the number of the operation you want to do:\n1- Read the file patients.txt and load the data.\n2- Insert a new patient from user with all its associated data. \n3- Find a patient and give the user the option to update the information of the patient if found.\n4- List all patients in lexicographic order with their associated information.\n5- List all patients that have the same illness.\n6- Delete a patient from the system.\n7- Save all words in file “patients_hash.data” and move to the hash menu\n8- Close the program\n");
    int c;
    scanf("%d", &c);
    while(c!=1){
        printf("\nPlease select 1 to read the equations before doing any operation. \n");
        printf("\n1- Read the file patients.txt and load the data.\n2- Insert a new patient from user with all its associated data. \n3- Find a patient and give the user the option to update the information of the patient if found.\n4- List all patients in lexicographic order with their associated information.\n5- List all patients that have the same illness.\n6- Delete a patient from the system.\n7- Save all words in file “patients_hash.data” and move to the hash menu\n8- Close the program\n");
        scanf("%d", &c);
    }
    while(c!=8){
        switch(c){
            case 1: {
                read();
                break;
            }
            case 2: {
                insertPatient(root);
                break;
            }
            case 3: {
                findPatient(root);
                break;
            }
            case 4: {
                printInOrder(root);
                break;
            }
            case 5: {
                char illness[MAXs];
                printf("Please type the name of the illness to show you the patients who have it: \n");
                fflush(stdin);
                gets(illness);
                sameIllness(root, illness);
                break;
            }
            case 6: {
                deletePatient(root);
                break;
            }
            case 7: {
                output = fopen("patients_hash.data.txt", "w");
                savePatients(root);
                fclose(output);
                hashMenu();
                break;
            }

        }
        printf("\nPlease choose the number of the operation you want to do:\n1- Read the file patients.txt and load the data.\n2- Insert a new patient from user with all its associated data. \n3- Find a patient and give the user the option to update the information of the patient if found.\n4- List all patients in lexicographic order with their associated information.\n5- List all patients that have the same illness.\n6- Delete a patient from the system.\n7- Save all words in file “patients_hash.data” and move to the hash menu\n8- Close the program\n");
        scanf("%d", &c);
    }
    return 0;
}
