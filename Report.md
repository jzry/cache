# NOTE: FOR MY REPORT I USED XSBench.t and MINIFE.t's first 2000 records only since the 21,000,000 took 30 minutes to run each time.

# NOTE: RUN MY BASH SHELL SCRIPTS TO EASILY SEE HOW MY PROGRAM IS RUNNING AND TO VERIFY THE RESULTS ON HERE.
# TO RUN MY BASH SHELL SCRIPTS I HAVE INCLUDED THEM IN THE FOLDER. They are the following:
## runa.sh
## runb.sh
## runc.sh
## rund.sh

# A (use runa.sh to test)

## Using Writeback (mode 1), cache associativity 4, LRU replacement (mode 0), and cache sizes from 8KB-128KB.

### My program is calculating miss ratio as: num_misses / (num_memory_write + num_memory_reads)

### Cache size:
#### 8K - 8192

XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.927439

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.930613

#### 16K - 16384

XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.933824

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.927502

#### 32K - 32768

XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.938265

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.949321

#### 64K - 65536

XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.942312

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.971821

#### 128K - 131072

XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.943303

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.972603

## Conclusion:
There doesn't seem to be a lot of variability in the data, however they both seem to trend upwards in the miss ratio (have a higher miss ratio when increasing the cache size. This might be because there is more sets to hash to in the cache so there might be more misses which increases the number of misses (the numerator in the miss ratio), in turn increasing the miss ratio. XSBench file seems to achieve better miss ratio in most of the experiments but not by much (only 1-5%).

# B (use runb.sh to test)

## Comparing vs writeback (mode 1) and write-through (mode 0), cache associativity 4, LRU replacement (mode 0), and cache sizes from 8KB-128KB. Show memory read / write.

### Cache size:
#### 8K - 8192

##### MODE 1 (write-back):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.927439
Number of reads: 73967
Number of writes: 5787

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.930613
Number of reads: 82818
Number of writes: 6175

##### MODE 0 (write-through):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.430667
Number of reads: 73967
Number of writes: 97783

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.481483
Number of reads: 82818
Number of writes: 89188

#### 16K - 16384

##### MODE 1 (write-back):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.933824
Number of reads: 70852
Number of writes: 5021

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.927502
Number of reads: 78744
Number of writes: 6155

##### MODE 0 (write-through):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.428057
Number of reads: 70852
Number of writes: 94668

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.480562
Number of reads: 78744
Number of writes: 85114

#### 32K - 32768

##### MODE 1 (write-back):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.938265
Number of reads: 68225
Number of writes: 4489

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.949321
Number of reads: 76726
Number of writes: 4096

##### MODE 0 (write-through):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.425699
Number of reads: 68225
Number of writes: 92041

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.480072
Number of reads: 76726
Number of writes: 83096

#### 64K - 65536

##### MODE 1 (write-back):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.942312
Number of reads: 66694
Number of writes: 4083

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.971821
Number of reads: 75147
Number of writes: 2179

##### MODE 0 (write-through):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.424251
Number of reads: 66694
Number of writes: 90510

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.47967
Number of reads: 75147
Number of writes: 81517

#### 128K - 131072

##### MODE 1 (write-back):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.943303
Number of reads: 65768
Number of writes: 3953

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.972603
Number of reads: 74550
Number of writes: 2100

##### MODE 0 (write-through):
XSBENCH (FIRST 2000 RECORDS)
Miss ratio: 0.423348
Number of reads: 65768
Number of writes: 89584

MINIFE (FIRST 2000 RECORDS)
Miss ratio: 0.479514
Number of reads: 74550
Number of writes: 80920

## Conclusion:
There was a consistent growth from 92-93% to 94-97% in miss rate for writeback mode as we progressively grew the cache size.
There was a shrinkage from 43-48% to 42-47% in miss rate for write-through mode we progressively grew the cache size.
This may indicate that as the cache size grows, the miss rate shrinks for write-through. As the cache size grows for write-back the miss rate grows.

# C (use runc.sh to test)
LRU (mode 0), 32768 Cache size, Associativity from 1 to 64 in multiples of 2.

## XSBENCHSmall.t

1 assoc

MISS RATIO: 0.841826
WRITES TO MEMORY: 15812
READS FROM MEMORY: 84154

2 assoc

MISS RATIO: 0.84202
WRITES TO MEMORY: 15889
READS FROM MEMORY: 84687

4 assoc

MISS RATIO: 0.938265
WRITES TO MEMORY: 4489
READS FROM MEMORY: 68225

8 assoc

MISS RATIO: 0.974389
WRITES TO MEMORY: 1586
READS FROM MEMORY: 60341

16 assoc

MISS RATIO: 0.980553
WRITES TO MEMORY: 1180
READS FROM MEMORY: 59497

32 assoc

MISS RATIO: 0.982007
WRITES TO MEMORY: 1075
READS FROM MEMORY: 58669

64 assoc

MISS RATIO: 0.986365
WRITES TO MEMORY: 805
READS FROM MEMORY: 58236

128 assoc

MISS RATIO: 0.986843
WRITES TO MEMORY: 775
READS FROM MEMORY: 58129

## MINIFESmall.t

1 assoc

MISS RATIO: 0.926147
WRITES TO MEMORY: 6168
READS FROM MEMORY: 77349

2 assoc

MISS RATIO: 0.926323
WRITES TO MEMORY: 6176
READS FROM MEMORY: 77649

4 assoc

MISS RATIO: 0.949321
WRITES TO MEMORY: 4096
READS FROM MEMORY: 76726

8 assoc

MISS RATIO: 0.937656
WRITES TO MEMORY: 4972
READS FROM MEMORY: 74779

16 assoc

MISS RATIO: 0.92546
WRITES TO MEMORY: 6050
READS FROM MEMORY: 75115

32 assoc

MISS RATIO: 0.926509
WRITES TO MEMORY: 6043
READS FROM MEMORY: 76185

64 assoc

MISS RATIO: 0.925983
WRITES TO MEMORY: 6039
READS FROM MEMORY: 75550

128 assoc

MISS RATIO: 0.925543
WRITES TO MEMORY: 6041
READS FROM MEMORY: 75093

## Conclusion
As we increase associativity with write-back mode and LRU:
For XSBENCHSmall.t the miss ratio seems to increase from 84 to 98 as we increase the associativity of the cache.
For MINIFESmall.t the miss ratio seems to stay stagnant at 92% as we increase the associativity of the cache.

This could suggest that one file may be commiting more writes than another, but from XSBenchSmall.t there appears to be more writes when comparing it to the writes of MINIFESmall.t, so the miss rate is likely less.

# D (use rund.sh to test)
Use FIFO (Mode 1) and write-back and do same assoc increase as in part C.

## XSBENCHSmall.t

1 assoc

MISS RATIO: 0.828831
WRITES TO MEMORY: 18120
READS FROM MEMORY: 87740

2 assoc

MISS RATIO: 0.811002
WRITES TO MEMORY: 21206
READS FROM MEMORY: 90996

4 assoc

MISS RATIO: 0.817819
WRITES TO MEMORY: 20039
READS FROM MEMORY: 89956

8 assoc

MISS RATIO: 0.815073
WRITES TO MEMORY: 20403
READS FROM MEMORY: 89927

16 assoc

MISS RATIO: 0.815491
WRITES TO MEMORY: 20634
READS FROM MEMORY: 91198

32 assoc

MISS RATIO: 0.816334
WRITES TO MEMORY: 20771
READS FROM MEMORY: 92320

64 assoc

MISS RATIO: 0.812042
WRITES TO MEMORY: 21156
READS FROM MEMORY: 91401

128 assoc

MISS RATIO: 0.814168
WRITES TO MEMORY: 20742
READS FROM MEMORY: 90875

----------------
MINIFESmall.t
----------------
1 assoc

MISS RATIO: 0.961288
WRITES TO MEMORY: 3599
READS FROM MEMORY: 89370

2 assoc

MISS RATIO: 0.947272
WRITES TO MEMORY: 5121
READS FROM MEMORY: 92001

4 assoc

MISS RATIO: 0.945818
WRITES TO MEMORY: 5337
READS FROM MEMORY: 93164

8 assoc

MISS RATIO: 0.946406
WRITES TO MEMORY: 5301
READS FROM MEMORY: 93609

16 assoc

MISS RATIO: 0.947185
WRITES TO MEMORY: 5291
READS FROM MEMORY: 94888

32 assoc

MISS RATIO: 0.947986
WRITES TO MEMORY: 5312
READS FROM MEMORY: 96814

64 assoc

MISS RATIO: 0.948554
WRITES TO MEMORY: 5338
READS FROM MEMORY: 98421

128 assoc

MISS RATIO: 0.948633
WRITES TO MEMORY: 5352
READS FROM MEMORY: 98839

## Conclusion
As we increase the associativity using write-back mode and FIFO:
The miss ratio for XSBenchSmall seems to decrease from 82% and stay stagnant at 81%.
The miss ratio for MINIFESmall seems to decrease from 96% and stay stagnant at 94%.
This may indicate that when each cache set is bigger, it becomes easier for there to be misses because there are less evictions of data as new data is inserted into the cache. When associativity is small, the cache set can only hold one piece of data and it gets evicted when replaced with a new one, which results in higher miss rate.
