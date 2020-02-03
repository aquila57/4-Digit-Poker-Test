#!/bin/bash
make -f pokerlib.mak clean
make -f poker.mak clean
make -f etauspokr.mak clean
make -f fibopokr.mak clean
make -f lfsrpokr.mak clean
make -f rupoker.mak clean
make -f sinepokr.mak clean
make -f gslpoker.mak clean
make -f diepoker.mak clean
rm -f alldie.out allgsl.out
