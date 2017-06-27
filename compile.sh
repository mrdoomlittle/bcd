cd ../bci; sh compile.sh; cd ../bcd;
gcc -std=c11 -I../bci/inc -L../bci/lib -o bcd bcd.c -lbci
