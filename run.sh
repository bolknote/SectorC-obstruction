#!/bin/sh

SC=../sectorc

if [ ! -d "$SC" ]; then
	echo SectorC compiler is not found
	exit 1
fi

cat "$SC/rt/lib.c" obf_hello.c "$SC/rt/_start.c" | "$SC/run_raw.sh" - 2>&-
