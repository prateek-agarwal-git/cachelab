#include <stdio.h>
#include <stdlib.h>
int main(){
	char A[]="ffff";
	int t;
	sscanf(A,"%x",&t);
	printf("%d\n",t);
	return 0;
}
