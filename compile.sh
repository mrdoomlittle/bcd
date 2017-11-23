cd ../bci; sh compile.sh; cd ../bcd;
cd ../bitct; sh compile.sh; cd ../bcd;
gcc -std=c11 -I../bci/inc -I../bitct/inc -L../bci/lib -L../bitct/lib -o bcd bcd.c -lmdl-bci -lmdl-bitct
