g++ -o SIM gshare.cpp

echo "M = 8 and N = 5"
echo " "
./SIM 8 5 mcf_trace.txt
./SIM 8 5 gobmk_trace.txt
echo " "

echo "M = 4 and N = 2"
echo " "
./SIM 4 2 mcf_trace.txt
./SIM 4 2 gobmk_trace.txt
echo " "

echo "M = 3 and N = 1"
echo " "
./SIM 3 1 mcf_trace.txt
./SIM 3 1 gobmk_trace.txt
echo " "
