#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h> 
#include </usr/local/Cellar/termbox/1.1.2/include/termbox.h>
#include "termbox.h"
//gcc -I/usr/local/Cellar/termbox/1.1.2/include game.c /usr/local/Cellar/termbox/1.1.2/lib/libtermbox.a
//	gcc -I/share/cs327/include game.c /share/cs327/lib/libtermbox.a -o game

char* card[] = {"0","Ac", "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ad", "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd",
"Ah", "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh","As", "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks"};

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

void create_card(int position_x, int position_y,char rank,char suit,int back);
void set_suit(char c,struct tb_cell *cell);
void show_table(game_state *state);
void show_game(game_state *game);
void show_foundations(game_state *state, int position_x, int position_y);
void show_stock(game_state *state, int position_x, int position_y);
void show_rules(game_state *state, int position_x, int position_y);

void make_move_table_to_foundations(game_state *state,char src, char dest);
void make_move_table_to_table(game_state *state,char src, char dest);

void move_t_to_t(game_state *state, int src_row, int src_col, int des_row, int des_col);
game_state* copy_game_state(game_state *st);
int compress_stock(game_state *st);
int check_flip_stock(game_state *state);
int check_flip_card(game_state *state);
int check_stock_to_foundations(game_state *state);
int check_stock_to_table(game_state *state);
int check_card_and_table(char *c1, char *c2);
void reset_stock_index(game_state *state);
void reset_table_index(game_state *state, int row);
int table_card_to_foundation_card(char* t1, char *f2);
int check_table_to_table(game_state *state);
int check_opposite_suits(char s1, char s2);
void add_c_to_move_set(game_state *state, char move);
int select_card_t_t(game_state *state, int src_col, int dest_col);
void show_selector(int x, int y);
void show_dest(game_state *state, int dest_col);
void move_t_to_t(game_state *state, int src_row, int src_col, int des_row, int des_col);
void randomize (int arr[], int n);
void swap(int*, int* );
game_state* random_game();

int main (int argc, char *argv[]){
    int N = 1000;
    int ar = 1;
    
    int  diagnostic = 0, c = 0, exchange_formate = 0 , 
    random_game_rule = 0, seed = 0, limit = 0;

    FILE *file_out = stdout;
    FILE* infile  = NULL ;//fopen(argv[1], "r");
    while(ar < argc ){
        if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'm'){
            N =  atoi(argv[ar+1]);
            ar++;
        }else if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'o'){
           file_out = fopen(argv[ar+2], "w");// file to out put
            ar+=2;
        }else if (*(argv[ar]) == '-' && *(argv[ar]+1) == 'x'){
            exchange_formate = 1; // alternative formate to the file
            
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'c'){
            c = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'f'){
           // file argument 
           printf("found file\n");
            infile = fopen(argv[ar+1], "r");
            ar++;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'v'){
            diagnostic = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == '1'){
            random_game_rule = 1;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == '3'){
            random_game_rule = 3;
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 's'){
            seed = 1;//*(argv[ar]+2);
        }else if(*(argv[ar]) == '-' && *(argv[ar]+1) == 'l'){
            limit = *(argv[ar+1])-'0';
            printf("limit %d\n",limit);
            printf("%c\nstock", *(argv[ar+1]));
            ar++;
        }else {
            //test 
          //infile = fopen(argv[ar], "r");
        };
         ar++;
    }
//      -s seed: Initialize the game by randomly shuffling the deck and dealing out cards, using the specified
// seed (a positive integer) to initialize the random number generator. Using the same seed should cause
// the same game to be dealt.
// • -1: Specify turn 1 if a random game is generated.
// • -3: Specify turn 3 if a random game is generated.
// • -l L: Specify limit L if a random game is generated (default is “unlimited”).
    if(seed != 1 && infile == NULL ){
        infile = stdin;
    }else{
        rules *rules = malloc(sizeof(rules));
        game_state *state = malloc(sizeof(game_state));
        if(seed == 0 || infile != NULL){
            printf("parse file\n");
            parse_file(infile,rules, state);
        }else{
            state = random_game();// create reandom seed and make game 
        }
        game_state *pre_move = copy_game_state(state);
        game_state *first_state = copy_game_state(state);
        struct tb_cell de;
        struct tb_cell sr;
        de.fg = TB_BLACK;de.bg = TB_WHITE;sr.fg = TB_BLACK;sr.bg = TB_WHITE;

        char src = 0; 
        char dest = 0;
        int resets_used = 0;
        struct tb_event event_action;   
        int x;
        int y;

        for(;;){  
            //show
            tb_init();
            show_game(state);
            tb_put_cell(0,33,&sr);
            tb_put_cell(2,33,&de);

            tb_present();
            tb_poll_event(&event_action);
            // event

            if(event_action.ch == 'q' || event_action.ch == 'Q'){
                break;
            }else if(event_action.ch == 'n' || event_action.ch == 'N'){
                // generate new game
                free(state);
                state = random_game();
		resets_used = 0;
            }else if(event_action.ch == 'r'|| event_action.ch == 'R'){
                // load old save
                free(state);
                state = copy_game_state(first_state);
            }else if(event_action.ch == 's'|| event_action.ch == 'S'){
                //save game
            }else if(event_action.ch == 'u'|| event_action.ch == 'U'){
                game_state *temp = state;
                state = pre_move;
                pre_move = temp;
            }else if(event_action.ch == 'w'|| event_action.ch == 'W'){
                if(src == 0){
                    src = 'w';
                }else{
                    dest = 'w';
                }
            }else if(event_action.ch == '.'){
                src = '.';
                dest = 0;
            }else if(event_action.ch == ','){
                src = ',';
                dest = 0;
            }else if(event_action.ch == 'f'|| event_action.ch == 'F'){
                if(src == 0){
                    src = 'f';
                }else{
                    dest = 'f';
                }
            }else if(event_action.ch == '1'){
                if(src == 0){
                    src = '1';
                }else{
                    dest = '1';
                }
            }else if(event_action.ch == '2'){
                if(src == 0){
                    src = '2';
                }else{
                    dest = '2';
                }
            }else if(event_action.ch == '3'){
                if(src == 0){
                    src = '3';
                }else{
                    dest = '3';
                }
            }else if(event_action.ch == '4'){
                if(src == 0){
                    src = '4';
                }else{
                    dest = '4';
                }
            }else if(event_action.ch == '5'){
                if(src == 0){
                    src = '5';
                }else{
                    dest = '5';
                }
            }else if(event_action.ch == '6'){
                if(src == 0){
                    src = '6';
                }else{
                    dest = '6';;
                }
            }else if(event_action.ch == '7'){
                if(src == 0){
                    src = '7';
                }else{
                    dest = '7';
                }
            }
            sr.ch = src; 
            de.ch = dest;
            //action
            if(src == '.'){
            //check and make move 

                if(check_flip_card(state)){
                   
                    free(pre_move);
                    pre_move = copy_game_state(state);
                    
                    state->stock_up_index = state->stock_up_index+1;
                    add_c_to_move_set(state,'.');     
                }
                src = 0;
                dest = 0;
            }else if(src == ','){
                
                //check and make move 
                if(check_flip_stock(state) && (resets_used < limit || limit == 0)){
                    resets_used++;
                    free(pre_move);
                    pre_move = copy_game_state(state);

                    state->stock_up_index = 0;
                }
                add_c_to_move_set(state,'r');     

                src = 0;
                dest = 0;
            }else if(dest != 0){     
                //check and make move 
                if(src == 'w' && dest == 'f'){
                    free(pre_move);
                    pre_move = copy_game_state(state);
                    //wast -> f   
                    if(check_stock_to_foundations(state)){
                       // if(state->stock_up_index != -1){
                        // // move card to foundation
                        state->foundations[suit_Hierarchy(get_card(state->stock[state->stock_up_index])[1])] 
                            = state->stock[state->stock_up_index] - suit_Hierarchy(get_card(state->stock[state->stock_up_index])[1])*13;
                        // // remove from stock;
                         state->stock[state->stock_up_index] = 0; 
                         reset_stock_index(state);
                        // // compress sock
                         compress_stock(state);

                        add_c_to_move_set(state,'w');
                        add_c_to_move_set(state,'f');
                    }  
                    src = 0;
                    dest = 0;    
                }else if(src == 'w' && isdigit(dest)){

                    int card;
                    int i = 0;
                    while(state->table[(dest-'0')-1][i] != 0){
                        i++;
                    }
                    i--;
                    card = state->table[(dest-'0')-1][i];

                    //wast -> table 
                    if(state->table[(dest-'0')-1][0] != 0){ // there is a card 
                        //check table with cards
                        if(check_card_and_table(get_card(card),get_card(state->stock[state->stock_up_index]))){
                             state->table[(dest-'0')-1][i+1] =  state->stock[state->stock_up_index];

                            state->stock[state->stock_up_index] = 0; 
                            reset_stock_index(state);
                            // // compress sock
                            compress_stock(state);

                            add_c_to_move_set(state,'w');
                            add_c_to_move_set(state,dest);
                        }
                       
                    }else{
                        //check table without cards
                        if(state->table[(dest-'0')-1][0] == 0 && 
                            get_card(state->stock[state->stock_up_index])[0] == 'K'){

                        state->table[(dest-'0')-1][i+1] =  state->stock[state->stock_up_index];

                        state->stock[state->stock_up_index] = 0; 
                         reset_stock_index(state);
                        // // compress sock
                         compress_stock(state);

                        add_c_to_move_set(state,'w');
                        add_c_to_move_set(state,dest);
                        }

                    }
                           
                               
                }else if(isdigit(src) && isdigit(dest) && src != dest){// table -> table c
                    int src_row = select_card_t_t(state, src-'0'-1,dest-'0'-1);
                    int dest_card;
                    int i = 0;
                    while(state->table[(dest-'0')-1][i] != 0){
                        i++;
                    }
                    i--;
                    dest_card = state->table[(dest-'0')-1][i];
                    if(check_card_and_table(get_card(dest_card),get_card(state->table[src-'0'-1][src_row]))){
                        free(pre_move);
                        pre_move = copy_game_state(state);
                        move_t_to_t(state,src_row, (src-'0')-1, i+1, (dest-'0')-1);
                        x=src_row;
                        y=i;
                        
                        add_c_to_move_set(state, src);
                        add_c_to_move_set(state,dest);
                    }else if(get_card(state->table[(src-'0')-1][src_row])[0] == 'K' && state->table[i+1][(dest-'0')-1] == 0){
                        free(pre_move);
                        pre_move = copy_game_state(state);
                        move_t_to_t(state,src_row, (src-'0')-1, i+1, (dest-'0')-1);
                        
                        add_c_to_move_set(state, src);
                        add_c_to_move_set(state,dest);
                    }
                        
            
                }else if(isdigit(src) && dest == 'f'){
                        if(state->table[(src-'0')-1][0] != 0){ // there is a card 
                            //get table card
                            int card;
                            int i = 0;
                            while(state->table[(src-'0')-1][i] != 0){
                                i++;
                            }
                            i--;
                            card = state->table[(src-'0')-1][i];

                            int dest_card = state->foundations[suit_Hierarchy(get_card(card)[1])] +suit_Hierarchy(get_card(card)[1]) * 13;
                            x = card;
                            y =dest_card;
                            if(table_card_to_foundation_card(get_card(card),get_card(dest_card))){
                            // //    if(1){
                                free(pre_move);
                                pre_move = copy_game_state(state);
                                // make_move_table_to_foundations(state,src, dest);
                                if(state->table_facedown_index[(src-'0')-1] != -1 && state->table_facedown_index[(src-'0')-1] == i-1){
                                    state->table_facedown_index[(src-'0')-1]--;
                                }
                                state->foundations[suit_Hierarchy(get_card(state->table[(src-'0')-1][i])[1])]++;
                                state->table[(src-'0')-1][i] = 0;


                                add_c_to_move_set(state,src);     
                                add_c_to_move_set(state,dest);     
                                
                            }
                        }
                    }
                    src = 0;
                    dest = 0;
            }
            if(src == 'f' || dest == 'w'){
                src = 0;
                dest = 0; 
            }
            //reset background
        }
        
        //struct tb_event event;
        //tb_poll_event(&event);
        tb_shutdown();
        printf("%d,%d,%s, %s\n",x,y,get_card(x),get_card(y));
        printf("%d,\n",suit_Hierarchy(get_card(x)[1]));
        printf("%d\n", table_card_to_foundation_card(get_card(x),get_card(y)));
        printf("return 0");
    }
    return 1;
}

// random game 
game_state* random_game(){
    // rules *rules = malloc(sizeof(rules));
    game_state *state = calloc(6,sizeof(game_state));
    //state->stock_up_index = 24;
    //return state;
    int card[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52};

    randomize(card,52);
        //create table 
    int index = 0;
    for(int i = 0 ;i < 7 ; i++){
        for(int j = 0; j < 7-i;j++){
            
            state->table[i][j] = card[index];
            index++;
        }
         state->table_facedown_index[i] = 6-i-1;
    }
    for(int i = 0; i<24;i++){
        state->stock[i] = card[29+i];
    } 
    compress_stock(state);
    state->stock_up_index = 0;
    return state;
}
void randomize (int arr[], int n)  
{  
    srand ( time(NULL) ); 
    for (int i = n - 1; i > 0; i--)  
    {   
        int j = rand() % (i + 1);  
        swap(&arr[i], &arr[j]);  
    }  
}  
void swap (int *a, int *b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 
//==== currsor 
int select_card_t_t(game_state *state, int src_col, int dest_col){
     tb_shutdown();
    struct tb_event ev;
    int x = src_col;
    int y = state->table_facedown_index[src_col]+1;
    int src_row = 0;
    for(;;){
        tb_init();
        show_game(state);
        show_selector(x,y);
        show_dest(state,dest_col);
        tb_present();
          
        tb_poll_event(&ev);
        if(ev.ch == 'c' || ev.ch == TB_KEY_ENTER){
           break;
        }else if(ev.ch == 'x' || ev.ch == TB_KEY_ARROW_UP){
            // change selector
            if(state->table[src_col][y+1] != 0 && state->table_facedown_index[src_col] == -1){
                y++;    
            }else if(y != 21 && state->table[src_col][y+1] != 0 && y >= 1+state->table_facedown_index[src_col]){
                y++;
            }

        }else if(ev.ch == 's' || ev.ch == TB_KEY_ARROW_DOWN){
            // change selector
            if(y != 0 && state->table_facedown_index[src_col] == -1){
                y--;    
            }else if(y != 0 && y > 1+state->table_facedown_index[src_col]){
                y--;
            }
        }

    }
     tb_shutdown();
    return y ;


}
void show_dest(game_state *state, int dest_col){
    
    struct tb_cell select;
    select.bg = TB_RED;
    select.fg = TB_RED;
    select.ch = '+';
    int k = 0;
    while(state->table[dest_col][k] != 0){
        k++;
    }
    k--;
    int x[] = {2,8,14,20,26,32,38};
    int y[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50};
    for(int i = 0 ; i< 5 ; i++){
        tb_put_cell(x[dest_col]+i-1,y[k],&select);
        tb_put_cell(x[dest_col]+i-1,y[k]+4,&select);
    }
    for(int i = 0 ; i< 5 ; i++){
        tb_put_cell(x[dest_col]-2,y[k]+i,&select);
        tb_put_cell(x[dest_col]+4,y[k]+i,&select);
    }
}
void show_selector(int col, int row){
    struct tb_cell select;
    select.bg = TB_RED;
    select.fg = TB_RED;
    select.ch = '+';
    int x[] = {2,8,14,20,26,32,38};
    int y[] = {2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50};
    // x = ;
    // y = ;
    for(int i = 0 ; i< 4 ; i++){
        tb_put_cell(x[col]-2+i,y[row]-2,&select);
        tb_put_cell(x[col]-2+i,y[row],&select);
    }
     tb_put_cell(x[col]-2,y[row]-1,&select);
      tb_put_cell(x[col]+1,y[row]-1,&select);
   




}

//============ show game 
void show_game(game_state *game){
    //show rules;
    show_rules(game,44,0);
    //show foundations.
    show_foundations(game,44,15);
    //show stock;
    show_stock(game,1,25);
    //show table
    show_table(game);
}
//card are 6 x 8
void create_card(int position_x, int position_y,char rank,char suit, int back){
    struct tb_cell cell_back_ground;
    struct tb_cell cell_highlight;
    struct tb_cell cell_rank;
    struct tb_cell cell_suit;

    cell_rank.bg = TB_WHITE;
    cell_rank.fg =TB_BLACK;
    cell_rank.ch = rank;
    
    cell_suit.bg = TB_WHITE;
  
    // cell_suit.ch = 0x02660;
    set_suit(suit,&cell_suit);
    
    cell_back_ground.bg = TB_WHITE;
     cell_back_ground.fg = TB_WHITE;
      cell_back_ground.ch = '+';
    cell_highlight.bg = TB_CYAN;
    cell_highlight.fg = TB_CYAN;
    cell_highlight.ch = '+';


    if(back == 0){
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+4; j++){
                tb_put_cell(i,j, &cell_back_ground);
            }
        }
        for(int i = position_x ; i < position_x+5; i++){
            tb_put_cell(i,position_y, &cell_highlight);
            tb_put_cell(i,position_y+4, &cell_highlight);
        }
        tb_put_cell(position_x+0,position_y+1,&cell_rank);
        tb_put_cell(position_x+3,position_y+3,&cell_rank);
        tb_put_cell(position_x+1,position_y+1,&cell_suit);
        tb_put_cell(position_x+4,position_y+3,&cell_suit);

    }else{
        cell_highlight.fg = TB_BLACK;
        cell_highlight.ch = '*';
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+5; j++){
                tb_put_cell(i,j, &cell_highlight);
            }
        }   
    }
}
void show_table(game_state *state){
    struct tb_cell cell_table_ground;
    cell_table_ground.fg = TB_GREEN;
    cell_table_ground.bg = TB_GREEN;
    cell_table_ground.ch = '+';
    //background 
    for(int i = 0 ; i < 43; i++){
        for(int j = 0 ; j < 25; j++){
          tb_put_cell(i,j, &cell_table_ground);
        }
    }
    cell_table_ground.fg = TB_BLACK;
    cell_table_ground.ch = '1';
    tb_put_cell(3,24, &cell_table_ground);
    cell_table_ground.ch = '2';
    tb_put_cell(9,24, &cell_table_ground);
    cell_table_ground.ch = '3';
    tb_put_cell(15,24, &cell_table_ground);
        cell_table_ground.ch = '4';
    tb_put_cell(21,24, &cell_table_ground);
        cell_table_ground.ch = '5';
    tb_put_cell(27,24, &cell_table_ground);
        cell_table_ground.ch = '6';
    tb_put_cell(33,24, &cell_table_ground);
        cell_table_ground.ch = '7';
    tb_put_cell(39,24, &cell_table_ground);
    cell_table_ground.fg = TB_GREEN;
    // cards first take the data then insert the card at qordnets 
    //int table[7][21];
    int x[] = {2,8,14,20,26,32,38};
    int y[] = {2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50};
   
    for(int i = 0 ; i < 7; i++){
        for(int j = 0 ; j < 21; j++){
            if(state->table[i][j] == 0){
                break;
            }else{
                if(state->table_facedown_index[i] >= j){
                    create_card(x[i]-1,y[j]-2,get_card(state->table[i][j])[0],get_card(state->table[i][j])[1],1);

                }else{
                    create_card(x[i]-1,y[j]-2,get_card(state->table[i][j])[0],get_card(state->table[i][j])[1],0);
                }
            }
           // printf("%c%c, ",get_card(state->table[i][j])[0],get_card(state->table[i][j])[1]);
        }
        //printf("\n");
    }
  

}
void show_foundations(game_state *state, int position_x, int position_y){
    struct tb_cell cell_foundation_ground;
    cell_foundation_ground.fg = TB_BLACK;
    cell_foundation_ground.bg = TB_WHITE;
    struct tb_cell ch;
    ch.fg = TB_WHITE;
    ch.bg = TB_DEFAULT;

    char *cc = "Foundations";
    char c = cc[0];
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i-1,position_y, &ch);
          c = cc[i];
    }
    position_y+=2;
    if(state->foundations[0] == 0){
        set_suit('c',&cell_foundation_ground);
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+5; j++){
                tb_put_cell(i,j, &cell_foundation_ground);
            }
        }
    }else{
        create_card(position_x,position_y,get_card(state->foundations[0] + 0*13)[0],'c',0);

    }
    if(state->foundations[1] == 0){
        set_suit('d',&cell_foundation_ground);
        position_x+=6;
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+5; j++){
                tb_put_cell(i,j, &cell_foundation_ground);
            }
        }
    }else{
        position_x+=6;
        create_card(position_x,position_y,get_card(state->foundations[1] + 1*13)[0],'d',0);
    }
    if(state->foundations[2] == 0){
        position_x+=6;
        set_suit('h',&cell_foundation_ground);
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+5; j++){
                tb_put_cell(i,j, &cell_foundation_ground);
            }
        }
    }else{
        position_x+=6;
        create_card(position_x,position_y,get_card(state->foundations[2] + 2*13)[0],'h',0);
    }
    if(state->foundations[3] == 0){
        set_suit('s',&cell_foundation_ground);
        position_x+=6;
        for(int i = position_x ; i < position_x+5; i++){
            for(int j = position_y ; j < position_y+5; j++){
                tb_put_cell(i,j, &cell_foundation_ground);
            }
        }
    }else{
        position_x+=6;
        create_card(position_x,position_y,get_card(state->foundations[3] + 3*13)[0],'s',0);
    }
}
void show_stock(game_state *state, int position_x, int position_y){
    struct tb_cell ch;
    struct tb_cell cell_stock_ground;
    struct tb_cell cell_stock_ground2;
    cell_stock_ground.fg = TB_CYAN;
    cell_stock_ground.bg = TB_CYAN;
    cell_stock_ground.ch = '+';

    cell_stock_ground2.fg = TB_GREEN;
    cell_stock_ground2.bg = TB_GREEN;
    cell_stock_ground2.ch = '+';

    ch.fg = TB_WHITE;
    ch.bg = TB_DEFAULT;

    // for(int i = position_x ; i < position_x+40; i++){
    //     for(int j = 40 ; j < 50; j++){
    //       tb_put_cell(i,j, &cell_stock_ground2);
    //     }
    // }
    // number of stock
    char *cc = "Stock";
    char c = cc[0];
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i-1,position_y, &ch);
          c = cc[i];
    }
    // out of stock
    if(state->stock[state->stock_up_index+1] == 0){
        for(int i = 0; i < 5;i++){
            for(int j = 0; j<5; j++){
                tb_put_cell(position_x+i,position_y+j+2, &cell_stock_ground2);
            }
        }
    }else{
        create_card(position_x,position_y+2,1,'c',1);
    }
    // number in waste
    cc = "Waste";
    c = cc[0];
    for(int i = 1; c != 0; i++){
        ch.ch = c;
           tb_put_cell(position_x+9+i,position_y, &ch);
          c = cc[i];
    }

    // show all waist cards

     for(int i = 0; i < 51 && i <= state->stock_up_index && state->stock[i] != 0;i++){
        create_card(position_x + 10 +i*2,position_y+2,get_card(state->stock[i])[0],get_card(state->stock[i])[1],0);
     }


}
void show_rules(game_state *state, int position_x, int position_y){
    struct tb_cell cell_rules_ground;
    struct tb_cell cell_rules_ground_black;
    cell_rules_ground.fg = TB_GREEN;
    cell_rules_ground.bg = TB_GREEN;
    cell_rules_ground.ch = '+';
    cell_rules_ground_black.fg = TB_BLACK;
    cell_rules_ground_black.bg = TB_BLACK;
    cell_rules_ground_black.ch = '+';

    struct tb_cell ch;
    ch.fg = TB_WHITE;
    ch.bg = TB_BLACK;
    //background
    for(int i = position_x ; i < position_x+45; i++){
        for(int j = position_y ; j < position_y+15; j++){
            tb_put_cell(i,j, &cell_rules_ground);
        }
    } 
    for(int i = position_x+1 ; i < position_x+45-1; i++){
        for(int j = position_y+1 ; j < position_y+15-1; j++){
            tb_put_cell(i,j, &cell_rules_ground_black);
        }
    } 
    //words
    char *cc = "Rules";
    char c = cc[0];
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "Q : Quite";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "N : New game";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "R : Restart";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "S : Save game";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "F : Foundations";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = ". : Flip stock card";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = ", : Reset stock";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "Table numbers : 1,2,3,4,5,6,7";
    c = cc[0];
    // position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(58+i,1, &ch);
          c = cc[i];
    }
    cc = "Table to table selections screen";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
     cc = "s : cursor up";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
     cc = "x : cursor down";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }
    cc = "c : select card";
    c = cc[0];
    position_y+=1;
    for(int i = 1; c != 0; i++){
        ch.ch = c;
          tb_put_cell(position_x+i+1,position_y+1, &ch);
          c = cc[i];
    }

    
    // 
}
void set_suit(char c,struct tb_cell *cell){
    switch (c)
    {
    case 's':
        cell->fg =TB_BLACK;
        cell->ch = 0x2660;
        break;
    case 'c':
        cell->fg =TB_BLACK;
        cell->ch = 0x2663;
        break;
    case 'h':
        cell->fg = TB_RED;
        cell->ch = 0x2665;
        break;
    case 'd':
        cell->fg = TB_RED;
        cell->ch =0x2666;
        break;
    
    default:
        break;
    }
}
//============ make moves 
void make_move_table_to_foundations(game_state *state,char src, char dest){
   int i = 0;
    while(i != 21 && state->table[src-'0'][i] != 0){
        i++;
    }
    i--;

    if(state->table_facedown_index[i] != -1 && state->table_facedown_index[(src-'0')-1] == i-1){
        state->table_facedown_index[i]--;
    }
    state->foundations[suit_Hierarchy(get_card(state->table[(src-'0')-1][i])[1])]++;

    state->table[(src-'0')-1][i] = 0;

}
void move_t_to_t(game_state *state, int src_row, int src_col, int des_row, int des_col){
    int card = state->table[src_col][src_row];
   
    if(state->table_facedown_index[src_col] != -1 && state->table_facedown_index[src_col]+1 == src_row){
        state->table_facedown_index[src_col]--;        
    }else if( src_col == 0){
         state->table_facedown_index[src_col] = -1;  
    }
    //state->table[src_col][src_row] = 0;
    int i = 0;
    while (card != 0 && i < 21){
  
        state->table[des_col][des_row+i] = card;
        state->table[src_col][src_row+i] = 0;
        i++;
        card = state->table[src_col][src_row+i];
    }
}
//============ 
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
//+++++======================================================= project 4 methods movements only
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
    if(state->stock[state->stock_up_index+1] == 0){
        return 1;
    }
    return 0;
}
int check_flip_card(game_state *state){
    if(state->stock_up_index != 51 && state->stock[0] != 0 && state->stock[state->stock_up_index] != 0){
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
    }else if(t1[0] == 'A'){
         return 1;
    }
    return 0;
}

int check_opposite_suits(char s1, char s2){
    if((s1 == 'c'|| s1 =='s') && (s2 == 'd' || s2 == 'h' )){
        return 1;
    }else if((s2 == 'c'|| s2 =='s') && (s1 == 'd' || s1 == 'h' )){
        return 1;
    }
    return 0;
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
void save_game(game_state *state){

}
//
