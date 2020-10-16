#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "linkedlist.h"

//#define Print_Testing

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
typedef struct Game
{
    Rules *rules;
    Foundations *foundations;
    TABLEAU_T *table;
    stockCards *stock;
    Move_T *moves;
    int number_of_moves;
} Game_state;



void get_Next_Char(FILE* f);
void getRules(FILE* f, Rules * );
void getfoundations(FILE* inf, Foundations *);
void get_Colon(FILE *f);
void getTableaut(FILE *f, TABLEAU_T *, Foundations *);
int skip_space(char c);
void getStock(FILE *, stockCards *);
void getMoves(FILE*, Move_T*);

int suit_Hierarchy(char );
int rank_Hierarchy(char );

char suit_int_to_position(int);
char rank_int_to_postion(int);

void Check_Data(stockCards *, Foundations *, TABLEAU_T *);

int valid_move(char src, char dest, char dot, List *src_list, List *dest_list);
int play_one_move(char src, char dest, char dot,Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t);
int Check_Moves(Move_T* m);


void transplant_tail(List *src_list, List *dest_list, char selector);
void cut_tail(List *);
List* reverse_list(List *);
int opposite_color(Node *src, Node *dest);

void print_stdin(FILE *, int number_moves, Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *);
void print_exchange_formate(FILE *file_out, int number_moves ,Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *);
void newPrint(FILE *file_out, int number_moves, Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *str_invalid_moves);


int main (int argc, char *argv[]) {
    int N = -1;
    int ar = 1;
    int f_forced = 0, diagnostic = 0, exchange_formate = 0;

    FILE *file_out = stdout;
    FILE* infile = fopen(argv[1], "r");
    while(ar < argc ){
        if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'm'){
            N =  atoi(argv[ar+1]);
            ar++;
        }else if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'o'){
           file_out = fopen(argv[ar], "w");// file to out put
 
        }else if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'x'){
            exchange_formate = 1; // alternative formate to the file
            
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'c'){

        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'f'){
            f_forced = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'v'){
            diagnostic = 1;
        }else{
          infile = fopen(argv[ar], "r");
        };
         ar++;
    }
    //argv[1]

    int i = 0;
    // while(i < argc)
    // {
    //     printf("%s\n", argv[i]);
    //     i++;
    // }
    if(infile == NULL ){
        infile = stdin;
    }
    if (infile == NULL) {
        printf("No such file\n");
    }else{
        Rules *rule = malloc(sizeof(struct Rules));
        Foundations *foundations = malloc(sizeof(struct Foundations));
        TABLEAU_T *table = malloc(sizeof(struct TABLEAU));
        stockCards *stock = malloc(sizeof(struct Stock));
        Move_T *moves = malloc(sizeof(Move_T));
        
        getRules(infile, rule);
        getfoundations(infile, foundations);
        getTableaut(infile, table, foundations);
        getStock(infile,stock);
        getMoves(infile, moves);

        //Check_Data(stock,foundations, table);

        Node *curr = moves->list->head;
         char str_invalid_moves[255] = {0};

       // play a N moves
        if(N != -1){
            int i = 0;
            while(i < N){  
                if(!play_one_move(curr->moveData->src, curr->moveData->dest,curr->moveData->dot,rule,foundations,stock,table)){
                    if(curr->moveData->dot == '.')
                    {
                        strcat(str_invalid_moves,". ");
                    }
                    else  if(curr->moveData->dot == 'r')
                    {
                        strcat(str_invalid_moves,"r ");
                    }
                    else
                    {
                        
                        strcat(str_invalid_moves,&(curr->moveData->src)); // memory issue
                        strcat(str_invalid_moves,"->");
                        strcat(str_invalid_moves,&(curr->moveData->dest));
                        strcat(str_invalid_moves," ");

                    }
                }
                curr = curr->next;
                i++;
            }
        
        }else{
            while(curr != NULL){
                 play_one_move(curr->moveData->src, curr->moveData->dest,curr->moveData->dot,rule,foundations,stock,table);
                curr = curr->next;
            }
        }
        if(str_invalid_moves[0] == NULL){
            strcat(str_invalid_moves, " all valid");;
        }
        if (exchange_formate)
        {
            print_exchange_formate(file_out,N,rule,foundations,stock,table, str_invalid_moves);
        
        }else{
             print_stdin(file_out,N,rule,foundations,stock,table, str_invalid_moves);
        }
        



        // while(curr != NULL){
        //    if( curr->moveData->dot == '.' ||  curr->moveData->dot == 'r'){
        //         printf(" %c \n", curr->moveData->dot);

        //     }else{
        //         printf(" %c->%c \n",curr->moveData->src, curr->moveData->dest);
        //     }
        //     printf("%c%c%c\n",curr->moveData->src, curr->moveData->dest,curr->moveData->dot);
        //     play_one_move(curr->moveData->src, curr->moveData->dest,curr->moveData->dot,rule,foundations,stock,table);
            
        //     Node *stock_curr = stock->list->head;
        //     while(stock_curr != NULL){
        //         //printf(" %c%c.%d, ",stock_curr->card->rank,stock_curr->card->suit,stock_curr->card->faceDown);
        //         printf("%p\n", stock_curr);
        //         stock_curr = stock_curr->next;
        //     }
        //     print_stdin(file_out,N,rule,foundations,stock,table,str_invalid_moves);

        //     curr = curr->next;
        // };

        //Check_Moves(moves);
        // print_stdin(file_out,N,rule,foundations,stock,table);
        // print_exchange_formate(file_out,N,rule,foundations,stock,table);
        
       
    }

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
    char c;
     for(;;){
        get_Next_Char(inf); 
        c = fgetc(inf);
        if(c == ':'){
            return;
        }
     }
};
void getRules(FILE* inf, struct Rules *r){
    #ifdef Print_Testing
        printf("Rules\n" );
    #endif
   
    get_Next_Char(inf);
    get_Colon(inf);
    get_Next_Char(inf);
    
    char c;

    while(!isdigit(c)){
        c = fgetc(inf);
    }
   
    r -> turn = c - '0';
    get_Next_Char(inf);
    c = fgetc(inf);
    //printf("%c1\n", c);

    if(c != 'u'){
        while(!isdigit(c)){
         c = fgetc(inf);
        // printf("%c2\n", c);
        }
        //ungetc(c, inf);
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
        printf("Stock cards\n");
    #endif
    get_Colon(inf);
    get_Next_Char(inf);
    
    s->list = malloc(sizeof(List));
    char c = fgetc(inf);
    
    while('M' != c){ // new line
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
                Node *current = s->list->head;
                // while(current->card->faceDown != 1){
                while(current != NULL){
                    current->card->faceDown = 1;
                    current = current->next;
                };
                get_Next_Char(inf);

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

        #ifdef Print_Testing
            Node * curr = s->list->head;
            while(curr != NULL){
                printf("%c,%c,%d - ",curr ->card ->rank, curr->card->suit,curr->card->faceDown);  
                curr = curr->next; 
            }
           
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
//================= Project 2 data

void getMoves(FILE* inf, Move_T* m){
    #ifdef Print_Testing
        printf("\nMoves\n");
    #endif

    get_Colon(inf);
    get_Next_Char(inf);
    
    m->list = malloc(sizeof(List));
    char c = fgetc(inf);
    
    while(c != EOF){ // new line
        if(c == '#'){
            while(c != '\n'){
                c = fgetc(inf);
            };    
            ungetc(c, inf);
            get_Next_Char(inf);
        }else if(c == '\n' || c == '\r' || c == '-' || c == '>'){
            c = fgetc(inf); 
        } else if( c == ' '){
            c = fgetc(inf);
        }else{ // add card
            Node *newNode = malloc(sizeof(Node));
            newNode ->moveData = malloc(sizeof(Move_T));
            if(c == 'r'){
                newNode->moveData->dot = 'r';
            }else if(c == '.'){
                newNode->moveData->dot = '.';
            }else{
                newNode->moveData->src = c;
                get_Next_Char(inf);
                c = fgetc(inf);
                newNode->moveData->dest = c;
            }
            addNode(m->list, newNode);
            c = fgetc(inf);
          

    //+=================== Printing Moves 
        #ifdef Print_Testing
        
               if( newNode->moveData->dot == '.' ||  newNode->moveData->dot == 'r'){
                printf(" %c ", newNode->moveData->dot);

                }else{
                  printf(" %c->%c ",newNode->moveData->src, newNode->moveData->dest);
                }
        #endif
         
          
           
        };    
   };         

};


//Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, Move_T* m)
int Check_Moves(Move_T* m){
    Node * curr = m->list->head;
    while(curr != NULL){
        char check_r = curr->moveData->dot;
        if(check_r != '.' || check_r != 'r' || check_r != '\0'){
            printf("Err - Move is dot is invalid\n");
            return 0;
        }
        check_r = curr->moveData->src;
        if(check_r != 'w' || !isdigit(check_r)){
            printf("Err - Move is src is invalid\n");
            break;
        }
        check_r = curr->moveData->dest;
        if(check_r != 'f' || !isdigit(check_r)){
            printf("Err - Move is dest is invalid\n");
            return 0;
        }
    }
    return 1;
}
int play_one_move(char src, char dest, char dot,
Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t){
   //seach for the src and dest or dot pick a attion based on that
    List * src_list;
    List * dest_list;
    
    //set src_list;
    if(dot == '.'){
        src_list = s->list;
    }else if(dot == 'r'){
       
        src_list = s->list;
    }else if(src =='w'){
      
        src_list = s->list;
    }else{ 

        src_list = t->tableList[7-(src-'0')];

    }
   //set dest_list;
    if(dest =='f'){
        dest_list = f->foundations[suit_Hierarchy(src_list->tail->card->suit)];
    }else{

        dest_list = t->tableList[7-(dest-'0')];

    }

    if(valid_move(src, dest, dot, src_list,dest_list)){
        #ifdef Print_Testing
          printf("valid move\n");
        #endif
        
        if(dot == '.'){
            //truns over r->turn number of cards. if covered cards < r->turn then turns over only the covered cards 
            for(int i = 0 ; i < r->turn ; ++i){
               // if(valid_move(src, dest, dot , src_list,dest_list)){
                    Node *curr = src_list->head;
                    Node *prev = NULL;
                
                    if(curr->next == NULL){
                        curr->card->faceDown = 0;
                    }else{
                        while(curr->next != NULL && curr->next->card->faceDown == 1){
                            prev = curr;
                            curr = curr->next;
                            
                        }
                        if(curr == src_list->head){

                            src_list->head = curr->next;
                        }
                        prev->next = curr->next;
                        src_list->tail->next = curr;
                        src_list->tail = curr;
                        curr->card->faceDown = 0;

                        curr = src_list->head;
                            while(curr->next != NULL){
                                curr = curr->next;
                            }
                            src_list->tail = curr;

                    }
                    Node *current = src_list->head;
                    while(current != NULL){
                        printf("%c%c ",current->card->rank,current->card->suit);
                        current = current ->next;
                    }
                    printf("\n");

                    //tail pointer error test
                    // Node *current = src_list->head;
                    // while(current != NULL){
                    //     printf("%c%c, %p\n",current->card->rank,current->card->suit,current);
                    //     current = current ->next;
                    // }
                    // current = src_list->tail;
                    //  printf("%c%c, %p\n",current->card->rank,current->card->suit,current);
                    //exit(0);
              //  }
            }

        }else if(dot == 'r'){
            // this also sets all facedown to 1
            src_list = reverse_list(src_list);
             Node *curr = src_list->head;
            while (curr != NULL){
                curr->card->faceDown = 1;
                curr = curr->next;
            }; 
            
        }else if(dest == 'f'){
            transplant_tail(src_list, dest_list, 'f');
        }else if(dest == 'w'){
            transplant_tail(src_list, dest_list, 'w');
        }else{
            transplant_tail(src_list, dest_list, 't');
        }
        //#ifdef Print_Testing
            if(dot != NULL){
                printf("%c\n", dot);
            }else
                printf("%c->%c\n", src, dest);

            printf("Valid move\n");
            for(int i = 0 ; i < 4 ; i++){
            fprintf(stdout,"%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
            //printf("%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
            }
                printf("\n");
            newPrint(stdout,0,r,f,s,t,"yes");
            printf("top of stock %c%c\n", s->list->tail->card->rank, s->list->tail->card->suit);
       // #endif

        return 1;
    }
    //Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t){
    //#ifdef Print_Testing
        if(dot != NULL){
            printf("%c\n", dot);
        }else
            printf("%c->%c\n", src, dest);

        printf("invlaid move\n");
        for(int i = 0 ; i < 4 ; i++){
            fprintf(stdout,"%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
            //printf("%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
        }
        printf("\n");
        newPrint(stdout,0,r,f,s,t,"yes");
        printf("top of stock %c%c\n", s->list->tail->card->rank, s->list->tail->card->suit);
    //#endif
    return 0;
}
int valid_move(char src, char dest, char dot, List *src_list, List *dest_list){
        #ifdef Print_Testing
          printf("valid_move method \n");
        #endif
    if(dot == '.'){// flip stock
        if(src_list->head != NULL && src_list->head->card->faceDown == 1){ //head is face down you can flip a card
            return 1;
        }
        return 0;

    }else if( dot == 'r'){ // reset stock
        if(src_list->head != NULL && src_list->head->card->faceDown == 0){ //if head is fe down you cant reset stock
            return 1;
        }
        return 0; 
    
    }else if(src_list->head == NULL){ // err no node card to move
        return 0;
    }else if(dest == 'f'){ // card to foundation
        if(dest_list->head->card->rank == '_'){
            if(src_list->tail->card->rank == 'A'){
                //printf("A");
                return 1;
            }

        }else if(rank_Hierarchy(src_list->tail->card->rank) == rank_Hierarchy(dest_list->head->card->rank)+1){
            if(src_list->tail->card->suit == dest_list->tail->card->suit){
                return 1;
            }
        }
       // printf("ranks - %d,%d\n",rank_Hierarchy(src_list->tail->card->rank) , rank_Hierarchy(dest_list->head->card->rank));
        
        return 0;
    }else{// checks cards
        if(dest_list->head == NULL){
            Node *curr = src_list->head;
            while(curr != NULL){
                if(curr->card->rank == 'K'){
                    return 1;
                }
                curr = curr->next;
            }
             return 0;
        }else{
            Node *curr = src_list->head;
            Node *de = dest_list->head;
            int i = 0;
            while(curr != NULL){
                #ifdef Print_Testing
                    printf("dest_list %c%c\n",de->card->rank,de->card->suit);

                    printf("curr rank_H %d %c%c dest_list rank_H %d %c%c, face down : %d\n",
                    rank_Hierarchy(curr->card->rank),
                    curr->card->rank,curr->card->suit,rank_Hierarchy(dest_list->tail->card->rank)-1, dest_list->tail->card->rank,dest_list->tail->card->suit
                    , curr->card->faceDown);
                    i++;
                    printf("%d\n", i);
                #endif

                if(curr->card->faceDown == 0 && rank_Hierarchy(curr->card->rank) == rank_Hierarchy(dest_list->tail->card->rank)-1){
                    if(opposite_color(curr,dest_list->tail)){
                            return 1;
                    }
                }
            
                curr = curr->next;
                if(de->next != NULL)
                de = de->next;
            }
        }
        return 0;
    }        
    return 0;
}
// given to linked lists gets the last node and moves it to the next list
 void transplant_tail(List *src_list, List *dest_list, char selector){
    #ifdef Print_Testing
        printf("Transplant_tail");
        printf("%p\n",dest_list->head);
    #endif

    if(selector == 'f'){
        //printf("%c%c\n",src_list->head->card->rank,src_list->head->card->suit);
        
        free(dest_list->head->card);
        free(dest_list->head);

        
        dest_list->head = src_list->tail;
        dest_list->head->card->faceDown = 0;
        cut_tail(src_list);
        
        //get prev
        Node *curr = src_list->head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        if(curr != NULL){
            curr->card->faceDown = 0;
        }

    }else if(selector == 'w'){

      //  printf("stock card %c%c to card %c%c \n",src_list->tail->card->rank,src_list->tail->card->suit,dest_list->tail->card->rank,dest_list->tail->card->suit);
        addNode(dest_list, src_list->tail);
        cut_tail(src_list);
        if(dest_list->tail->card->faceDown != 0 ){
            dest_list->tail->card->faceDown = 0;
        };

    }else{
        
        Node *prev = NULL;
        Node *curr = src_list->head;
        if(dest_list->head == NULL){
            while(curr->next != NULL){
                if(curr->card->rank == 'K'){
                prev->card->faceDown = 0;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
            prev->next = NULL;
            src_list->tail = prev;
            dest_list->head = curr;
            
            
        }else{
        
            while(curr != NULL){
                //printf("%c%c - > %c%c\n",curr->card->rank ,curr->card->suit,dest_list->tail->card->rank,dest_list->tail->card->suit);

                if(curr->card->faceDown == 0 && rank_Hierarchy(curr->card->rank) == rank_Hierarchy(dest_list->tail->card->rank)-1){
                
                    if(opposite_color(curr,dest_list->tail)){
                        if(curr == src_list->head){
                            //printf("src head case tasplant");
                            src_list->head = NULL;
                            src_list->tail = NULL;

                            dest_list->tail->next = curr;
                            
                            Node * current = curr;
                            while(current->next != NULL){
                                current = current->next;
                            }
                            dest_list->tail = current;

                            curr->card->faceDown = 0;
                        }else{

                            if(dest_list->tail == NULL){
                                dest_list->head = curr;
                            }else{
                                dest_list->tail->next = curr;
                            }
                            Node * current = curr;
                            while(current->next != NULL){
                                current = current->next;
                            }
                            dest_list->tail = current;

                            src_list->tail = prev;
                            prev->next = NULL;

                            prev->card->faceDown = 0;
                        }
                    }
                }else{
                    #ifdef Print_Testing
                    printf("this failed in transplant\n");
                    #endif
                }

                prev = curr;
                curr = curr->next;
                
            } 
        }
    }
}
// tested and works 
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

   
    list->head = NULL;
    list->tail = NULL;
     free(list);
    list = newList;
    return newList;
}
int opposite_color(Node *src, Node *dest){
    char color_src;
    char collor_dest;

    // gets colors 
    if(src->card->suit == 'c'|| src->card->suit == 's'){
        color_src = 'b';
    }else {
        color_src = 'r';
    }
    if(dest->card->suit == 'c'|| dest->card->suit == 's'){
        collor_dest = 'b';
    }else {
        collor_dest = 'r';
    }
    // checks for opposite color 
    if( color_src != collor_dest){
        return 1;
    }
    return 0;


}

void print_stdin(FILE *file_out, int number_moves, Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *str_invalid_moves){
    fprintf(file_out,"Processed %d moves,%s", number_moves, str_invalid_moves);
    //ilegal moves 
    fprintf(file_out,"\n");
    
    printf("Foundations\n");
    for(int i = 0 ; i < 4 ; i++){
        fprintf(file_out,"%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
        //printf("%c%c ", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
    }
    fprintf(file_out,"\n");
    fprintf(file_out,"Tableau\n");

     newPrint(file_out, number_moves, r, f, s, t,str_invalid_moves);
     // waste
     fprintf(file_out,"Waste Top\n");
     if(s->list->tail == NULL){
         fprintf(file_out,"(empty)\n");
     }else{
        fprintf(file_out,"%c%c \n",s->list->tail->card->rank,s->list->tail->card->suit);
     }
     
     
}
void newPrint(FILE *file_out, int number_moves, Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *str_invalid_moves){
    char print_array[14] = "";
    int array_index = 0;
    int row = 0;
     while(row < 13){
        if(row == 0){
            for(int i = 0 ; i < 7 ; i++){
                if(t->tableList[6-i]->head == NULL){
                     //fprintf(file_out,".. ");
                     print_array[array_index] = '.';
                     print_array[array_index+1] = '.';

                     array_index+=2;
                }else if(t->tableList[6-i]->head->card->faceDown == 1){
                    //fprintf(file_out,"## ");
                    print_array[array_index] = '#';
                    print_array[array_index+1] = '#';
                    array_index+=2;
                }else{
                    //fprintf(file_out,"%c%c ", t->tableList[6-i]->head->card->rank,t->tableList[6-i]->head->card->suit);
                    print_array[array_index] = t->tableList[6-i]->head->card->rank;
                    print_array[array_index+1] = t->tableList[6-i]->head->card->suit;
                    array_index+=2;
                }
            }
            array_index = 0;
            row++;
        }else{
            for(int i = 0 ; i < 7 ; i++){
                Node *curr = t->tableList[6-i]->head;
                int num = 0;
                while(num <= row){
                    if(curr == NULL){
                        //fprintf(file_out,".. ");
                        print_array[array_index] = '.';
                        print_array[array_index+1] = '.';
                        array_index+=2;
                        break;
                    }else if(num == row){
                        if(curr->card->faceDown == 1){
                            //fprintf(file_out,"## ");
                            print_array[array_index] = '#';
                            print_array[array_index+1] = '#';
                            array_index+=2;

                        }else{
                           // fprintf(file_out,"%c%c ", curr->card->rank, curr->card->suit);
                            print_array[array_index] = curr->card->rank;
                            print_array[array_index+1] = curr->card->suit;
                            array_index+=2;   
                        }
                        break;
                    }else{
                        curr = curr->next;
                        num++;
                    }
                }
            }
            array_index = 0;
            row++;
        }
         // printf("%s\n", print_array);
        // check if true print row else dont 
        int bool = 0;
           for(int i = 0 ; i < 14; i++){
                if(print_array[i] != '.'){
                    bool = 1;
                    break;
                }
            }
        if(bool){
            for(int index = 0, i = 0 ; i < 7; i++ ){
                fprintf(file_out,"%c%c ", print_array[index], print_array[index+1]);
                index+=2;
            }
            fprintf(file_out,"\n");
        }
        for(int i = 0 ; i<14; i++){
        print_array[i] = 0;
        }
         
       // bool = 0;
     }
}
void print_exchange_formate(FILE *file_out, int number_moves ,Rules * r, Foundations * f, stockCards * s, TABLEAU_T * t, char *str_invalid_moves){
     fprintf(file_out,"Processed %d moves,%s", number_moves,str_invalid_moves);
    //ilegal moves 
    fprintf(file_out,"\n");
    
    fprintf(file_out, "RULES :\n");

    fprintf(file_out, "turn %d\n", r->turn);
    if(r->limit == -1){
        fprintf(file_out, "unlimited\n");  
    }else{
        fprintf(file_out, "turn %d\n", r->limit);
    }

    //foundation
    fprintf(file_out, "FOUNDATIONS :\n");
    for(int i = 0 ; i < 4l; i++){
        fprintf(file_out, "%c%c\n", f->foundations[i]->head->card->rank,f->foundations[i]->head->card->suit);
    }
    
    fprintf(file_out, "TABLEAU :\n");
    for(int i = 0 ; i < 7 ; i++){
        
        Node * curr = t->tableList[i]->head;
        int print_facedown = 0;
        while(curr!= NULL){
            if(curr->card->faceDown == 0 && print_facedown == 0){
             print_facedown = 1;
             //print |
              fprintf(file_out, "| ");
             // card
             fprintf(file_out, "%c%c ", curr->card->rank, curr->card->suit);
            }else{
                fprintf(file_out, "%c%c ", curr->card->rank, curr->card->suit);
            }
             curr = curr->next;
        }
        fprintf(file_out, "\n");

    }

    fprintf(file_out, "STOCK :\n");
        
        Node * curr = s->list->head;
        int print_facedown = 0;
        while(curr!= NULL){
            if(curr->card->faceDown == 0 && print_facedown == 0){
             print_facedown = 1;
             //print |
              fprintf(file_out, "| ");
             // card
             fprintf(file_out, "%c%c ", curr->card->rank, curr->card->suit);
            }else{
                fprintf(file_out, "%c%c ", curr->card->rank, curr->card->suit);
            }
            curr = curr->next;

        }
    fprintf(file_out, "\n");


    fprintf(file_out, "MOVES :\n");


    
}
//===================================== project 3


// main method 
void check_for_winnable_game(Game_state *state,int number_moves ,int forced){
//    Game_state *final = method(state, 1 ,number_moves, forced);
//     if(check_if_game(final) == 1){
//         //winnable
//         fprintf(stdout,"# Game is winnable in %d moves", final->number_of_moves);
//     }else{
//         //not winable
//         fprintf(stdout,"# Game is not winnable within %d moves", final->number_of_moves);
//     }
}

Game_state * method(Game_state *state, int start_move,int number_moves , int forced){
    // if(forced == 1){
    //     // check stock to foundation only top cards
    //     // check table to foundation only top cards

    //     //check for forced move
    // }
    // //check for other move
    return state;
}

// helper method 
//a game is guaranteed to
//be winnable if there are no covered cards, no stock cards, and at most one waste card. This should be the
//condition that you search for.


int check_if_game(Game_state *state){
  
  // check stock
    if(state->stock->list->head->card->faceDown != 0 && 
    state->stock->list->head->next != NULL){
        return 0;
    }
    // check table
    for(int i = 0 ; i < 7 ; i++){
        List* row = state->table->tableList[i];
        Node* curr = row->head;
        while(curr != NULL){
            if(curr->card->faceDown == 1){
                return 0;
            }
            curr = curr->next;
        }
    }
    //check foundation
    // no need to check
    return 1;
}

Game_state* copy_game_state(Game_state *st){
    Game_state *newState = malloc(sizeof(Game_state));
    newState->rules = malloc(sizeof(struct Rules));
    newState->foundations = malloc(sizeof(struct Foundations));
    newState->table = malloc(sizeof(struct TABLEAU));
    newState->stock = malloc(sizeof(struct Stock));
    newState->moves = malloc(sizeof(Move_T));
    newState->number_of_moves = st->number_of_moves;
    
    //copy rules
    newState->rules->limit = st->rules->limit;
    newState->rules->turn = st->rules->turn;
    //copy foundations
    for(int i = 0 ; i < 4; i++){
        List *list = malloc(sizeof(List));
        Node *node = malloc(sizeof(Node));
        node->card = malloc(sizeof(Card));
        list->head = node;
        list->tail = node;
        newState->foundations->foundations[i] = list;
        list->head->card->rank = st->foundations->foundations[i]->head->card->rank;
        list->head->card->suit = st->foundations->foundations[i]->head->card->suit;
        list->head->card->faceDown = 0;
    }
    
    //copy table
     for(int i = 0 ; i < 7; i++){
        List *list = malloc(sizeof(List));
        newState->table->tableList[i] = list;
        if(st->table->tableList[i]->head == NULL){
            // node is null
           
        }else{
            //add node

            Node *curr = st->table->tableList[i]->head;
             while(curr != NULL){
                Node *node = malloc(sizeof(Node));
                node->card = malloc(sizeof(Card));
                node->card->suit = curr->card->suit;
                node->card->rank = curr->card->rank;
                node->card->faceDown = curr->card->faceDown;
                
                addNode(list,node);


                curr = curr->next;
            }
        }
    }
    //copy stock
        List *list = malloc(sizeof(List));
       
        Node *curr = st->stock->list->head;
            while(curr != NULL){
            Node *node = malloc(sizeof(Node));
            node->card = malloc(sizeof(Card));
            node->card->suit = curr->card->suit;
            node->card->rank = curr->card->rank;
            node->card->faceDown = curr->card->faceDown;
            
            addNode(list,node);


            curr = curr->next;
        }
        newState->stock->list = list;



    //copy moves
        // if i want to save moves made



    return newState;
}

Game_state *check_for_move(Game_state *st, char if_f){
    Game_state *newState = malloc(sizeof(Game_state));
    newState->rules = malloc(sizeof(struct Rules));
    newState->foundations = malloc(sizeof(struct Foundations));
    newState->table = malloc(sizeof(struct TABLEAU));
    newState->stock = malloc(sizeof(struct Stock));
    newState->moves = malloc(sizeof(Move_T));
    newState->number_of_moves = st->number_of_moves;
    
    // check stock

    // check table

    //check winable


    return newState;
}
// //this method checks if the top most stock card can move to the table or to the foundations 
// char check_if_node_is_moveable_stock(Game_state *st, List src_list, Node *src_node){
//     // check foundations
//     List *foundation = st->foundations->foundations[suit_Hierarchy(src_node->card->suit)];
//         if(foundation->head->card->rank == '_'){
//             if(src_node->card->rank == 'A'){
//                 //printf("A");
//                 return 1;
//             }

//         }else if(rank_Hierarchy(src_node->card->rank) == rank_Hierarchy(foundation->card->rank)+1){
//             if(src_node->card->suit == foundation->head->card->suit){
//                 return 1;
//             }
//         }
//         /// done checking foundations 

//         // check table

//         for(int i = 0 ; i < 7 ; i++){
//          Node *curr = st->table->tableList[i]->head;
//             int i = 0;
//             while(curr != NULL){

//                 #ifdef Print_Testing
//                     printf("dest_list %c%c\n",de->card->rank,de->card->suit);

//                     printf("curr rank_H %d %c%c dest_list rank_H %d %c%c, face down : %d\n",
//                     rank_Hierarchy(curr->card->rank),
//                     curr->card->rank,curr->card->suit,rank_Hierarchy(dest_list->tail->card->rank)-1, dest_list->tail->card->rank,dest_list->tail->card->suit
//                     , curr->card->faceDown);
//                     i++;
//                     printf("%d\n", i);
//                 #endif

//                 if(curr->card->faceDown == 0 && rank_Hierarchy(curr->card->rank) == rank_Hierarchy(src_node->card->rank)-1){
//                     if(opposite_color(curr,src_node)){
//                             return 1;
//                     }
//                 }
            
//                 curr = curr->next;
//             }
//         }






//     return 0;
// }
// //this method checks if a card in the table can move to other tables or the fondations
// int check_if_node_is_moveable_table(Game_state *st, List src_list, Node *src_node){

//    List *foundation = st->foundations->foundations[suit_Hierarchy(src_node->card->suit)];
//     if(src_node->next == NULL){
//         if(foundation->head->card->rank == '_'){
//             if(src_node->card->rank == 'A'){
//                 //printf("A");
//                 return 1;
//             }

//         }else if(rank_Hierarchy(src_node->card->rank) == rank_Hierarchy(foundation->card->rank)+1){
//             if(src_node->card->suit == foundation->head->card->suit){
//                 return 1;
//             }
//         }
//     }
//      /// done checking foundations 
//      for(int i = 0 ; i < 7; i++){
//          List *table_list = st->table->tableList[i];
//          if(&table_list != &src_list){
//                Node *curr = st->table->tableList[i]->head;
//             int i = 0;
//             while(curr != NULL){

//                 #ifdef Print_Testing
//                     printf("dest_list %c%c\n",de->card->rank,de->card->suit);

//                     printf("curr rank_H %d %c%c dest_list rank_H %d %c%c, face down : %d\n",
//                     rank_Hierarchy(curr->card->rank),
//                     curr->card->rank,curr->card->suit,rank_Hierarchy(dest_list->tail->card->rank)-1, dest_list->tail->card->rank,dest_list->tail->card->suit
//                     , curr->card->faceDown);
//                     i++;
//                     printf("%d\n", i);
//                 #endif

//                 if(curr->card->faceDown == 0 && rank_Hierarchy(curr->card->rank) == rank_Hierarchy(src_node->card->rank)-1){
//                     if(opposite_color(curr,src_node)){
//                             return 1;
//                     }
//                 }
            
//                 curr = curr->next;
//             }


//          }
//      }

//     return 0;
// }

