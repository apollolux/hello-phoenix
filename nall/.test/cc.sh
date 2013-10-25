#debugging
#g++-4.7 -std=gnu++11 -Wzero-as-null-pointer-constant -g -o test test.cpp -I../.. -lX11 -pthread

#optimizing
g++-4.7 -std=gnu++11 -Wzero-as-null-pointer-constant -Wreturn-type -march=native -O3 -fomit-frame-pointer -s -o test test.cpp -I../.. -lX11 -pthread

#profiling
#g++-4.7 -std=gnu++11 -O3 -pg -o test test.cpp -I../../ -lX11 -pthread

