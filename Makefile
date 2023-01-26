


.PHONY: clean
clean:
	rm -rf autograder scheduler
#################### You can start your variable definition above ####################
__TEST_SEQ ?= $(shell bash -c 'read -p "enter test seq (integer): " dir; echo $$dir')
__PER_THREAD_REQUEST_MAX ?= $(shell bash -c 'read -p "enter maximum request a thread can issue: " dir; echo $$dir')
__NUM_DISK_REQUEST ?= $(shell bash -c 'read -p "enter number of disk requests: " dir; echo $$dir')
ONE := 1
N = $(shell echo ${NUM_DISK_REQUEST} - ${ONE} | bc)
NUMBERS = $(shell seq 0 ${N})
DISK_INS = $(NUMBERS:%=disk.in%)
DIRNAME = intensive_test_
__MAX_QUEUE_CAP = $(shell bash -c 'read -p "enter maximum disk queue capacity: " cap; echo $$cap')
MAX_QUEUE_CAP = $(__MAX_QUEUE_CAP)
#################### You can rename your output .txt as you like here ####################
YOUR_TEST_FILE_NAME = my_result.txt
#################### You can rename your output .txt as you like here ####################


#################### You can add your own recipes below ####################








#################### Don't touch anything below if you want everything to work as designed ####################
.PHONY: genTest autograder grade
.ONESHELL:
genTest: genTest.cpp
	$(eval TEST_SEQ=${__TEST_SEQ})
	$(eval NUM_DISK_REQUEST=${__NUM_DISK_REQUEST})
	$(eval PER_THREAD_REQUEST_MAX=${__PER_THREAD_REQUEST_MAX})
	rm -rf test$(TEST_SEQ)
	mkdir test$(TEST_SEQ)
	g++ -Wall -std=c++17 -DTEST_SEQ=$(TEST_SEQ) -DDEBUG -DNUM_DISK_REQUEST=$(NUM_DISK_REQUEST) -DPER_THREAD_REQUEST_MAX=$(PER_THREAD_REQUEST_MAX) -o $@ $^
	./genTest

autograder:autograder.cpp
	g++ -g -Wall -std=c++17 -o $@ $^

grade: autograder
	$(eval TEST_SEQ=${__TEST_SEQ})
	$(eval MAX_QUEUE_CAP=${__MAX_QUEUE_CAP})
	$(eval NUM_DISK_REQUEST=${__NUM_DISK_REQUEST})
	cp autograder test$(TEST_SEQ)
	cd test$(TEST_SEQ); \
	./autograder $(MAX_QUEUE_CAP) $(NUM_DISK_REQUEST) ${YOUR_TEST_FILE_NAME} $(DISK_INS)

.PHONY: test
test: scheduler
#	mkdir -p $(DIRNAME)$(TEST_TAG)
	$(eval TEST_SEQ=${__TEST_SEQ})
	$(eval MAX_QUEUE_CAP=${__MAX_QUEUE_CAP})\
	$(eval NUM_DISK_REQUEST=${__NUM_DISK_REQUEST})
	rm -f test$(TEST_SEQ)/scheduler
	cp scheduler test$(TEST_SEQ)
	cd test$(TEST_SEQ); \
	./scheduler $(MAX_QUEUE_CAP) $(DISK_INS) > ${YOUR_TEST_FILE_NAME}