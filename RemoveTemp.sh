#!/usr/bin/env bash

#RemoveTemp.sh - Cesar Quitazol

rm -f "Task1out.txt"
rm -f "Task1out.sh.txt"
rm -f "Task2out.txt"
rm -f "Task3out.txt"


for IDX in 3 4 5 6 7 8 9 10 11 12 13 14 15
do
    # For Task 2
    rm -f ".tmp.task2.words$IDX.txt"
    rm -f ".task2.words$IDX.txt"

    # ForTask 3
    rm -f ".task3.words$IDX.txt"

done