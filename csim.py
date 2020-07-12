#!/usr/bin/env python3
import sys
import getopt
import re
print(sys.argv[1:])
optlist,args = getopt.getopt(sys.argv[1:],"t:E:b:s:v::h::")
print(optlist)
#exit(0)
associativity =1
traceFile = ""
blockBits = 0
setBits= 1
verbose = 0
for (opt,val) in optlist:
	print(val)
	if opt == '-E':
		associativity = int(val)
	elif opt == '-s':
		setBits = int(val)
	elif opt == '-b':
		blockBits = int(val)
	elif opt == '-t':
		traceFile = str(val)
	elif opt == '-v':
		verbose = 1
numSets = 1<<setBits
LRUCache = [[-1 for i in range(associativity)]for j in range(numSets)]
LRUIndex = [0 for _ in range(numSets)]
hitCount  = 0
missCount = 0
evictionCount = 0
numLoads= 0
numMod = 0
numStores = 0
file_handler = open(traceFile, "r")
l = 0
for trace in file_handler:
	l+=1
	traceList = re.split(r'[ ,]',trace)
	if traceList[0] == 'I':
		continue
	#print(traceList)
	t = traceList[1]
	if t == 'L':
		numLoads+=1
	if t == 'M':
		numMod +=1
	if t == 'S':
		numStores+=1
	address = int(traceList[2],16)
	address = address >>blockBits
	setIndex = address & (numSets-1)
	#print("setIndex=", setIndex)
	address = address >> setBits
	currIndex = LRUIndex[setIndex]
	currLRUIndex = currIndex
	c = 0
	emptySlot = 0
	emptySlotIndex = 0
	hitSuccess = 0
	while c < associativity:
			storedAddress= LRUCache[setIndex][currIndex]
			if storedAddress == -1 and emptySlot == 0:
				# cache entry empty
				emptySlot = 1
				emptySlotIndex = currIndex
			elif storedAddress == address:
				#print("hit")
				hitSuccess = 1
				hitCount+=1
				break
			currIndex+=1
			if currIndex == associativity:
				currIndex = 0
			c+=1
	if c == associativity and emptySlot == 1 and hitSuccess == 0:
		#print("empty slot and miss")
		missCount+=1
		LRUCache[setIndex][emptySlotIndex] = address
	elif c == associativity and emptySlot == 0 and hitSuccess == 0:
			#cache miss	
			#print("miss and eviction")
			evictionCount+=1
			missCount+=1
			LRUCache[setIndex][currLRUIndex] = address
			LRUIndex[setIndex]+=1
			if LRUIndex[setIndex] == associativity:
				LRUIndex[setIndex] = 0
	if t == 'M':
		hitCount+=1
	#print(LRUCache)
print("hitCount=", hitCount,"missCount=", missCount, "evictionCount=",evictionCount)
print("numLoads=", numLoads,"numMod=", numMod,"numStores=",numStores)
print(l)	
file_handler.close()


