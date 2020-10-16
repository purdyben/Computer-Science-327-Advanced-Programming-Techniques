#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


typedef struct node
{
    int id;
    int reference;
    struct node *next;

} Node_T;

void reference_count( Node_T *L_list, Node_T **V_list, int node_count);

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
    Node_T *L_list = malloc(node_num * sizeof(Node_T));
    // for(int i = 0 ; i<9 ;i++){
    //     printf("%p ",V_list[i]);
    // }
    printf("\n");

    int node_count = 0;

    while(node_count < node_num)
    {
        // init nodes 
       // L_list[node_count] = malloc(sizeof(Node_T));
        L_list[node_count].id = node_count;
        L_list[node_count].reference = node_count;
        L_list[node_count].next = NULL;

        node_count++;

    }
    // Checks the input file
    char c_var; 
    int point_src;
    int point_dest;
    while(fscanf(infile,"%c %d->%d\n", &c_var,&point_src,&point_dest) != EOF)
    {   
        
        if(c_var == 'V')
        { 
            if(point_src > 9 || point_src < 1){
                printf("Error -  V src input is out of bonds:  %c is not in the bounds" , point_src);
                exit(0);
            }else if(point_dest > node_count || node_count < 1){
                printf("Error -  V destination input is out of bonds: %d is not in the bounds" , point_dest);
                exit(0);
            }else if (point_dest == 0){
                V_list[point_src] = NULL;
            }else{
                V_list[point_src] = (L_list + point_dest-1);
            }
        }else
        {
            if(point_src > node_count || point_src < 1){
                printf("Error -  L src input is out of bonds: %d is not in the bounds" , point_src);
                exit(0);
            }else if(point_dest > node_count || node_count < 1){
                printf("Error -  L destination input is out of bonds: %d is not in the bounds" , point_dest);
                exit(0);
            }else if (point_dest == 0){
                (L_list + point_src -1)->next = NULL;
            }else{
                 (L_list + point_src -1)->next = (L_list + point_dest -1);
                 //(*L_list + point_dest -1)->reference++;
            }
        }
    }

    reference_count(L_list, V_list,node_num);

   return 1; 
}
void reference_count( Node_T *L_list, Node_T **V_list,int node_num){
    int ref[node_num];
    memset(ref, 0, sizeof(ref));
    // for(int i = 0 ; i < node_num ; ++i){
    //   L_list[i].reference = 0;
    // }
    for(int i = 0 ; i < 9 ; ++i){
        if((*(V_list+i))!= 0x0){
            //(*(V_list+i))->reference = 1;
            ref[(*(V_list+i))->id] = 1;
        }
    }
    for(int node_count = 0; node_count < node_num ; node_count++){
        if((L_list + node_count)->next != NULL){
          //(L_list + node_count)->next->reference++;
           ref[(L_list + node_count)->next->id]++;
        }
    }
    for(int i = 0 ; i < node_num ; i++){
        if(ref[i]!= 0){
        printf("Node %d has %d incoming pointers\n",i+1,ref[i]);
        }
    }
}
    //print out all connections 

    // for(int i = 0 ; i < 15 ; i++){
    //     printf("node %d: ", i);
    //     Node_T *curr = (L_list)+i;   
    //     Node_T *prev = NULL;
    //     while (curr != NULL){            
    //         printf(" %d ->" , curr->id+1);
   
    //         if(curr->next != 0x0 && prev == curr->next){
    //             printf(" %d ->" , prev->id+1);
    //             break;
    //         }
            
    //         prev = curr;
    //         curr = curr -> next;
    //     }
    //     printf(" n \n");
    // }

    // for(int i = 0 ; i < 9 ; i++){

    //     if(*((V_list)+i) != 0x0){
    //         printf("V-node %d: ", i);

    //         Node_T *curr = (*(V_list+i));
    //         Node_T *prev = NULL;
    //         while (curr != NULL){
    //             printf(" %d ->" , curr->id+1);
    //             if(curr->next != 0x0 && prev == curr->next){
    //                 printf(" %d ->" , prev->id+1);
    //                 break;
    //             }
    //             prev = curr;
    //             curr = curr -> next;
    //         }
    //         printf(" n \n");
    //     }else{
    //         printf("V-node %d: ", i);
    //         printf(" -> %p" ,(*(V_list+i)));
    //         printf("\n");
    //     }
    // }

        // for(int i = 0 ; i < node_num ; i++){
    //     if(L_list[i].reference != 0){
    //     printf("Node %d has %d incoming pointers\n",i+1,L_list[i].reference );
    //     }
    // }
// Node 1 has 1 incoming pointers
// Node 2 has 1 incoming pointers
// Node 3 has 1 incoming pointers
// Node 5 has 1 incoming pointers
// Node 6 has 3 incoming pointers
// Node 7 has 1 incoming pointers
// Node 10 has 1 incoming pointers
// Node 11 has 2 incoming pointers
// Node 12 has 2 incoming pointers