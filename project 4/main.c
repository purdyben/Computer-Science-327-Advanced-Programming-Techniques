#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//#define Print_Testing;


char* card[] = {"0","Ac", "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ad", "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd",
"Ah", "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh","As", "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks"};

long hashset[10000000][2] = {0};
int hashset_index = 0;
int cache_n = 0;

typedef struct rule{
    int turn;
    int limit;
}rules;
typedef struct game
{
    int foundations[4];
    int stock[52];
    int stock_up_index;
    int table[7][21];
    int table_facedown_index[7];
    char move_Set[2000];

} game_state;

char get_Next_Char(FILE* inf);
int hash_card(char rank, char suit);
char check_rules(char c);
char check_founcations(char c);
char check_table(char c);
char check_stock(char c);

char get_founcations(FILE* inf, game_state *st);
char get_rules(FILE* inf, rules *r);
char get_table(FILE* inf, game_state *st);
char get_stock(FILE* inf, game_state *st);

int suit_Hierarchy(char suit);
int rank_Hierarchy(char rank);

char parse_file(FILE* inf, rules *r, game_state * st);

void get_Colon(FILE* inf);
char* get_card(int hash_card);

void DFS(game_state *state,rules *r, int max_number, int v, int c);
int DFSUtil(game_state *state, rules *r, int move_num,int max_number, int *depth, int can_referesh_stock, int v, int c);
int check_game(game_state *state);
game_state* copy_game_state(game_state *st);
int DFS_table(game_state *state,rules *r, int move_num, int max_number, int *depth, int can_referesh_stock, int v, int c);
int DFS_stock(game_state *state,rules *r, int move_num,int Max_number, int *depth, int can_referesh_stock, int v, int c);
int check_stock_to_foundations(game_state *state);
int check_stock_to_table(game_state *state);

void print_std(FILE *file_out, int number_moves, game_state *state,rules *r); 

int check_opposite_suits(char s1, char s2);
int check_flip_card(game_state *state);
int check_flip_stock(game_state *state);
int compress_stock(game_state *st);

void reset_stock_index(game_state *state);
void reset_table_index(game_state *state, int row);

int check_card_and_table(char *c1, char *c2);
int table_card_to_foundation_card(char* t1, char *f2);
void move_t_to_t(game_state *state, int src_row, int src_col, int des_row, int des_col);
int check_tc_to_tc(char *t1, char *t2);
void add_c_to_move_set(game_state *state, char move);

int hash_table(game_state *state);
int check_hash_table(long n, int x);

int main (int argc, char *argv[]) {
    int N = 1000;
    int ar = 1;
    
    int f_forced = 0, diagnostic = 0, c = 0, exchange_formate = 0;

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
            c = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'f'){
            f_forced = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'v'){
            diagnostic = 1;
        }else{
          infile = fopen(argv[ar], "r");
        };
         ar++;
    }
    if(infile == NULL ){

        infile = stdin;
    }
    if (infile == NULL) {

        printf("No such file\n");

    }else{
        rules *rules = malloc(sizeof(rules));
        game_state *state = malloc(sizeof(game_state));
        parse_file(infile,rules, state);
 
        DFS(state,rules,N,diagnostic,c);
        

    }
    
}

char* get_card(int hash_card){
    if(hash_card == -1){
        return "-1";
    }
    return card[hash_card];
}

int hash_card(char rank, char suit){
    int sh =  suit_Hierarchy(suit);
    int r = rank_Hierarchy(rank);

    return sh*13+r+1;  
}
char get_Next_Char(FILE* inf){
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
    if (EOF == c) return 0;
    break;
  }
  //printf("%c\n", c);
  return c;
    //ungetc(c, inf);
};

void get_Colon(FILE* inf){
    char c;
     for(;;){
        c = get_Next_Char(inf); 
        // c = fgetc(inf);
        if(c == ':'){
            return;
        }
     }
};

char parse_file(FILE* inf, rules *r, game_state * st){
    char c = get_Next_Char(inf);
    if(check_rules(c)){
        get_rules(inf, r);

    }else{
        return 0;
    }
    if(check_founcations(c)){
       c = get_founcations(inf, st);
        
        // for(int i = 0 ; i < 4; i++){
        //     printf("%d %s, ", st->foundations[i],get_card( st->foundations[i])  );
        // }
        // printf("\n");
    }else{
        return 0;
    }
    if(check_table(c)){
     c= get_table(inf, st);
    }else{
        return 0;
    }
    // return3 1;
    if(check_stock(c)){
        get_stock(inf, st);
    }else{
        return 0;
    }

    return 1;
}

char check_rules(char c){
    if(c == 'R'){
        return '1';
    };
    return '0';
}
char get_rules(FILE* inf, rules *r){
    get_Colon(inf);
    char c = 'a';
    while(c != 'F'){
        if(!isdigit(c)){
            c = get_Next_Char(inf);
        }else{
            r->turn = c-'0';
            break;
        }    
    }
    c = 'a';
    while(c != 'F'){
        //printf("%c\n", c );
        if(c == 'u'){
            r->limit = -1;
            break;
        }else if(!isdigit(c)){
           c = get_Next_Char(inf);
    
        }else{
            r->limit = c-'0';
            break;
        }
        
    }  
    return 0;
}

char check_founcations(char c){
    if(c == 'F'){
        return '1';
    };
    return '0';
}

char get_founcations(FILE* inf, game_state *st){
    get_Colon(inf);
    char rank = 0;
    char c = 0; 
    int index = 0;

    while(index < 4){ 
        rank = get_Next_Char(inf);
        c = get_Next_Char(inf); 
        if(rank == '_'){
            st->foundations[index] = 0;
        }else{
            st->foundations[index] = (hash_card(rank, c)-1) % 13+1;
        }
        index++;
    } 
    while(c != 'T'){
        c = fgetc(inf);
    }
    return c;  
}

char check_table(char c){
    if(c == 'T'){
        return '1';
    };
    return '0';
}

char get_table(FILE* inf, game_state *st){
    get_Colon(inf);
    char rank = 'x';
    char suit = 'x';
    char c = 'x';
    // suck it 
    
    for(int i = 0 ; i<7; i++){
        int index = 0;
        c = get_Next_Char(inf);
        //printf("%c",c );
    // suck it 
        while(c != '\n'){ // new line
            if(c == '#'){
                while(c != '\n'){
                    c = fgetc(inf);
                };
                //ungetc(c,inf);
                break;
            }else if(c == ' ' || c == '\r' || c == '\t'){
                // do nothing 
            }else if( c == '|'){
                 st->table_facedown_index[i] = index-1;
             }else {
                 // found a card char
                 if(rank == 'x'){
                    rank = c;
                 }else{
                     //printf("%c%c", rank,c);
                    st->table[i][index] = hash_card(rank, c);
                    rank = 'x';
                    suit = 'x';
                    index++;
                 }
            }
            
           // printf("%c",c);
             c = fgetc(inf);
         }
    }
    return c;
}

char check_stock(char c){
    if(c == 'S'){
        return '1';
    };
    return '0';
}

char get_stock(FILE* inf, game_state *st){
    get_Colon(inf);
    char rank = 'x';
    char suit = 'x';
    char c = 'x';
    int index = 0;
    c = get_Next_Char(inf);
    
    while(c != 'M' && c != EOF){
            if(c == '#'){
                while(c != '\n'){
                    c = fgetc(inf);
                };
                break;
            }else if(c == '\n' || c == ' ' || c == '\r' || c == '\t'){
                // do nothing 
            }else if( c == '|'){
                st->stock_up_index = index-1;
            }else {
                // found a card char
                if(rank == 'x'){
                    rank = c;
                }else{
                    if(c == ' '){
                        c = '-';
                    }
                    st->stock[index] = hash_card(rank, c);
                    rank = 'x';
                    suit = 'x';
                    index++;
                }
            }
            c = fgetc(inf);
        }

    return c;
}

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
void print_std(FILE *file_out, int number_moves, game_state *state,rules *r){  
    
    fprintf(file_out,"move number %d ", number_moves);
    //ilegal moves 
    fprintf(file_out,"\n");

    printf("Foundations\n");
    for(int i = 0 ; i < 4 ; i++){
        char * s = get_card(state->foundations[i]); 
        if(state->foundations[i] == 0){
            if(i == 0){
                fprintf(file_out,"_c ");
            }else if(i == 1){
                fprintf(file_out,"_d ");
            }else if(i == 2){
                fprintf(file_out,"_h ");
            }else if(i == 3){
                fprintf(file_out,"_s ");
            }
        }else{
            fprintf(file_out,"%s ", get_card(state->foundations[i] + i*13));
        }
    }
 
    fprintf(file_out,"\n");

    // for(int i = 0; i < 7 ; i++){
    //     for(int j = 0 ; j < 21; j++){
    //         if(strncmp(get_card(state->table[i][j]), "0",2)){
    //             fprintf(file_out,"%s ", get_card(state->table[i][j]));
    //         }
    //     }
    //     fprintf(file_out,"\n");
    // }

    fprintf(file_out,"\n");
    //table
    int check = 0;
    for(int j = 0 ; j < 21; j++){
        int check = 0;
        for(int i = 0; i < 7 ; i++){
            if( !strncmp(get_card(state->table[i][j]), "0",2)){
                 fprintf(file_out,".. ");
            }
            else if( strncmp(get_card(state->table[i][j]), "0",2) && j <= state->table_facedown_index[i]){
                fprintf(file_out,"## ");
                check = 1;
            }else if(strncmp(get_card(state->table[i][j]), "0",2)){
                fprintf(file_out,"%s ", get_card(state->table[i][j]));
                check = 1;
            }
        }
        if(check == 1){
            fprintf(file_out,"\n");
            check = 0;
        }else {
             fprintf(file_out,"\n");
            //break;
        }

    }

    printf("\nstock\n");
    //table stock
    for(int i = 0 ; i < 52 ; i++){
        if(strncmp(get_card(state->stock[i]), "0",2)){
            fprintf(file_out,"%s ", get_card(state->stock[i]));
        }
    }
    fprintf(file_out,"\n");
    if(state->stock_up_index == -1){
            for(int i = 0 ; i < 52 ; i++){
                if(!strncmp(get_card(state->stock[i]), "0",2)){
                    break;
                }else{
                fprintf(stdout,"## ");
                }
            }
    }
    else{
    //     // !strncmp(get_card(state->stock[i]), "0",2)
        for(int i = 0 ; i < 52 ; i++){
            if((strncmp(get_card(state->stock[i]), "0",1) && i > state->stock_up_index)){
                fprintf(stdout,"## ");
            }else if(strncmp(get_card(state->stock[i]), "0",1)){
                fprintf(stdout,"%s ", get_card(state->stock[i]));
            }
        }   
    }
    
    printf("\n");
}

void add_c_to_move_set(game_state *state, char move){
    char c = state->move_Set[0];
        int index = 0;
        while(c != '\0' && index < 1000){
            index++;
             c = state->move_Set[index];
        }
        state->move_Set[index] = move;
}

void DFS(game_state *state,rules *r, int max_number, int v, int c){
    int depth = 1;
    fprintf(stdout,"Using DFS search\n");

    int x = state->stock[cache_n];
    while(x != 0){
        cache_n++;
        x = state->stock[cache_n];
    }
    //printf("----------------------------------------\n");
    DFSUtil(state,r,0,max_number, &depth, 0,v,c);
    if(v == 1){
        //change later 
        fprintf(stdout,"%d Configurations checked\n", depth);
    }
    if(c == 1){
        fprintf(stdout, "Cache had %d entries.\n",cache_n);
    }
    fprintf(stdout,"Game is not winnable within %d moves\n", max_number);
    //free(state);

}

int DFSUtil(game_state *state, rules *r, int move_num,int max_number, int *depth, int can_referesh_stock, int v, int c){
    //printf("--------------------------\n");
    //print_std(stdout,move_num,state,r);
    // char a[16] = {'7','f','6','4','6','f','1','7','1','f','1','f','7','1','7','3'};
    // if(!strncmp(state->move_Set,a,16)){
    //     exit(0);
    // }
        if(0){
            char x = state->move_Set[0];
            int index = 0;
            while(x != '\0'){
                if(x == '.'){
                    fprintf(stdout,". ");
                }else if(x == 'r'){
                    fprintf(stdout,"r ");
                }else{
                    fprintf(stdout,"%c->",x);
                        index++;
                        x  = state->move_Set[index];
                    fprintf(stdout,"%c ",x);
                }
                index++;
                x = state->move_Set[index];

            }
            printf("\n");
        }
        
        // if(state->move_Set[14] == '7'  && state->move_Set[15] == '3' ){
        //     print_std(stdout,4,state,r);
        //     exit(0);
        // }
    //=========================================================================
    if(move_num >= max_number){
        return 0;
    }
    if(v == 1){
        if(*depth % 100000  == 0){
            printf("%d configurations checked so far\n", *depth);
        }   
    }
    if(check_game(state) == 1){
        
         //print_std(stdout,move_num,state,r);
        if(v == 1){
            fprintf(stdout,"%d Configurations checked\n", *depth);
        }

        if(c == 1){
            fprintf(stdout, "Cache had %d entries.\n",cache_n);
        }

        fprintf(stdout,"# Winnable in %d moves\n", max_number);

        char c = state->move_Set[0];
        int index = 0;
        while(c != '\0'){
            if(c == '.'){
                fprintf(stdout,".\n");
            }else if(c == 'r'){
                fprintf(stdout,"r\n");
            }else{
                fprintf(stdout,"%c->",c);
                    index++;
                    c = state->move_Set[index];
                 fprintf(stdout,"%c\n",c);
            }
            index++;
             c = state->move_Set[index];

        }

        //print_std(stdout,4,state,r);

        exit(0);
    }

 
    DFS_table(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    DFS_stock(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    // DFS_table(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    // DFS_stock(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    // DFS_table(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    // DFS_stock(state,r,move_num,max_number,depth,can_referesh_stock,v,c);
    
    return 1;
}

int DFS_table(game_state *state,rules *r, int move_num,int max_number, int *depth, int can_referesh_stock, int v, int c){
   
    for(int i = 0; i< 7 ;i++){
        int table_card = state->table[i][0];
        int col = 0;
        if(table_card != 0){
            while(table_card != 0 && col < 21){
                col++;
                table_card = state->table[i][col];
            }
            table_card = state->table[i][col-1];
           // printf("%s, %s", get_card(table_card), get_card(card));
            
    
            int f_card = state->foundations[suit_Hierarchy(get_card(table_card)[1])];
               if(f_card+1 == (table_card-1)%13+1){
                // make move
                //printf("move t to foundations \n");
                 
                game_state *new_state = copy_game_state(state);
                if(new_state->table_facedown_index[i] != -1 && new_state->table_facedown_index[i]+1 == col-1){
                    new_state->table_facedown_index[i]--;
                }
                new_state->foundations[suit_Hierarchy(get_card(table_card)[1])]++;

                new_state->table[i][col-1] = 0;
                //reset_table_index(state,i);

                add_c_to_move_set(new_state,(7-i)+'0');
                add_c_to_move_set(new_state, 'f');

                *depth = *depth + 1;
                //printf("t -> foundations\n");
                
                int n = DFSUtil(new_state,r,move_num+1,max_number,depth,can_referesh_stock,v,c);
                free(new_state);
                new_state = NULL;
       
            }    
        }
    }

      //check tables 
    //printf("check table\n");
    for(int row = 0; row< 7 ;row++){
        if(state->table[row][0] != 0){
            int table_card = state->table[row][0];
            int index = 0;
            while(table_card != 0){
                index++;
                table_card = state->table[row][index];
        
            }
            table_card = state->table[row][index-1];

            // go through the whole list
            int card;
            
            for(int i = 0 ; i < 7 ; i++){
                for(int j = 0 ; j < 21 ; j++){
                    int card = state->table[i][j];
                    //printf("--begin\n");
                    if(j > state->table_facedown_index[i] && check_tc_to_tc(get_card(card), get_card(table_card))){
                        //move list to table card

                            // bann last card moved from if it doesnt uncover a card
                        
                        game_state *new_state = copy_game_state(state);
                       // printf("%p %d %d %d %d\n",new_state,i,j,row,index);
                        move_t_to_t(new_state,i,j,row,index);
                    
                        long n = hash_table(new_state);
                        
                        if(move_num < 2 || !check_hash_table(n,move_num)){
                            cache_n++;

                            hashset[hashset_index][0] = n;
                            hashset[hashset_index][1] = move_num;

                            hashset_index++;
                            
                            *depth = *depth + 1;
                            
                            add_c_to_move_set(new_state,(7-i)+'0');
                            add_c_to_move_set(new_state, (7-row)+'0');
                            //printf("t->  table\n");
                            int n = DFSUtil(new_state,r,move_num+1,max_number,depth,can_referesh_stock,v,c);
                            free(new_state);
                            //new_state = NULL;
                        }  
                    }  
                    //printf("end --\n"); 
                }
            }
        }
    }

    
    return 1;
}

int DFS_stock(game_state *state,rules *r, int move_num,int max_number, int *depth, int can_referesh_stock, int v, int c){
    //segfault please pretty please 


    if(check_flip_stock(state) && can_referesh_stock == 0){
        game_state *new_state = copy_game_state(state);
        new_state->stock_up_index = -1;
        add_c_to_move_set(state,'r');

        *depth = *depth + 1;
        //print_std(stdout,4,state,r);
        //printf(" flip stock\n");
        int n = DFSUtil(new_state,r,move_num+1,max_number,depth,can_referesh_stock,v,c);
        free(new_state);
        new_state = NULL;
    }
    if(can_referesh_stock == 1){
        can_referesh_stock = 0;
    }
    //printf("check flip card\n");
    if(check_flip_card(state)){
        game_state *new_state = copy_game_state(state);
        new_state->stock_up_index = new_state->stock_up_index+1;
        add_c_to_move_set(new_state,'.');
        *depth = *depth + 1;
        //  printf(" flip card\n");
        DFSUtil(new_state,r,(move_num+1),max_number,depth,can_referesh_stock,v,c);
        free(new_state);
        new_state = NULL;
    }
    //printf("check flip foundations\n");
    if(check_stock_to_foundations(state)){
       // printf("move to foundations\n");
        
        game_state *new_state = copy_game_state(state);
        // move card to foundation
        new_state->foundations[suit_Hierarchy(get_card(new_state->stock[new_state->stock_up_index])[1])] = new_state->stock[new_state->stock_up_index];
        // remove from stock;
        new_state->stock[new_state->stock_up_index] = 0; 
        reset_stock_index(new_state);
        // compress sock
        compress_stock(new_state);

        add_c_to_move_set(new_state,'w');
        add_c_to_move_set(new_state,'f');

        *depth = *depth + 1;
        //printf(" c-> foundations \n");
        int n = DFSUtil(new_state,r,move_num+1,max_number,depth,can_referesh_stock,v,c);
        free(new_state);
        new_state = NULL;
   }
    //printf("check flip table\n");
    if(check_stock_to_table(state)){
       // printf("can move to table\n");
           game_state *new_state = copy_game_state(state);
        int s_card = state->stock[state->stock_up_index];
        for(int i = 0; i< 7 ;i++){
            char* table_card = get_card(state->table[i][0]);
            int col = 0;
            if(table_card != 0 || get_card(s_card)[0] == 'K'){
                while(strncmp(table_card,"0",1) && col < 21){
                    col++;
                    table_card = get_card(state->table[i][col]);
                }
              
            }
            if(check_card_and_table(get_card(state->table[i][col-1]),get_card(s_card))){
                cache_n++;
               
                new_state->table[i][col] = s_card;
                new_state->stock[state->stock_up_index] = 0;
                reset_stock_index(new_state);
                compress_stock(new_state);

                add_c_to_move_set(new_state,'w');
                add_c_to_move_set(new_state, i+'0');

                *depth = *depth + 1;
                //print_std(stdout,4,state,r);
                int n = DFSUtil(new_state,r,move_num+1,max_number,depth,can_referesh_stock,v,c);
                free(new_state);
                new_state = NULL;
                break;
            }
        }
    }
    //printf("return DFS_stock\n");
    return 0;
}
// Technically, a solitaire game is won when all cards are in the foundations. However, a game is guaranteed to
// be winnable if there are no covered cards, no stock cards, and at most one waste card. This should be the
// condition that you search for.
int check_game(game_state *state){
    // printf("%d %d\n",state->stock_up_index,state->stock[0]);
    // printf("%d %d\n",state->stock[0],state->stock[1]);
    if(state->stock_up_index == -1 && state->stock[0] != 0){
        return 0;
    }else if(state->stock[0] != 0 && state->stock[1] != 0){
        return 0;
    }else {
        for(int i = 0; i< 7; i++ ){
          
            if(state->table_facedown_index[i] != -1){
            return 0;
            }
        }
    } 
    //printf("\n");
    return 1;
}

int check_tc_to_tc(char *t1, char *t2){
    if(check_opposite_suits(t1[1],t2[1]) && rank_Hierarchy(t1[0])+1 == rank_Hierarchy(t2[0])){
        return 1;
    }
    return 0;
}

void move_t_to_t(game_state *state, int src_row, int src_col, int des_row, int des_col){
    int card = state->table[src_row][src_col];
    if(state->table_facedown_index[src_row] != -1 && state->table_facedown_index[src_row]+1 == src_row){
        state->table_facedown_index[src_row]--;        
    }
    int i = 0;
    while (card != 0 && i < 21){
       // printf("%d\n",i);
   
        state->table[des_row][des_col+i] = card;
        state->table[src_row][src_col+i] = 0;
        i++;
        card = state->table[src_row][src_col+i];
    }
        //print_std(stdout,4,state,r);
}
int compress_stock(game_state *st){

    int faceup_card = st->stock[st->stock_up_index];

    for(int i = 1 ; i < 52; i++){
        for(int j = 0 ; j <= i; j++){
            if(st->stock[j] == 0){
                st->stock[j] = st->stock[i];
                st->stock[i] = 0;
            }
        }
    }
    if(st->stock_up_index != -1 && st->stock_up_index != 53){
        for(int i = 0; i < 52; i++){
            if(faceup_card == st->stock[i]){
                st->stock_up_index = i;
                return 1;
            }
        }
    }else{
        return 1;
    }
    return 0;
}
//checked
game_state* copy_game_state(game_state *st){
    game_state *new_state = malloc(sizeof(game_state));
    // //foundations
    memcpy(new_state->foundations,st->foundations, 4*sizeof(int));
    //stock
    memcpy(new_state->stock,st->stock, 52*sizeof(int));
    new_state->stock_up_index = st->stock_up_index;
    //table
     for(int i = 0 ; i< 7; i++){
        memcpy(new_state->table[i],st->table[i], 21*sizeof(int));
    }
    //table
    memcpy(new_state->table_facedown_index,st->table_facedown_index, 7*sizeof(int));

    memcpy(new_state->move_Set,st->move_Set,2000*sizeof(char));

    return new_state;
}
int check_flip_stock(game_state *state){
    if(state->stock[state->stock_up_index] == 0){
        return 1;
    }
    return 0;
}
int check_flip_card(game_state *state){
    if(state->stock_up_index != 52 && state->stock[0] != 0 && state->stock[state->stock_up_index] != 0){
        return 1;
    }
    return 0;
}
int check_stock_to_foundations(game_state *state){

        if(state->stock_up_index == -1 || state->stock[state->stock_up_index] == 0 ){   
            //printf("a\n");
            return 0;
        }else if(state->stock[state->stock_up_index] != 0){
            if(state->foundations[suit_Hierarchy(get_card(state->stock[state->stock_up_index])[1])] == 0 && get_card(state->stock[state->stock_up_index])[0] == 'A'){
                return 1;
            }else if(state->foundations[suit_Hierarchy(get_card(state->stock[state->stock_up_index])[1])]+ suit_Hierarchy(get_card(state->stock[state->stock_up_index])[1])*13 
            == state->stock[state->stock_up_index]-1 ){
                return 1; 
            }
        }

    return 0;
}
int check_stock_to_table(game_state *state){

    char stock_rank = get_card(state->stock[state->stock_up_index])[0] ;
    char stock_suit = get_card(state->stock[state->stock_up_index])[1] ;
    
    for(int i = 0; i < 7 ; i++){
        char* table_card = get_card(state->table[i][0]);
       
        int col = 0;
        if(table_card != 0){
            while(strncmp(table_card,"0",1) && col < 21){
                col++;
                table_card = get_card(state->table[i][col]);
            }
            table_card = get_card(state->table[i][col-1]);

            //printf("%s ",table_card);
            if(check_opposite_suits(table_card[1], stock_suit)
            && rank_Hierarchy(stock_rank)+1 == rank_Hierarchy(table_card[0])){
                return 1;
            }
        }
    }
    //printf("\n");
    return 0;
}
int check_card_and_table(char *c1, char *c2){

   if(check_opposite_suits(c1[1], c2[1])
        && rank_Hierarchy(c1[0])-1 == rank_Hierarchy(c2[0])){
        return 1;
    }
    return 0;
}
void reset_stock_index(game_state *state){
    if(state->stock[0] == 0 || state->stock_up_index == -1){
        state->stock_up_index = -1;
    }else{
        state->stock_up_index = state->stock_up_index -1;
    }
}
void reset_table_index(game_state *state, int row){
    if(state->table[row][0] == 0){
         state->table_facedown_index[row] = -1;

    }else if(state->table[row][0] != 0 &&  state->table_facedown_index[row] != -1){
        char* table_card = get_card(state->table[row][0]);
        int col = 0;
        if(table_card != 0){
            while(strncmp(table_card,"0",1) && col < 21){
                col++;
                table_card = get_card(state->table[row][col]);
            }
            table_card = get_card(state->table[row][col-1]);
            if(state->table_facedown_index[row] > col-1 ){
                // must flip a card over
                state->table_facedown_index[row] = state->table_facedown_index[row] -1;
            }    

        }
    }
}
int table_card_to_foundation_card(char* t1, char *f2){
    if(t1[1] == f2[1] && rank_Hierarchy(t1[0])+1 == rank_Hierarchy(f2[0])){
        return 1;
    }
    return 0;
}
int check_table_to_table(game_state *state){
        char stock_rank = get_card(state->stock[state->stock_up_index])[0] ;
    char stock_suit = get_card(state->stock[state->stock_up_index])[1] ;
    
    for(int i = 0; i < 7 ; i++){
        char* table_card = get_card(state->table[i][0]);
        int col = 0;
        while(col < 21 && table_card != 0){
            
            col++;
            table_card = get_card(state->table[i][col]);
        }
    }
    return 1;
}
int check_opposite_suits(char s1, char s2){
    if((s1 == 'c'|| s1 =='s') && (s2 == 'd' || s2 == 'h' )){
        return 1;
    }else if((s2 == 'c'|| s2 =='s') && (s1 == 'd' || s1 == 'h' )){
        return 1;
    }
    return 0;
}
int check_hash_table(long key, int x){
    for(int i = 0 ; i < 1000000 &&  i < hashset_index && hashset[hashset_index][0] != 0; i++){
        if(key == hashset[i][0] && hashset[i][1] != x){
            return 1;
        }
    }
    return 0;
}
int hash_table(game_state *state){
    unsigned long hash = 5381;
    // for(int i = 0; i < 7; i++){
    //     int c = state->table[i][0]; 
    //     int col = 0;
    //     int plus = 0;
    //     while(c != 0 && col < 22){
    //         hash = ((hash << 5) + hash) + c*col*i* 33; //* hash * 33 + c;
    //         col++;
    //         plus++;
    //         c = state->table[i][col]; 
    //     }
    // }
    // for(int i = 0; i < 7; i++){
    //     int c = state->table[i][0]; 
    
    //     int col = 1;
    //     while(c != 0){
    //          c = state->table[i][col]; 
    //         col++;
    //     }
    //     c = state->table[i][col-1]; 
    //      hash = ((hash << 5) + hash) + c*col*i* 33;
 
    // }
    for(int i = 0; i < 7; i++){
        int c = state->table[i][0]; 
    
        int col = 0;
        while(c != 0){
             hash = ((hash << 5) + hash) + c*col*i;
            
            col++;
            c = state->table[i][col]; 
        }
    }
    unsigned long zeros =1;
    //unsigned long hash = 5381;
    for(int i = 0; i < 7; i++){
        int c = state->table[i][0]; 
         int col = 0;
        unsigned long hash_plus = 0;
        while(c != 0 && col < 22){
            hash_plus += ((hash_plus << 5) + hash) + ((c-1)%13+1)*i*col;
            col++;
            c = state->table[i][col]; 
        }
        hash += hash_plus*zeros;
        zeros = zeros*100;
    }
   // printf("%ld\n", hash);

    return hash;
}