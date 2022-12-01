#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cmath>
#include "heap.h"

struct {
bool MEAN = false;
bool STD = false;
bool MIN = false;
bool FIRSTQ = false;
bool MEDIAN = false;
bool THIRDQ = false;
bool MAX = false;
} options;

struct{
float mean = 0;
    struct {
        //float count = 0;
        float value = 0;
        std::vector<float> linear_data;
    } std;
float min = 99999;
float max = 0;
    struct{
        float value = 0;
        minheap min_median;
        maxheap max_median;
    } median;
    
    struct{
        float value = 0;
        minheap min_firstq;
        maxheap max_firstq;
    } firstq;

    struct{
        float value = 0;
        minheap min_thirdq;
        maxheap max_thirdq;
    } thirdq;

} data_structures;

int track_index = 0;



class Stats{
    public:
std::string first_date = "";
std::string first_time = "";
std::string last_date;
std::string last_time;
/*
void calculate(std::vector<element> &data){
first_date = data[0].date;
first_time = data[0].time;
last_date = data[data.size()-1].date;
last_time = data[data.size()-1].time;

stats[0] = calculate_mean(data);
stats[1] = calculate_std(data);
stats[2] = calculate_min(data);
stats[3] = calculate_firstq(data);
stats[4] = calculate_median(data);
stats[5] = calculate_thirdq(data);
stats[6] = calculate_max(data);
};
*/
void calculate_mean(float new_value){
    
    data_structures.mean = data_structures.mean + (new_value - data_structures.mean)/(data_structures.std.linear_data.size()+1);
}
void calculate_std(){
    float sum = 0;
    for(int i = 0; i < data_structures.std.linear_data.size(); i++){
        sum += (data_structures.std.linear_data[i] - data_structures.mean)*(data_structures.std.linear_data[i] - data_structures.mean);
    } 
    data_structures.std.value = sqrt(sum/(data_structures.std.linear_data.size() - 1));
    return;
}

void calculate_median(){
    if (data_structures.median.min_median.get_size() > data_structures.median.max_median.get_size()){
        data_structures.median.value = data_structures.median.min_median.get_min();
        return ;
    }
    else{
        data_structures.median.value = (data_structures.median.max_median.get_max() + data_structures.median.min_median.get_min())/2;
        return;
    }
}
void calculate_firstq(){
 //gumbel quartile
    int total_size = data_structures.firstq.min_firstq.get_size() + data_structures.firstq.max_firstq.get_size();
    int state = total_size%4;
    float q1 = 0;

    if(state == 0){
        q1 = data_structures.firstq.min_firstq.get_min()*0.75 + data_structures.firstq.max_firstq.get_max()*0.25;
    }
    else if(state == 1){
        q1 = data_structures.firstq.max_firstq.get_max();
    }
    else if(state == 2){
        q1 = data_structures.firstq.max_firstq.get_max()*0.75 + data_structures.firstq.min_firstq.get_min()*0.25;
    }
    else if(state == 3){
        q1 = (data_structures.firstq.max_firstq.get_max() + data_structures.firstq.min_firstq.get_min())/2;
    }
    data_structures.firstq.value = q1;
}

void calculate_thirdq(){
 //gumbel quartile
    int total_size = data_structures.thirdq.min_thirdq.get_size() + data_structures.thirdq.max_thirdq.get_size();
    int state = total_size%4;
    float q3 = 0;

    if(state == 0){
        q3 = data_structures.thirdq.min_thirdq.get_min()*0.75 + data_structures.thirdq.max_thirdq.get_max()*0.25;
    }
    else if(state == 1){
        q3 = data_structures.thirdq.max_thirdq.get_max();
    }
    else if(state == 2){
        q3 = data_structures.thirdq.max_thirdq.get_max()*0.75 + data_structures.thirdq.min_thirdq.get_min()*0.25;
    }
    else if(state == 3){
        q3 = (data_structures.thirdq.max_thirdq.get_max() + data_structures.thirdq.min_thirdq.get_min())/2;
    }
    data_structures.thirdq.value = q3;
}
void print(){
    std::cout <<first_date<<","<<first_time<<","<<last_date<<","<<last_time<<",";
    if(options.MEAN){
        std::cout <<data_structures.mean<<",";
    }if(options.STD){
        std::cout <<data_structures.std.value<<",";
    }if(options.MIN){
        std::cout <<data_structures.min<<",";
    }if(options.FIRSTQ){
        std::cout <<data_structures.firstq.value<<",";
    }if(options.MEDIAN){
        std::cout <<data_structures.median.value<<",";
    }if(options.THIRDQ){
        std::cout <<data_structures.thirdq.value<<",";
    }if(options.MAX){
        std::cout <<data_structures.max<< std::endl;
    }
}
void write(){
    std::ofstream out;
    out.open("out.txt", std::ios::app);
    std::streambuf *coutbuf = std::cout.rdbuf(); 
    std::cout.rdbuf(out.rdbuf());
    std::cout <<first_date<<","<<first_time<<","<<last_date<<","<<last_time<<",";
    if(options.MEAN){
        std::cout <<data_structures.mean<<",";
    }if(options.STD){
        std::cout <<data_structures.std.value<<",";
    }if(options.MIN){
        std::cout <<data_structures.min<<",";
    }if(options.FIRSTQ){
        std::cout <<data_structures.firstq.value<<",";
    }if(options.MEDIAN){
        std::cout <<data_structures.median.value<<",";
    }if(options.THIRDQ){
        std::cout <<data_structures.thirdq.value<<",";
    }if(options.MAX){
        std::cout <<data_structures.max;
    }
    std::cout << std::endl;
    std::cout.rdbuf(coutbuf);
}


};

Stats stats;

class manager{
    public:
        
        void add_element_heap_median(float &e){
           if(data_structures.median.min_median.get_size() == data_structures.median.max_median.get_size()){
                data_structures.median.max_median.insert(e);
                float transfer = data_structures.median.max_median.extract_max();
                data_structures.median.min_median.insert(transfer);
           }
           else{
                data_structures.median.min_median.insert(e);
                float transfer = data_structures.median.min_median.extract_min();
                data_structures.median.max_median.insert(transfer);
           }
        }
        void add_element_heap_firstq(float &e){
            int state = (data_structures.firstq.min_firstq.get_size() + data_structures.firstq.max_firstq.get_size())%4;
            if(state == 0){
                if(e < data_structures.firstq.max_firstq.get_max()){
                    data_structures.firstq.max_firstq.insert(e);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                    float transfer = data_structures.firstq.min_firstq.extract_min();
                    data_structures.firstq.max_firstq.insert(transfer);
                }
            }
            else if(state == 1){
                if(e < data_structures.firstq.max_firstq.get_max()){
                    data_structures.firstq.max_firstq.insert(e);
                    float transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
            else if(state == 2){
                if(e < data_structures.firstq.min_firstq.get_min()){
                    data_structures.firstq.max_firstq.insert(e);
                    float transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
            else if(state == 3){
                if(e < data_structures.firstq.min_firstq.get_min()){
                    data_structures.firstq.max_firstq.insert(e);
                    float transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
        }
        void add_element_heap_thirdq(float &e){
            int state = (data_structures.thirdq.min_thirdq.get_size() + data_structures.thirdq.max_thirdq.get_size())%4;
            if(state == 0){
                if(e < data_structures.thirdq.max_thirdq.get_max()){
                    data_structures.thirdq.max_thirdq.insert(e);
                }
                else{
                    data_structures.thirdq.min_thirdq.insert(e);
                    float transfer = data_structures.thirdq.min_thirdq.extract_min();
                    data_structures.thirdq.max_thirdq.insert(transfer);
                }
            }
            else if(state == 1){
                if(e < data_structures.thirdq.max_thirdq.get_max()){
                    data_structures.thirdq.max_thirdq.insert(e);
                    float transfer = data_structures.thirdq.max_thirdq.extract_max();
                    data_structures.thirdq.min_thirdq.insert(transfer);
                }
                else{
                    data_structures.thirdq.min_thirdq.insert(e);
                }
            }
            else if(state == 2){
                if(e < data_structures.thirdq.min_thirdq.get_min()){
                    data_structures.thirdq.max_thirdq.insert(e);
                    float transfer = data_structures.thirdq.max_thirdq.extract_max();
                    data_structures.thirdq.min_thirdq.insert(transfer);
                }
                else{
                    data_structures.thirdq.min_thirdq.insert(e);
                }
            }
            else if(state == 3){
                if(e < data_structures.thirdq.min_thirdq.get_min()){
                    data_structures.thirdq.max_thirdq.insert(e);
                    float transfer = data_structures.thirdq.max_thirdq.extract_max();
                    data_structures.thirdq.min_thirdq.insert(transfer);
                }
                else{
                    data_structures.thirdq.min_thirdq.insert(e);
                }
            }
        }

        void add_element(float &e){
           if(options.MEAN || options.STD){
               stats.calculate_mean(e);
           }
           if(options.STD){
              data_structures.std.linear_data.push_back(e);
               stats.calculate_std();
           }
           if(options.MIN){
               if(e < data_structures.min){
                   data_structures.min = e;
               }
           }
           if(options.MAX){
               if(e > data_structures.max){
                   data_structures.max = e;
               }
           }
           if(options.FIRSTQ){
                add_element_heap_firstq(e);
                stats.calculate_firstq();
           }
           if(options.MEDIAN){
                add_element_heap_median(e);
                stats.calculate_median();
           }
           if(options.THIRDQ){
                add_element_heap_thirdq(e);
                stats.calculate_thirdq();
           }
        }
};

//std::vector<struct element> data;

void reader(char* file) {
    std::ifstream reader;
    std::string line;
    manager manager;

    reader.open(file);
    getline(reader, line);
    //std::cout << line << std::endl;
    int stat_count = stoi(line);
    for(int i = 0; i < stat_count; i++){
        getline(reader, line);
        //std::cout <<line << std::endl;
            if(line == "mean"){
                options.MEAN = true;
            }else if(line == "std"){
                options.STD = true;
            }else if(line == "min"){
                options.MIN = true;
            }else if(line == "firstq"){
                options.FIRSTQ = true;
            }else if(line == "median"){
                options.MEDIAN = true;
            }else if(line == "thirdq"){
                options.THIRDQ = true;
            }else if(line == "max"){
                options.MAX = true;
            }
        }

    
    
    getline(reader, line);
    getline(reader, line, ',');
    int data_count = stoi(line);
    //std::cout << data_count << std::endl;
    getline(reader, line);
    //std::cout << line << std::endl;
    if(line == "gap"){
        track_index = 2;
    }else if(line == "grp"){
        track_index = 3;
    }else if(line == "v"){
        track_index = 4;
    }else if(line == "gi"){
        track_index = 5;
    }

    for(int i = 0; i < data_count; i++){
        getline(reader, line);
        if(line == "add"){
            getline(reader, line, ',');
            //std::cout << line << std::endl;
            if(stats.first_date == ""){
                stats.first_date = line;
            }
            stats.last_date = line;
            getline(reader, line, ',');
            if(stats.first_time == ""){
                stats.first_time = line;
            }
            stats.last_time = line;
           for(int i = 0; i < track_index - 1; i++){
                getline(reader, line, ',');
            }
           std::cout <<"Added Value is: " <<line << std::endl;
           float temp = stof(line);
           manager.add_element(temp);
            
        }
        if(line == "print"){
            //stats.calculate(data);
            stats.write();
            stats.print();
        }
    }

    reader.close();
    return;
}

void test_heap(minheap &heap){
heap.insert(1);
    heap.insert(2);
    heap.insert(3);
    heap.insert(4);
    heap.insert(5);
    heap.insert(6);
    heap.insert(7);
    heap.insert(8);
    heap.insert(9);
    heap.insert(10);
    heap.insert(11);
    heap.insert(12);
    heap.insert(13);
    heap.insert(1.4);
    heap.print();
    if(heap.check_heap()){
        std::cout << "Heap is valid" << std::endl;
    }else{
        std::cout << "Heap is not valid" << std::endl;
    }
    std:: cout << heap.extract_min();
    heap.print();
    if(heap.check_heap()){
        std::cout << "Heap is valid" << std::endl;
    }else{
        std::cout << "Heap is not valid" << std::endl;
    }

}

int main(int argc, char** argv) {
   reader(argv[1]);

  	return 0;
}
