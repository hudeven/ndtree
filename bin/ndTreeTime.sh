#!/bin/bash
START=$(date +%s)

./ndTree
#netstat
END=$(date +%s)
DIFF=$(($END - $START))
echo "It took $DIFF seconds"
