g++-4.7 -std=gnu++11 -Wzero-as-null-pointer-constant -I../.. -O3 -fomit-frame-pointer -c ../phoenix.cpp -DPHOENIX_REFERENCE
g++-4.7 -std=gnu++11 -Wzero-as-null-pointer-constant -I../.. -O3 -fomit-frame-pointer -c test.cpp
g++-4.7 -s -o test-reference test.o phoenix.o
rm *.o
