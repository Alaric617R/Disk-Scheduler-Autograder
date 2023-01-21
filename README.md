# EECS 482 P1 Disk Scheduler Autograder
## Part 1 - Test case generator
+ ### Intro:
  The source file of generating testcases is contained in `genTest.cpp`. Please do not modify the names of the macros or it won't connect with the simplity of `Makefile`. Alaric is trying his best to offer a simple interface to using this little testcase generator (you can write one too! But it's like a pesky bee)
+ ### Usage:
```make
    make genTest
```
  After `make genTest`, three prompts will appear at command line asking for configuration.
```make
    enter test seq (integer):
    enter number of disk requests:
    enter maximum request a thread can issue:
```
  Enter parameters of the testcases:
  + A test sequence number `k`. This is a control number of your testcases generated at this term. All of them are confined to the directory `test${k}`.
  + The number of disk requests `N`, from `disk.in0` to `disk.in${N}`. They can be found in directory `test${k}`.
  + The maximum number of track requests a requester thread can issue `M`. This parameter will make sure your requester files will contain disk requests in $[1,M]$ randomly.


## Part Two - Autograder
+ ### Intro: 
  The source file of the autograder is contained in `autograder.cpp`. Alaric doesn't want to articulate how useful it is. But the error message should be human readable. The rationale behind this autograder is to check the synchronization between servicing thread and requester threads. It will check, for example, whether the *disk queue* is filled to it's current maximum, i.e., `min(num_active_thread, max_disk_queue)`; whether a requester thread waits for it's previous request to be dealt with after issuing a new requests, etc. These are the bugs that enslaved Alaric for some while and hope the error message will give enough information to help you debug. The autograder, however, can't help you with over CPU time. For that kind of bug, it's suggested that you print out "*who is waiting*" kinda message using debug printing method. For myself, I use:
  ```c++
        #ifdef DEBUG
        std::cout << "server is waiting\n";
        #endif

  ```
It will be very handy to show this kinda messages just add `-DDEBUG` flag when you're compiling your disk scheduler. In general, as long as you've avoided busy waiting you CPU time will be fine. Think carefully how `cv` and `mutex` is going to help you.
+ ### Usage
Before doing anything, make sure this `Makefile`, `autograder.cpp` and `genTest.cpp` are along with your disk scheduler executable. **MAKE SURE YOUR EXECUTABLE IS NAMED AS `scheduler`**. Or Alaric has to add a new prompt for whenever you're trying to use the autograder you have to retype your name (sounds cumbersome). It's encourage you integrate your own Makefile with Alaric's autograder Makefile so that you won't have to worry about multiple makefiles. Dump them (with your executable named `scheduler`) to another subdirectory is also gonna work.
+ To build the `autograder`, simply type the following in your terminal:
```make
make autograder
```


