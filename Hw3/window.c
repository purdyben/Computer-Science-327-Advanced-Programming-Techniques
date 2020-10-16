#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void initialize();
void fill_line();

struct windowSize{
	unsigned int dxlow;
	unsigned int dylow; 
	unsigned int dxhigh;
	unsigned int dyhigh;
};

struct window{
unsigned int xlow; 
unsigned int ylow; 
unsigned int xhigh; 
unsigned int yhigh;
char wch;
};


void fill_windows(struct window W[], unsigned wmax);
void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]);


//struct window (*W)[];
unsigned int numw;
unsigned int wmax;


int main() {
	struct windowSize wSize;
	printf("set up window size:  dxlow,dylow dxhigh,dyhigh \n");
	scanf("%d,%d", &wSize.dxlow, &wSize.dylow);
	scanf("%d,%d", &wSize.dxhigh, &wSize.dyhigh);
	
	scanf("%d", &wmax); 

	struct window W[wmax];	
	fill_windows(W, wmax);
	int i;
	for(int i = 0; i < wmax; ++i){
		printf("xlow: %d, ylow: %d, xhigh: %d, yhigh: %d , char, %c\n", W[i].xlow, W[i].ylow, W[i].xhigh, W[i].yhigh, W[i].wch  );
	}
	int y;
	for( y = 0 ; y <= wSize.dyhigh; y++){
		char line[wSize.dxhigh+1];
		int i;
		for(i = 0;i < wSize.dxhigh+1 ;++i)
			// code readibilty ++
			i[line] = 32; 

		fill_line(W, wmax, wSize.dxlow, wSize.dxhigh, y, line);
	}


	return 1;
}

unsigned fill_windows(struct window W[], unsigned wmax){
	
	
	for(int i = 0; i < wmax; i++){ // pops the array;
		int temp;
		int temp2;
		char tt; 
		scanf("%d,%d", &temp,&temp2); 
		W[i].xlow = temp;
		W[i].ylow = temp2;

		scanf("%d,%d", &temp,&temp2); 
		W[i].xhigh = temp;
		W[i].yhigh = temp2;


		char c; 
		//printf("char \n");
		scanf(" %c", &c); 
		W[i].wch = c;
		//printf("---------------------------------\n");
	}
}

void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]){
	//printf("Wsize %d, x1: %d, x2: %d, y: %d \n", Wsize,x1,x2,y);
	
	for(int i = 0 ; i < Wsize ; ++i){
		for(int j = 0 ; j <= x2; ++j){
			if(j >= W[i].xlow && j<= W[i].xhigh){
				if(y >= W[i].ylow && y <= W[i].yhigh ){
					line[j] = W[i].wch;
				}	
			}
		}
		
	}
	//display 
	printf("%s\n", line);
}

// 0 0 10 10
//  
// 0 0 3 3 A
// 2 2 5 5 B
// 7 7 15 9 C
// 0 7 5 11 D
// 9 0 12 4 E
// 0,0 10,10
// 5
// 0,0 3,3 A
// 2,2 5,5 B
// 7,7 15,9 C
// 0,7 5,11 D
// 9,0 12,4 E