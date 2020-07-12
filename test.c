#include <stdio.h>
#include <stdlib.h>
int main(){
	char A[]=" S ffff,3";
	unsigned long long address;
	char type;
	int sizeData;
	sscanf(A," %c %llx,%d",&type,&address,&sizeData);
	printf("type=%c, address=%lld, sizeData=%d\n",type,address,sizeData);

	return 0;
}
