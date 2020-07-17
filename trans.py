#!/usr/bin/env python3
import random
A = [[random.randint(1,5) for i in range(5)]for j in range(5)]
B = [[0 for i in range(5)] for j in range(5)]
for i in range(5):
	for j in range(5):
		temp = A[i][j]
		B[j][i] = temp
print(A)
print(B)

