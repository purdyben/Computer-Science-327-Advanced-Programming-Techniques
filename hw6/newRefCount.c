#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


typedef struct node
{
    void* data;
    int test;
    int reference;
    struct node *next;

} Node_T;


int main (int argc, char *argv[])
 {
    FILE *infile = fopen(argv[1], "r");
    printf("%s\n",argv[1]);
    if(infile == NULL ){
            infile = stdin;
    };
    int node_num;
    fscanf(infile, "%d\n", &node_num);
    printf("%d\n", node_num);


    Node_T *V_list[9] = {NULL};
    Node_T **L_list = malloc(node_num * sizeof(Node_T*));

    int node_count = 0;

    while(node_count < node_num)
    {
        // init nodes 
        L_list[node_count] = malloc(sizeof(Node_T));
        //L_list[node_count]->reference = 1;
        printf("%d\n",node_count);
        L_list[node_count]->test = node_count;
        node_count++;

    }
    printf(" %d\n" ,((*L_list)+1)->test);
    printf(" %d\n" ,((*L_list)+2)->test);
    printf(" %d\n" ,((*L_list)+3)->test);
    printf("\n");
    char c_var; 
    int point_src;
    int point_dest;
    while(fscanf(infile,"%c %d->%d\n", &c_var,&point_src,&point_dest) != EOF)
    {   
        
        if(c_var == 'V')
        { 
            if(point_src > 9 || point_src < 1){
                printf("Error -  V src input is out of bonds");
                exit(0);
            }else if(point_dest > node_count || node_count < 1){
                printf("Error -  V destination input is out of bonds");
                exit(0);
            }else{
                 V_list[point_src] = *(L_list + point_dest-1);
                 //(*L_list + point_dest -1)->reference++;
            }
        }else
        {
            if(point_src > node_count || point_src < 1){
                printf("Error -  L src input is out of bonds");
                exit(0);
            }else if(point_dest > node_count || node_count < 1){
                printf("Error -  L destination input is out of bonds");
                exit(0);
            }else{
                 (*L_list + point_src -1)->next = (*L_list + point_dest -1);
                 //(*L_list + point_dest -1)->reference++;
            }
        }
        
     
    }

    // print out all connections 

    for(int i = 0 ; i < 15 ; i++){
        printf("node %d: ", i);
        Node_T *curr = (*L_list)+i;   
        Node_T *prev = NULL;
        printf(" %d ->" , curr->test+1);    
        while (curr != NULL){            
            printf(" %d ->" , curr->test+1);
   
            if(curr->next != 0x0 && prev == curr->next){
                printf(" %d ->" , prev->test+1);
                break;
            }
            
            prev = curr;
            curr = curr -> next;
        }
        printf(" n \n");
    }

    for(int i = 0 ; i < 9 ; i++){

        if(*((V_list)+i) != 0x0){
            printf("V-node %d: ", i);

            Node_T *curr = (*(V_list+i));
            Node_T *prev = NULL;
            //while (curr != NULL){
                printf(" %d ->" , curr->test+1);
            //     if(curr->next != 0x0 && prev == curr->next){
            //         printf(" %d ->" , prev->test+1);

            //         break;
            //     }
            //     prev = curr;
            //     curr = curr -> next;
            // }
            printf(" n \n");
        }else{
            printf("V-node %d: ", i);
            printf(" -> %p" ,(*(V_list+i)));
            printf("\n");
        }
    }
   return 1; 
}