#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void signalHandler(int);

int main(){

    int x=1;


        while(x!=4)
	{
                signal(SIGINT,&signalHandler);
                printf(" 4 zamyka program, chyba\n ");
		scanf("%d",&x);
        }

        return 0;
}

 void signalHandler(int s){

    printf("nie dziala %d\n",s);
 }



