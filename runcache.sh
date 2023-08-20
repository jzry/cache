g++ -o SIM cache.cpp

# 8K writeback
echo "writeback"
echo " "
echo "8K XSBENCH.t writeback"
./SIM 8192 4 0 1 XSBENCHSmall.t
echo "8K MINIFE.t writeback"
./SIM 8192 4 0 1 MINIFESmall.t
echo " "

# 8K write-through
echo "write-through"
echo " "
echo "8K XSBENCH.t write-through"
./SIM 8192 4 0 0 XSBENCHSmall.t
echo "8K MINIFE.t write-through"
./SIM 8192 4 0 0 MINIFESmall.t
echo " "

# 16K writeback
echo "writeback"
echo " "
echo "16K XSBENCH.t writeback"
./SIM 16384 4 0 1 XSBENCHSmall.t
echo "16K MINIFE.t writeback"
./SIM 16384 4 0 1 MINIFESmall.t
echo " "

# 16K write-through
echo "write-through"
echo " "
echo "16K XSBENCH.t write-through"
./SIM 16384 4 0 0 XSBENCHSmall.t
echo "16K MINIFE.t write-through"
./SIM 16384 4 0 0 MINIFESmall.t
echo " "

# 32K writeback
echo "writeback"
echo " "
echo "32K XSBENCH.t writeback"
./SIM 32768 4 0 1 XSBENCHSmall.t
echo "32K MINIFE.t writeback"
./SIM 32768 4 0 1 MINIFESmall.t
echo " "

# 32K write-through
echo "write-through"
echo " "
echo "32K XSBENCH.t write-through"
./SIM 32768 4 0 0 XSBENCHSmall.t
echo "32K MINIFE.t write-through"
./SIM 32768 4 0 0 MINIFESmall.t
echo " "

# 64K writeback
echo "writeback"
echo " "
echo "64K XSBENCH.t writeback"
./SIM 65536 4 0 1 XSBENCHSmall.t
echo "64K MINIFE.t writeback"
./SIM 65536 4 0 1 MINIFESmall.t
echo " "

# 64K write-through
echo "write-through"
echo " "
echo "64K XSBENCH.t write-through"
./SIM 65536 4 0 0 XSBENCHSmall.t
echo "64K MINIFE.t write-through"
./SIM 65536 4 0 0 MINIFESmall.t
echo " "

# 128K writeback
echo "writeback"
echo " "
echo "128K XSBENCH.t writeback"
./SIM 131072 4 0 1 XSBENCHSmall.t
echo "128K MINIFE.t writeback"
./SIM 131072 4 0 1 MINIFESmall.t
echo " "

# 128K write-through
echo "write-through"
echo " "
echo "128K XSBENCH.t write-through"
./SIM 131072 4 0 0 XSBENCHSmall.t
echo "128K MINIFE.t write-through"
./SIM 131072 4 0 0 MINIFESmall.t
echo " "
