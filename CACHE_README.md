# To Compile:

1. Enter ```g++ -o SIM cache.cpp``` in command line / terminal / shell.
This will generate an executable called SIM

# To Run:

1. Enter ```./SIM 32768 8 1 1 /home/TRACES/MCF.t``` in command line / terminal / shell.
2. Wait for output (which will take a very long time because of the for loops).
3. If you want to see what my program is doing, open cache.cpp and in the header macros change DEBUG variable from 0 to 1, so you can see as the program finishes. The XSBench.t file can take 20-30 minutes to finish due to for loops, MINIFE.t may take a shorter time period to load (15 minutes).
NOTE: PLACE FILE IN THE SAME DIRECTORY AS THE EXECUTABLE (THEY ARE BOTH IN THE SAME FOLDER) SO YOU CAN SIMPLY DO: ```./SIM 32768 8 1 1 MCF.t```. BUT IT SHOULD ALSO WORK IF YOU PLACE THE INPUT FILE IN ANOTHER DIRECTORY.

## Command line parameters are:
(EXECUTABLE_NAME) (CACHE_SIZE) (ASSOC) (REPLACEMENT) (WB) (TRACE_FILE_DIR/NAME)

## Further information:
<CACHE_SIZE> is the size of the simulated cache in bytes
<ASSOC> is the associativity
<REPLACEMENT> replacement policy: 0 means LRU, 1 means FIFO
<WB> Write-back policy: 0 means write-through, 1 means write-back
<TRACE_FILE> trace file name with full path

### Example explained:
./SIM 32768 8 1 1 /home/TRACES/MCF.t
This will simulate a 32KB write-back (because we use a 1 in the 3rd index argument) cache with 8-way associativity and FIFO replacement policy (because we use a 1 in the 4th index argument). The memory trace will be read from /home/TRACES/MCF.t

# Output:

a. The total miss ratio for L1 cache
b. The # writes to memory
c. The # reads from memory

# Validation runs:

./SIM 32768 8 0 1 /home/TRACES/XSBENCH.t
0.112008 
44.000000 
2371758.000000

./SIM 32768 8 0 0 /home/TRACES/XSBENCH.t
0.112008 
5013495.000000
2371758.000000

# File inputs and running XSBench.t:
- If you want a quick file to test, I took the first 2000 records from MINIFE.t and copy / pasted them into mid.t, and small.t contains only 42 records from XSBench.t.

- XSBench.t and MINIFE.t take like 20-30 minutes to run and I couldn't get my computer to finish running them but the program does work. I included a JPEG of the 10,000,000th record that was what I reached before my computer crashed from running the program because it took to long.
