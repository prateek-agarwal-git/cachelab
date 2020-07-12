#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
int main(int argc, char ** argv)
{
	int hitCount=0, missCount=0,evictionCount=0;
	int associativity = 1;
	int blockBits = 0;
	int numSets = 2;
	int setBits = 1;
	int verbose = 0;
	char options_string[]= "E:s:b:t:h::v::";
	char * traceFile;
	unsigned long long ** LRUCache;
	int opt;
	FILE * fp;
	while ((opt = getopt(argc, argv, options_string)) != -1) {
		switch(opt) {
			case 'E':
				associativity = atoi(optarg);
				break;
			case 's':
				setBits = atoi(optarg);
				break;
			case 'b':
				blockBits = atoi(optarg);		
				break;
			case 't':
				traceFile = strdup(optarg);
				break;
			case 'v':
				verbose = 1;
				break;
			}	
	}
	numSets = verbose;
	numSets = 1 << setBits;
	LRUCache = (unsigned long long **)malloc(numSets* sizeof(unsigned long long *));
	if (LRUCache == NULL){
		printf("Insufficient memory\n");
		exit(1);
	} 
	for (int i = 0; i < numSets; i++){
		LRUCache[i] = (unsigned long long *) malloc(associativity* sizeof(unsigned long long)); 	
		if (LRUCache[i] == NULL){
			printf("Insufficient memory\n");
			exit(1);
		}
		for (int j = 0 ; j < associativity; j++){
			LRUCache[i][j] =(unsigned long long) -1;			
		}
	}
	fp = fopen(traceFile,"r" );
	int charRead;
	char * line = (char *) malloc(64 * sizeof(char));
	if (line == NULL){ 
		printf("insufficient memory\n");
		exit(1);
	}
	size_t n = 64;
	while ((charRead=getline(&line, &n, fp))!= -1){
		if (line[0] == 'I')
			continue;
		char type;
		unsigned long long address;
		int sizeData;
		sscanf(line," %c %llx,%d",&type,&address,&sizeData);
		if (type ==	'M'){
			hitCount+=1;
		}
		address = address >> blockBits;
		int setIndex = address &(numSets-1);
		address = address >> setBits;		
		int currIndex = 0;
		int emptySlot = 0;
		int emptySlotIndex = 0;
		int hitSuccess = 0;
		while (currIndex < associativity){
			unsigned long long storedAddress = LRUCache[setIndex][currIndex];
			if (storedAddress == (unsigned long long) -1){
				emptySlot = 1;
				emptySlotIndex = currIndex;
				break;
			}
			else if (storedAddress == address){
				int ind = associativity-1;
				while ((ind >0) && (LRUCache[setIndex][ind] == (unsigned long long) -1)){
						ind-=1;
				}
				int hitIndex = currIndex;
				while (hitIndex < ind){
					LRUCache[setIndex][hitIndex] =	LRUCache[setIndex][hitIndex+1];
					hitIndex+=1;
				}
				LRUCache[setIndex][ind] = storedAddress;
				hitSuccess = 1;
				hitCount+=1;
				break;
			}
			currIndex+=1;
		}

	if ((emptySlot == 1) && (hitSuccess == 0)){
		missCount+=1;
		LRUCache[setIndex][emptySlotIndex] = address;
	} 
	else if ((emptySlot == 0) && (hitSuccess == 0)){
		evictionCount+=1;
		missCount+=1;
		int ind = 0;
		while (ind < associativity -1){
			LRUCache[setIndex][ind]	= LRUCache[setIndex][ind+1];
			ind+=1;
		}
		LRUCache[setIndex][associativity-1] = address;
	}
}
	printSummary(hitCount, missCount, evictionCount); 
	fclose(fp);
  return 0;
}
