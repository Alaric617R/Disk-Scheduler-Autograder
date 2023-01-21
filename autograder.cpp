#include <fstream>
#include <vector>
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
using std::string;
using std::stringstream;
using std::unordered_map;
using std::fstream;
using std::vector;
using std::queue;
using std::unique_ptr;
using namespace std;

struct track_id{
    size_t track;
    size_t id;
};

size_t server_track(const string& line){
    stringstream ss(line);
    string buff;
    size_t track;
    ss >> buff >> buff >> buff >> buff;
    ss >> track;
    return track;
}

size_t server_id(const string& line){
    stringstream ss(line);
    string buff;
    size_t id;
    ss >> buff >> buff;
    ss >> id;
    return id;
}
size_t requester_id(const string& line){
    stringstream ss(line);
    string buff;
    size_t id;
    ss >> buff;
    ss >> id;
    return id;
}

size_t requester_track(const string& line){
     stringstream ss(line);
    string buff;
    size_t track;
    ss >> buff >> buff >> buff;
    ss >> track;
    return track;
}

class Disk_queue{
    private:
    size_t prev = 0;
    vector<size_t> dq;
    vector<size_t> ids;
    public:
    void push(const size_t& val, const size_t& id){
        dq.push_back(val);
        ids.push_back(id);
    }
    track_id pop(){
        if (dq.empty()){
            throw std::runtime_error("Popping when empty!");
        }
        size_t min = abs( (int)(dq.front()) - (int)( prev) );
        size_t idx = 0;
        for (size_t i = 0 ; i <dq.size(); i++){
            size_t diff = abs( (int)dq[i] - (int)prev );
            if(diff < min){
                min = diff;
                idx = i;
            }
        }
        size_t track = dq[idx];
        size_t id = ids[idx];
        prev = track;
        dq.erase(dq.begin()+(int)idx);
        ids.erase(ids.begin()+(int)idx);
        track_id ret;
        ret.id = id;
        ret.track = track;
        return ret;
    }
    size_t size() const{return dq.size();}
    bool isEmpty() const {return this->size() == 0;}
    int find_id_matched_track(const size_t& id, const size_t& track){
        int idx = 0;
        for (; idx < (int)dq.size(); idx++){
            if (dq[idx] == track && ids[idx] == id) break;
        }
        return idx;
    }
    void erase(const int& pos){
        dq.erase(dq.begin() + pos);
        ids.erase(ids.begin() + pos);
    }
};

class Requester_tracks{
    queue<size_t> track_queue;
    bool prev_finished = true;
    int id = -1;
    public:
    Requester_tracks(int id_ = -1):id{id_}{}
    bool isWaiting() const {return !prev_finished;}
    void enableNextReq() {prev_finished = true;}
    size_t pop(){
        prev_finished = false;
        size_t top = track_queue.front();
        track_queue.pop();
        return top;
    }
    void push(const size_t& val){
        track_queue.push(val);
    }
    size_t size() const {return track_queue.size();}
    bool isEmpty() const {return this->size() == 0;}
    friend ostream& operator<<(ostream& os,const Requester_tracks& other){
        queue<size_t> temp = other.track_queue;
        os << "------" << " id = " << other.id << " ------\n";
        while(!temp.empty()){
            os << '\t' << temp.front() << "\t\n";
            temp.pop();
        }
        return os;
    }
};

int main(int argc, char** argv){
    // ./ag_sim max_disk_queue num_disk_file WHOEVER.out test#/disk_0 ... test#/disk_n
    size_t num_requesters = atoi(argv[2]);
    size_t max_disk_queue = atoi(argv[1]);
    vector< Requester_tracks* > requester_stash;
    unordered_map<size_t, int> hash_tbl;
    requester_stash.resize(num_requesters);
    Disk_queue disk_queue;
    size_t num_thread_active = num_requesters;
    size_t target_cap = min(num_thread_active, max_disk_queue);

    for (size_t i = 0; i < num_requesters; i++){
        auto que = new Requester_tracks((int)i);
        // auto que = unique_ptr< queue<size_t> >(new queue<size_t>());
        fstream in(argv[i+4]);
        size_t track;
        while(in >> track){
            (*que).push(track);
            auto iter = hash_tbl.find(track);
            if (iter == hash_tbl.end()){
                hash_tbl[track] = 1;
            }
            else
                (*iter).second ++;
        }
        requester_stash[i] = que;
        if (que->isEmpty())
            num_thread_active --;
    }
    #ifdef ALL_PRINT
    for (const auto& que : requester_stash){
        cout << (*que);
    }
    #endif
    ifstream to_be_checked_file(argv[3]);
    string line;
    size_t line_num = 1;
    bool noError = true;

    while(getline(to_be_checked_file,line)){
        if (line[0] == 's'){
            // check if disk queue has been filled to target_cap
            target_cap = min(num_thread_active,max_disk_queue);
            if (disk_queue.size()!= target_cap){
                cerr << "Disk queue not full! On line " << line_num << ": disk queue has reached capacity of "<< disk_queue.size() 
                <<" target_cap: " << target_cap << endl;
                noError = false;
                #ifdef DEBUG
                exit(6);
                #endif
            }
            size_t cur_track = server_track(line);
            size_t cur_id = server_id(line);
            auto fd = hash_tbl.find(cur_track);
            if (fd == hash_tbl.end()){
                cerr << "Wrong answer on line " << line_num << endl;
                cerr << "Output: " << line << ". No such request" << endl;
                noError = false;
                break;
            }
            (*fd).second --;
            // check if id and track matches
            track_id service_info = disk_queue.pop();
            if (service_info.track != cur_track){
                cerr << "Wrong answer on line " << line_num <<": should serve id="
                << service_info.id << " track=" << service_info.track << endl;
                cerr << "actual output id=" << cur_id << " track="<<cur_track << endl;
                noError = false;
                #ifdef DEBUG
                exit(6);
                #endif
            }
            // encountered same track number but differnet id
            else if (service_info.id != cur_id){
                int idx = disk_queue.find_id_matched_track(cur_id,cur_track);
                disk_queue.erase(idx);
                disk_queue.push(service_info.track,service_info.id);
            }
            // check if num_thread_active has changed
            if (requester_stash[cur_id]->isEmpty())
                num_thread_active --;
            // signal current thread to quit waiting
            requester_stash[cur_id]->enableNextReq();
        }
        else if (line[0] == 'r'){
            // check capacity of the disk queue
            target_cap = min(num_thread_active, max_disk_queue);
            if (target_cap <= disk_queue.size()){
                cerr << "Disk queue already full! On line " << line_num << ": disk queue has reached capacity of "<< disk_queue.size() 
                <<" target_cap: " << target_cap << endl;
                noError = false;
                #ifdef DEBUG
                exit(6);
                #endif
            }

            size_t id = requester_id(line);
            auto cur_req_queue = requester_stash[(int)id];
            #ifdef ALL_PRINT
            cout << (*cur_req_queue);
            #endif
            size_t track = requester_track(line);
            // check whether current thread is legible of issuing request
            if (cur_req_queue->isWaiting()){
                cerr << "Wrong answer on line " << line_num << ": requester " << id << " should be waiting previous request.\n" <<endl;
                noError = false;
                #ifdef DEBUG
                exit(6);
                #endif
            }
            // check whether it's the top of the queue being issued
            size_t correct_out = cur_req_queue->pop();
            if ( correct_out != track ){
                cerr << "Wrong answer on line " << line_num;
                cerr << ": output " << track << " should be " << correct_out << endl;
                noError = false;
                #ifdef DEBUG
                exit(6);
                #endif
            }
            // push result into disk queue
            disk_queue.push(track,id);
        }
        else if (line == "No runnable threads.  Exiting.") continue;
        else{
            cerr << "Wrong answer on line " << line_num << endl;
            cerr << "Unkown print\n";
            noError = false;
        }
        line_num ++;
    }
    for (size_t i = 0; i < requester_stash.size(); i++){
        if ( (*requester_stash[i]).size() != 0){
            cerr << "Requester " << i << " wrong! Requestes not fully processed!\n";
            noError = false;
        }
    }
    for (const auto i : hash_tbl){
        if(i.second != 0){
            cerr << "Server wrong! Track " << i.first << " not served!\n";
            noError = false;
        }
    }
    for (auto q : requester_stash){
        if(!q->isEmpty()){
            cout << (*q);
        }
    }
    // cout << "Scheduler runs successfully!\n";
    for (auto q : requester_stash){
        delete q;
    }
    if (noError){
        cout << "Congrats! scheduler runs successfully!\n";
    }
    return 0;
}
/* debugging disk_queue code snippet
    while(getline(to_be_checked_file,line)){
        if(line[0] == 'r'){
            disk_queue.push(requester_track(line));
        }
        if(line[0] == 's'){
            cout << disk_queue.pop() << endl;
        }
    }
    to_be_checked_file.clear();
    to_be_checked_file.seekg(0,to_be_checked_file.beg);

*/