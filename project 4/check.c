#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "linkedlist.h"

//#define Print_Testing


typedef struct cardCheck
{
    int C; //covered cards
    int S;// stock cards
    int W; //waste cards

} cardCheck;

typedef struct Rules
{
    int turn;
    int limit;

} Rules;

typedef struct Foundations
{
    List * foundations[4];

} Foundations;

typedef struct TABLEAU
{
    List *tableList[7];

} TABLEAU_T;
typedef struct Stock
{
    List *list;

} stockCards;
typedef struct Moves
{
     List *list;

} Move_T;


void get_Next_Char(FILE* f);
void getRules(FILE* f, Rules * );
void getfoundations(FILE* inf, Foundations *);
void get_Colon(FILE *f);
void getTableaut(FILE *f, TABLEAU_T *, Foundations *);
int skip_space(char c);
void getStock(FILE *, stockCards *);

int suit_Hierarchy(char );
int rank_Hierarchy(char );

char suit_int_to_position(int);
char rank_int_to_postion(int);

void Check_Data(stockCards *, Foundations *, TABLEAU_T *);
List* reverse_list(List *list);
void cut_tail(List *list);

int main (int argc, char *argv[]) {

    FILE *file_out = stdout;
    FILE* infile = fopen(argv[1], "r");
    if(infile == NULL ){
        infile = stdin;
    }
        Rules *rule = malloc(sizeof(struct Rules));
        Foundations *foundations = malloc(sizeof(struct Foundations));
        TABLEAU_T *table = malloc(sizeof(struct TABLEAU));
        stockCards *stock = malloc(sizeof(struct Stock));
        // Move_T *moves = malloc(sizeof(Move_T));
        
        getRules(infile, rule);
        getfoundations(infile, foundations);
        getTableaut(infile, table, foundations);
        getStock(infile,stock);
        
        Check_Data(stock,foundations, table);
        // printf("tail %c%c head %c %c", stock->list->tail->card->rank,
        // stock->list->tail->card->suit,
        // stock->list->head->card->rank,
        //stock->list->head->card->suit);
        // Node *curr = moves->list->head;
        //  char str_invalid_moves[255] = {0};

    fclose(infile);
    fclose(file_out);

    return 1;

};

void get_Next_Char(FILE* inf){
  char c;
  for (;;) {
    c = fgetc(inf);
    if (' ' == c) continue;
    if ('\t' == c) continue;
    if ('\n' == c) continue;
    if ('\r' == c) continue;
    if ('-' == c) continue;
    if ('>' == c) continue;
    if ('#' == c){
       while(c != '\n'){
        c = fgetc(inf);
       };
       continue;
    };
    if (EOF == c) return;
    break;
  }
  ungetc(c, inf);
};
void get_Colon(FILE* inf){
    char c = fgetc(inf);
     while(c != EOF){
        //get_Next_Char(inf); 
        c = fgetc(inf);
        
        if(c == ':'){
            return;
        }
     }
     if(c == EOF){
         printf("Invalid file\n");
         exit(0);
     }
};
void getRules(FILE* inf, struct Rules *r){
    #ifdef Print_Testing
        printf("Rules\n" );
    #endif
    
    get_Colon(inf);
    get_Next_Char(inf);
        char c;

    while(!isdigit(c)){
        c = fgetc(inf);
        if(c == ':'){
            printf("Invalid file\n");
            exit(0);
        }
    }
    r -> turn = c - '0';
    get_Next_Char(inf);
    c = fgetc(inf);
    //printf("%c1\n", c);

    if(c != 'u'){
        while(!isdigit(c)){
         c = fgetc(inf);
        // printf("%c2\n", c);
           if(c == ':'){
            printf("Invalid file\n");
            exit(0);
        }
        }
         r ->limit = c - '0';
        
        
    }else{
        r -> limit = -1;
    }
    #ifdef Print_Testing
         printf("%d, %d\n" ,r->limit, r->turn );
    #endif
   

};
void getfoundations(FILE* inf, struct Foundations *f){
    #ifdef Print_Testing
        printf("foundations\n");
    #endif

    get_Colon(inf);
    get_Next_Char(inf);
    for(int i = 0;i < 4; ++i){
        char rank = fgetc(inf);
        char suit = fgetc(inf);
            Node *newNode = malloc(sizeof(Node));
            newNode->card = malloc(sizeof(Card));
            switch (suit){
            List *list = malloc(sizeof(List));

            case 'c':
                newNode->card->suit = 'c';
                newNode->card->rank = rank;
                list = malloc(sizeof(List));
                f->foundations[0] = list;
                f -> foundations[0]->head = newNode;
                f -> foundations[0]->tail = newNode;
                break;
            case 'd':
                newNode->card->suit = 'd';
                newNode->card->rank = rank;
                list = malloc(sizeof(List));
                f->foundations[1] = list;
                f -> foundations[1]->head = newNode;
                f -> foundations[1]->tail = newNode;

                break;
            case 'h':
                newNode->card->suit = 'h';
                newNode->card->rank = rank;
               list = malloc(sizeof(List));
                f->foundations[2] = list;
                f -> foundations[2]->head = newNode;
                 f -> foundations[2]->tail = newNode;
                break;
            case 's':
                newNode->card->suit = 's';
                newNode->card->rank = rank;
                list = malloc(sizeof(List));
                f->foundations[3] = list;
                f -> foundations[3]->head = newNode;
                f -> foundations[3]->tail = newNode;

                break; 
            };

    
         get_Next_Char(inf);
    }
     #ifdef Print_Testing
         for(int i = 0 ; i < 4 ; i++){
             printf("%c,%c\n", f->foundations[i]->head->card->rank,  f->foundations[i]->head->card->suit);
        // printf("%c\n", f -> foundations[i]);

    };
        printf("%c\n", fgetc(inf));
    #endif
   

};
void getTableaut(FILE *inf,  TABLEAU_T *table, Foundations *f){
    #ifdef Print_Testing
        printf("Tableaut - method\n");
    #endif

    get_Colon(inf);
    get_Next_Char(inf);

    for(int i = 0 ; i< 7 ; ++i){
        List *newList = malloc(sizeof(List));
        char c = fgetc(inf);
        int j =0;

        while('\n' != c){ // new line
            if(c == '#'){
                while(c != '\n'){
                    c = fgetc(inf);
                };
                break;
            }else if(c == '\n' || c == '\r'){
                c = fgetc(inf); 
            } else if( c == ' '){
                c = fgetc(inf);
            }else{ // add card
                Node *newNode = malloc(sizeof(Node));
                newNode -> card = malloc(sizeof(Card));
                if(c == '|'){
                    newNode->card->faceDown = 1;
                    // addNode(newList,newNode);
                    // Node *current = newList->head;
                    // while(current->card->faceDown != 1){
                    //     current->card->faceDown = 1;
                    //     current = current->next;
                    // };
                    Node *current = newList->head;
                    // while(current->card->faceDown != 1){
                    while(current != NULL){
                    current->card->faceDown = 1;
                    current = current->next;
                };
                }else{
                    newNode->card->faceDown = 0;
                    newNode->card->rank = c;
                    get_Next_Char(inf);
                    c = fgetc(inf);
                    newNode->card->suit = c;
                     addNode(newList,newNode);
                };
                c = fgetc(inf);

            };            
        };   

        table->tableList[i] = newList;
        Node *current = table->tableList[i]->head;
        
        #ifdef Print_Testing
            printf("List %d ", i);
            while(current != NULL){
                if(current->card->faceDown == 1 && current ->card ->rank == '\0'){
                    printf("| ");
                }else
                {
                printf("%c,%c,%d - ",current ->card ->rank, current->card->suit,current->card->faceDown);  
                }
                current = current->next;
            }
            printf("\n");


        #endif
     
    };
};
int skip_space(char c){
    if(c == ' '){
        return 1;
    };
    return 0;
};
void getStock(FILE* inf, stockCards *s){
    #ifdef Print_Testing
        printf("Stock cards11\n");
    #endif
    get_Colon(inf);
    get_Next_Char(inf);
    
    s->list = malloc(sizeof(List));
    char c = fgetc(inf);
    while(c != 'M'){ // new line
        if(c == EOF){
            break;
        }
        if(c == '#'){
            while(c != '\n'){
                c = fgetc(inf);
            };
        }else if(c == '\n' || c == '\r'){
            c = fgetc(inf); 
        } else if( c == ' '){
            c = fgetc(inf);
        }else{ // add card
            Node *newNode = malloc(sizeof(Node));
            newNode -> card = malloc(sizeof(Card));
             if(c == '|'){
                //newNode->card->faceDown = 1;
                // make prev cards faceDown
                //addNode(s->list, newNode);
                if(s->list->head != NULL){
                        Node *current = s->list->head;
                    // while(current->card->faceDown != 1){
                    while(current != NULL){
                        current->card->faceDown = 1;
                        current = current->next;

                    };
                    get_Next_Char(inf);
                }

            }else{
                newNode->card->faceDown = 0;
                newNode->card->rank = c;
                get_Next_Char(inf);
                c = fgetc(inf);
                newNode->card->suit = c;
                addNode(s->list, newNode);
                s->list->tail = newNode;
            };
            c = fgetc(inf);
        };

    }; 
    // reversing the #$%$ing face down cards 
    

     if(s->list->head != NULL){
        Node *current = s->list->head;
        while(current != NULL){
            if( current->card->faceDown == 1)
                current->card->faceDown = 0;
            else
            {
                current->card->faceDown = 1;
            }
            
            current = current->next;
        };

         s->list = reverse_list(s->list);
    }
        #ifdef Print_Testing
            Node * curr = s->list->head;
            while(curr != NULL){
                printf("%c,%c,%d - ",curr ->card ->rank, curr->card->suit,curr->card->faceDown);  
                curr = curr->next; 
            }
           
            printf("done with stock");
        #endif
};

//good luck trying to read this monster. 
void Check_Data(stockCards *stock, Foundations *f, TABLEAU_T *table)
{
    #ifdef Print_Testing
        printf("check data beginging");
    #endif
    int valid = 1;
    List *doubledList = malloc(sizeof(List));
    List *missingList = malloc(sizeof(List));
    int check_deck[13][4] = {0}; // c,d,h,s


    /// going through all 7 rows on the table
    for(int i = 0 ; i < 7; i++)
    {
        // parsing threw the table
        Node *current = table->tableList[i]->head;
        int j = 0;

        int deckPosition ;

        //going threw the list on the row i
        while(current != NULL)
        {
            deckPosition = check_deck[rank_Hierarchy(current->card->rank)][suit_Hierarchy(current->card->suit)];
            //checking that the node has a valid card and not a  | 
            //also making sure we havent seen ths card befor
            if(deckPosition == 0)
            {
                //printf("missing card\n");
                check_deck[rank_Hierarchy(current->card->rank)][suit_Hierarchy(current->card->suit)] = 1;
            
            }else if(deckPosition == 1)
            {
                //adding double card to list doubleList
                Node *newNode = malloc(sizeof(Node));
                newNode->card->rank = current->card->rank;
                newNode->card->suit = current->card->suit;
                newNode->card->faceDown = current->card->faceDown;
                addNode(doubledList, newNode);
            };

            current = current->next;
        };
    };
    // pars threw Stock
   Node *current = stock->list->head;
        int j = 0;
        int deckPosition ;

        //going threw the list on the row i
        while(current != NULL)
        {
            deckPosition = check_deck[rank_Hierarchy(current->card->rank)][suit_Hierarchy(current->card->suit)];
            //checking that the node has a valid card and not a  | 
            //also making sure we havent seen ths card befor
            if(deckPosition == 0)
            {
               // printf("missing card\n");
                check_deck[rank_Hierarchy(current->card->rank)][suit_Hierarchy(current->card->suit)] = 1;
            
            }else if(deckPosition == 1)
            {
                //adding double card to list doubleList
                Node *newNode = malloc(sizeof(Node));
                newNode->card->rank = current->card->rank;
                newNode->card->suit = current->card->suit;
                newNode->card->faceDown = current->card->faceDown;
                addNode(doubledList, newNode);
            };
            current = current->next;
        };



//     // pars threw foundations
    
    for( int i = 0; i < 4; i++){
        int numID = rank_Hierarchy(f->foundations[i]->head->card->rank);
        int compare = 0; 
        // printf("%d, %d, %d, %c\n ",numID, compare, i, (f->foundations[i]));
        if(numID != -1){
            while(compare <=  numID){
               if( check_deck[compare][i] == 1){
                Node *newNode = malloc(sizeof(Node));
                newNode->card->rank = current->card->rank;
                newNode->card->suit = current->card->suit;
                newNode->card->faceDown = current->card->faceDown;
                addNode(doubledList, newNode);
               };
                check_deck[compare][i] = 1;
                compare++;
            };
        }
         while(compare <=  numID){
            printf("%c, %c\n",rank_Hierarchy(i) ,suit_Hierarchy(compare));
            // check_deck[rank_Hierarchy(i)][suit_Hierarchy(compare)] = 1;
            compare++;
        };
        // deckPosition = check_deck[rank_Hierarchy(current->card->rank)][suit_Hierarchy(current->card->suit)];
    };



// creates a linkedlist of missing cards for printing
    for(int i = 0 ; i < 4 ; i++){
        for(int j = 0 ; j < 13 ; j++){
            if(check_deck[j][i] == 0){
                Node *newNode = malloc(sizeof(Node));
                newNode->card = malloc(sizeof(Card));
                newNode->card->rank = rank_int_to_postion(j);
                newNode->card->suit = suit_int_to_position(i);
                addNode(missingList, newNode);
            };
        };
    };

    
    
    // //============================= print Check_Data
        #ifdef Print_Testing
        printf("\n");
         for(int i = 0 ; i < 4; i++)
        {
        for(int j = 0 ; j < 13; j++)
        {
            printf("%d, ",check_deck[j][i]);
        };
        printf("\n");
        };
        #endif

//     //============================= print for loops for testing
 
        int doubleInteget = 0;
        Node *curr = doubledList->head;
        if( curr != NULL){
            printf("Doubled cards ");
            while(curr != NULL)
            {
                printf("%c%c ", curr->card->rank,curr->card->suit);
                curr = curr->next;
                doubleInteget++;
            };
            exit(0);

        } 
        curr = missingList->head;
        int missingInteger = 0;
        if( curr != NULL){
            printf("missing cards ");
            while(curr != NULL)
            {
                printf("%c%c ", curr->card->rank,curr->card->suit);
                curr = curr->next;
                ++missingInteger;
            };
             exit(0);

        }

        if(missingInteger != 0 || doubleInteget != 0){
           // printf("Invalid file is invalid\n");
        }else{
            printf("Input file is valid\n");
            
             // table cardsa
              int s_count = 0;
             for(int i = 0; i<7; i++){
                 
                 Node *table_curr = table->tableList[i]->head;
                 while(table_curr != NULL){
                     if(table_curr->card->faceDown == 1){
                        s_count++;
                     }
                     table_curr= table_curr->next;
                 }
             }
             printf("%d covered cards\n",s_count);
            
            // stock cardsa
            curr = stock->list->head;
            s_count = 0;
            while(curr != NULL){
                 if(curr->card->faceDown == 1){
                    s_count++;
                 }
                curr = curr->next;
                
            }

            printf("%d stock cards\n", s_count);

             // waste cardsa
             curr = stock->list->head;
            s_count = 0;
             while(curr != NULL){
                 if(curr->card->faceDown == 0){
                    s_count++;
                 }
                curr = curr->next;
                
            }
            printf("%d waste cards\n", s_count);


        
        };
};

int suit_Hierarchy(char suit)
{
    //c,d,h,s
    switch (suit)
    {
    case 'c':
        return 0;
    case 'd':
        return 1; 
    case 'h':
        return 2;
    case 's':
        return 3; 
    };
    return -1;
};
int rank_Hierarchy(char rank)
{
    switch (rank){
        case 'A':
            return 0;
        case '2':
            return 1; 
        case '3':
            return 2;
        case '4':
            return 3;
        case '5':
            return 4; 
        case '6':
            return 5;
        case '7':
            return 6; 
        case '8':
            return 7;
        case '9':
            return 8; 
        case 'T':
            return 9;
        case 'J':
            return 10;
        case 'Q':
            return 11;
        case 'K':
            return 12;
        default:
            return -1;
    };
};
char suit_int_to_position(int suit)
{
    //c,d,h,s
    switch (suit)
    {
    case 0:
        return 'c';
    case 1:
        return 'd'; 
    case 2:
        return 'h';
    case 3:
        return 's'; 
    };
    return '@';
};
char rank_int_to_postion(int rank)
{
    switch (rank){
        case 0:
            return 'A';
        case 1:
            return '2';
        case 2:
            return'3';
        case 3:
            return '4';
        case 4:
            return'5';
        case 5:
            return'6';
        case 6:
            return '7';
        case 7:
            return '8';
        case 8:
            return '9';
        case 9:
            return 'T';
        case 10:
            return 'J';
        case 11:
            return 'Q';
        case 12:
            return 'K';
      
    };
    return -1;
};
 List* reverse_list(List *list){
    if(list->head == NULL){
        return list;
    }else if(list->head == list->tail){
        return list;
    }
    List * newList = malloc(sizeof(List));
    addNode(newList, list->tail);
    cut_tail(list);
    Node *curr = list->head;
    while(curr != newList->tail){
        addNode(newList, list->tail);
        cut_tail(list);
        if(list->head == list->tail){
            addNode(newList,list->tail);
            list->head = NULL;
            list->tail = NULL;
            break;
        };
    }
    newList->tail->next = NULL;

    // curr = newList->head;
    // while (curr != NULL){
    //     curr->card->faceDown = 1;
    //     curr = curr->next;
    // } ; 
    list->head = NULL;
    list->tail = NULL;
     free(list);
    list = newList;
    return newList;
}
void cut_tail(List *list){
    if(list->head != NULL){
        Node *curr = list->head;
        Node *prev = NULL;
        if(list->head == list->tail){
            list->head = NULL;
            list->tail = NULL;
        }else{
            while(curr->next->next != NULL){
                curr = curr->next;
                prev = curr;
            };
            curr->next = NULL;
            list->tail = curr;
        }

    }
}