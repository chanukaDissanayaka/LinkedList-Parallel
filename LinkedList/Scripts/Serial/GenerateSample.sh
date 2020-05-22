#!/bin/bash

gcc linkedListSerial.c 

for (( i=0; i < 1000; ++i ))

do
	./a.out 1000 10000 0.50 0.25 0.25
done
