#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 1000
FILE* input, *output;
int cPath=0;
int graph[SIZE][SIZE];
char towns[SIZE][SIZE][100];
void showMenu(){
    printf("\nWelcome to my Palestine:\n1. Read the file segments.txt and load the data.\n2. Enter 2 points to compute the shortest path between them.\n3. The route of the shortest distance to a file called “route.txt”\n4. Exit\n");
}
void read(){
    input=fopen("segments.txt", "r");
    char line[100];
    while(fgets(line,100,input)!=NULL){
        char *tok;
        int src,des,km;
        char town[100];
        memset(town, '\0', 100);
        tok = strtok(line," ");
        src = atoi(tok);
        tok = strtok(NULL, " ");
        des = atoi(tok);
        tok = strtok(NULL, " ");
        while(!(tok[0]-'0'>=0 && tok[0]-'0'<=9)){
            strcat(town, tok);
            strcat(town, " ");
            tok = strtok(NULL, " ");
        }
        km = atoi(tok);
        graph[src][des]=km;
        graph[des][src]=km;
        strcpy(towns[src][des],town);
        strcpy(towns[des][src],town);
    }

    fclose(input);
}

void dijkstra(vertex matrix[MAX][MAX], int start, int end)
{
    int distance[MAX];

    int visitedSet[MAX];

    int parent[MAX];

    for (int i = 0; i < MAX; i++)
    {
        parent[i] = -1;
        distance[i] = INT_MAX;
        visitedSet[i] = 0;
    }
    distance[start] = 0;
    for (int count = 0; count < MAX - 1; count++)
    {
        int u = minDistance(distance, visitedSet);

        visitedSet[u] = 1;

        for (int n = 0; n < MAX; n++)
            if (!visitedSet[n] && matrix[u][n] ->value && distance[u] + matrix[u][n] ->value < distance[n])
            {
                parent[n] = u;
                distance[n] = distance[u] + matrix[u][n] ->value;
            }
    }
    if(distance[end] != INT_MAX){
        display(distance, MAX, parent, start, end);
        printf("\nSave path to file? ");
        char yes[3];
        scanf("%s", yes);
        if(strcasecmp(yes, "yes") == 0){
            output = fopen("route.txt", "a");
            displayFile(distance, MAX, parent, start, end);
            fprintf(output, "\n");
            fclose(output);
        }
        printf("\nEnter the new destination:(or Enter -1 to end) ");
        int x;
        scanf("%d", &x);
        if(x == -1){
            return;
        }
        else{
            dijkstra(matrix, end, x);
        }
    }
    else{
        printf("There is no way between %d and %d\n", start, end);
    }
}

int minC(int cost[], int visited[]){
    int ind;
    int mn=INT_MAX;
    for(int i=0; i<SIZE; i++){
        if(cost[i]<mn && !visited[i]){
            mn=cost[i];
            ind=i;
        }
    }
    return ind;
}
void printPath(int parent[], int j)
{
    if (parent[j] == - 1)
        return;

    printPath(parent, parent[j]);
    printf("%s ",towns[j][parent[j]]);
    printf("%d ", j);

    fprintf(output,"%s ",towns[j][parent[j]]);
    fprintf(output,"%d ", j);

}
void printFirst(int cost[], int parent[],int src, int dest){

    printf("Vertex\t Distance\tPath");
    if (cost[dest]!=INT_MAX)
        printf("\n%d -> %d \t %d\t\t%d\n",src, dest, cost[dest], src);
    else
        printf("\n%d -> %d \t no path\n",src, dest);

    fprintf(output,"Vertex\t Distance\tPath");
    if (cost[dest]!=INT_MAX)
        fprintf(output,"\n%d -> %d \t %d\t\t%d\n",src, dest, cost[dest], src);
    else
        fprintf(output,"\n%d -> %d \t no path\n",src, dest);
    if (cost[dest]!=INT_MAX)
        printPath(parent, dest);

}

void dijkstra(int src, int des){
    int cost[SIZE], parent[SIZE], visited[SIZE];
    for(int i=0; i<SIZE; i++){
        cost[i]=INT_MAX;
        parent[i]=-1;
        visited[i]=0;
    }
    cost[src]=0;
    for(int i=0; i<SIZE; i++){
        int x;
        x = minC(cost, visited);
        visited[x]=1;
        for(int y=0; y<SIZE; y++){
            if(graph[x][y] && !visited[y] && cost[x]+graph[x][y]<cost[y]){
                cost[y]=cost[x]+graph[x][y];
                parent[y]=x;
            }
        }
    }
    printFirst(cost, parent, src, des);

}

int main()
{
    showMenu();
    int c;
    scanf("%d", &c);
    while(c!=1){
        printf("Please read the data from the input file before doing any process. \n");
        showMenu();
        scanf("%d", &c);
    }
    read();
    while(c!=4){
        int l,r;
        switch(c){
            case 1: read();
                    break;
            case 2:
                printf("Enter the first point: ");
                scanf("%d",&l);
                printf("Enter the second point: ");
                scanf("%d",&r);
                dijkstra(l, r);
                cPath=1;
                break;
            case 3:
                output = fopen("route.txt", "w");
                if (cPath){
                    dijkstra(l, r);
                }
                else {
                    printf("No shortest path has been executed\n\n");
                }
                break;

            case 4: break;
            default: printf("Please enter valid data\n"); break;
        }
        showMenu();
        scanf("%d", &c);
    }
    fclose(output);
    return 0;
}


