#  Disk Scheduler Project Autograder
## Part 1 - Test case generator
+ ### Intro:
  The source file of generating testcases is contained in `genTest.cpp`. Please do not modify the names of the macros or it won't connect with the simplicity of `Makefile`. Alaric is trying his best to offer a simple interface to using this little testcase generator (you can write one too! But it's like a pesky bee)
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
  >I've already include three test folders. `test1` is light-weight with only 10 files and `test3` is workload-intensive with 100 requester threads each could issue at most 260 requests. You may start from `test1`, then to middle one `test2`. After succeeding those two, drain your scheduler on `test3`.


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
Before doing anything, make sure this `Makefile`, `autograder.cpp` and `genTest.cpp` are along with your disk scheduler executable. **MAKE SURE YOUR EXECUTABLE IS NAMED AS `scheduler`**. Or Alaric has to add a new prompt for whenever you're trying to use the autograder you have to retype the name of your executable(sounds cumbersome). It's encouraged you integrate your own Makefile with Alaric's autograder Makefile so that you won't have to worry about multiple makefiles. Dump them (with your executable named `scheduler`) to another subdirectory is also gonna work. You might first need to generate some test files using `make genTest`. If you use your own test files, please put them into a subdirectory folder named **EXACTLY** as `testN` where `N` is an integer, leaving no white space there.
### Testing has two steps:
1. Run your `scheduler` and redirect output (default to *my_result.txt*) to a test folder you've specified.
```make
make test
### The following prompts will appear after you type `make test`. The answers to the prompt is an example.
enter test seq (integer): 1 # choose test1 folder
enter maximum disk queue capacity: 3 # as you like
enter number of disk requests: 10 # must be 10 since test1 only contains 10 disk files
```
You will find your run result inside `test1/my_result.txt`, using the above example.
> Just like Part one, the prompts specifies how to pass arguments to your disk scheduler. *Remember the test sequence number, you gonna will it when you run autograder on your my_result.txt*

**Note:** The number of disk requests must be **EXACTLY** the same as the number of `disk.in#` file in `test#` folder. So you might need to do a little counting in that testcase folder where you're going to run your `scheduler` on. It's meant to generate a long, tedious array of *disk.in0 disk.in1 ... disk.inN* to feed your program.

2. Run autograder on your generated result using a single command of GNU make (yyds!). We proceed the example in step 1
```make
make grade
## The following prompts will appear after you type `make grade`. The answers to the prompt is an continuation of example in step 1.
enter test seq (integer): 1
# must be 1, b/c your output is in test1/my_result.txt
enter maximum disk queue capacity: 3
# must be 3, b/c you choose this number to run your scheduler. The autograder should know under what condition you've run your scheduler.
enter number of disk requests: 10
# must be 10, since test1 folder only contains 10 disk files
```
> Step 1 and step 2 is hugely coupled. You must remember the parameters when running your scheduler and tell the autograder the same amount of information. Otherwise, the grading process will yield buggy result and mislead your debug process :(

> You always **stay in the same directory as this Makefile**. The file redirection and hierachy are done within the `make` command.

>In case you want to run the autograder alone without the help of this Makefile, do it as follows:
```bash
 ./autograder max_disk_queue N my_result.txt disk.in0 disk.in1... disk.inN
 # N is number of disk files you've input to your scheduler
```
After running `make test` and `make grade`, you will be able to find the outcome in your terminal. You can also open up your run result in the test folder you specified.

## Maintenance
This autograder is written by Alaric when he is enslaved working on this project. It helped him expose all the bugs and Alaric thinks it will be a good idea to share this with all hardworking  students learning concurrent programming.

>If there are any bugs, typos, suggestions, etc. Please reach out through my Instagram `alaricieiuxcs`.

---
## Wish you good luck!
by Alaric Chen 21 Jan 2023
