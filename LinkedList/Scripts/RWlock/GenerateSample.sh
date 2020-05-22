#!/bin/bash

gcc linkedListRWlock.c -lpthread

for (( i=0; i < 1000; ++i ))

do
	./a.out 8 1000 10000 0.99 0.005 0.005
done
