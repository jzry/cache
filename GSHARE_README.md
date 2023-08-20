# README

## Compilation, execution, and arguments

All my C++ code for the gshare simulator is in gshare.cpp.

To compile ghshare do:
```g++ -o SIM gshare.cpp```

To execute gshare do:
```./SIM 5 4 mcf_trace.txt```

The arguments for the executable are as follows:
``` ./SIM <M> <N> <FILENAME>```

and the output produced is formatted as follows:
``` M N MISPREDICTION_RATE```

## Running the shell scripts

runa.sh, runb.sh, runc.sh, and validation.sh are bash scripts that let you easily test part A B C of the report using my program. They make it easier for you to validate my work.

To run each bash script, type:
``` bash runa.sh``` and wait for the work to load, it may take about 15 seconds to load the output since mcf_trace and gobmk_trace are big files.

## The report

The report is a PDF that holds the result of the bash scripts in a compiled, easier to read manner. If you want to validate my program, run my bash script files which compile and execute the program using the different arguments required by part A, B, and C in the project PDF.

The JPEG files are the same results achieved running the bash scripts, I simply compiled them all to make it easier to read. The same JPEG files are in the report PDF.
