// ==> Pouring termbox-1.1.2.catalina.bottle.tar.gz
// 🍺  /usr/local/Cellar/termbox/1.1.2: 9 files, 93.3KB
// ben@Bens-Air-4 project 4 % 

// #include <termbox.h>
// #include <iostream>

unsigned long RNG_seed;
int main(){
    //idea main 
    tb_init();

    struct tb_cell cell;
    
}


double Random()
{
    const unsigned long MODULUS = 2147483647;
    const unsigned long MULTIPLIER = 48271;
    const unsigned long Q = MODULUS / MULTIPLIER;
    const unsigned long R = MODULUS % MULTIPLIER;
    unsigned long t1 = MULTIPLIER * (RNG_seed % Q);
    unsigned long t2 = R * (RNG_seed / Q);
    if (t1 > t2) {
        RNG_seed = t1 - t2;
    } else {
        RNG_seed = t1 + (MODULUS - t2);
    }
    return ((double) RNG_seed / MODULUS);
}

long choose_randomly_between(long a, long b)
{
return a + (long) ( (b-a+1) * Random() );
}
void shuffle(int* deck)
{
    int i;
    for (i=0; i<52; i++) deck[i] = i+1;
        for (i=0; i<51; i++) {
            int j = choose_randomly_between(i, 51);
            if (i != j) {
            //Swap(deck[i], deck[j]);
        }
    }
}