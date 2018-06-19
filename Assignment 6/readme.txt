After-Action Report

I came across several errors while working on this homework. First, when passing in
the fourth argument into the pthread_create() function, I got the following warning:
    cast to 'int *' from smaller integer type 'int'
Realizing what the error was, I instead created a int array that held the id's of
all the threads and passed in the address of the appropriate id to the function.
Then, in thread helper function, I got this warning:
    warning: cast from pointer to integer of different size [-Wpointer-to\
    -int-cast]
To fix this error, I casted the function parameter to an int pointer and deferenced
it to fix the warning.

After running "make clean check", we got the following results: 
time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m40.960s
user	0m40.958s
sys	0m0.000s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m21.007s
user	0m41.326s
sys	0m0.002s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m11.027s
user	0m43.338s
sys	0m0.001s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m6.908s
user	0m46.292s
sys	0m0.002s
mv 8-test.ppm.tmp 8-test.ppm

Our implementation of SRT drastically improves the performance of the original one,
when we create multiple threads. As the number of threads increases, the real time
of the program decreases by about a factor of 2. The program's real time with 8
threads is almost 1/8th of the time it takes with 1 thread. However, the user time
slightly increaes with more threads and the system time is too small to make an
impact. Overall, our implementation shows that parallelism by implementing
multithreading can greatly improve program run times.
