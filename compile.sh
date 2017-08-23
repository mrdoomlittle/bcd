cd ../bci; sh compile.sh; cd ../bcd;
cd ../8xdrm; sh compile.sh; cd ../bcd;
gcc -std=c11 -I../bci/inc -I../8xdrm/inc -L../bci/lib -L../8xdrm/lib -o bcd bcd.c -lbci -l8xdrm
