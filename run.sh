#!/bin/sh

SC=../sectorc
cat $SC/rt/lib.c obf_hello.c $SC/rt/_start.c | $SC/run_raw.sh - 2>&-
