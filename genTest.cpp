#include <fstream>
#include <random>
#include <string>
#include <iostream>

#ifndef NUM_DISK_REQUEST
#define NUM_DISK_REQUEST 10
#endif

#ifndef TEST_SEQ
#define TEST_SEQ 1
#endif

#ifndef PER_THREAD_REQUEST_MAX
#define PER_THREAD_REQUEST_MAX 6
#endif

using std::string;
using std::ofstream;
using namespace std;

random_device  rand_dev;
mt19937  generator(rand_dev());
uniform_int_distribution<int> track_distr(0,999);
uniform_int_distribution<int> req_num_distr(1,PER_THREAD_REQUEST_MAX);

int main(){
    string test_dir = string("test") + to_string(TEST_SEQ);
    #ifdef DEBUG
    cout << test_dir << '\n';
    #endif
    for(int i = 0 ; i < NUM_DISK_REQUEST; i++){
        ofstream out(test_dir + "/disk.in" + to_string(i));
        int num_req = req_num_distr(generator);
        for(int j = 0; j < num_req; j++){
            out << track_distr(generator) << '\n';
        }
        out.close();
    }
    return 0;
}
