#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cmath>
#include <chrono>
#include "heap.h"

//to view time taken per calculation function enable PERF_STAT by setting it to 1
#define PERF_STAT 0

#if PERF_STAT
struct {
    struct simple_perf{
        std::chrono::duration<double,std::micro> time;
        int count = 0;
    };
    simple_perf mean, min, max, std;
    struct heap_perf{
        std::chrono::duration<double,std::micro> add_time;
        std::chrono::duration<double,std::micro> calculate_time;
        int count = 0;
    };
    heap_perf median, firstq, thirdq;
    
} perf;
#endif

//stores options of input file 
struct {
bool MEAN = false;
bool STD = false;
bool MIN = false;
bool FIRSTQ = false;
bool MEDIAN = false;
bool THIRDQ = false;
bool MAX = false;
} options;


//stores required data for statistics
struct{
    int count = 0;
double mean = 0;
    struct {
        double count = 0;
        double value = 0;
        double M2 = 0;
        //std::vector<double> linear_data;
    } std;
double min = 99999;
double max = 0;
    struct{
        double value = 0;
        minheap min_median;
        maxheap max_median;
    } median;
    
    struct{
        double value = 0;
        minheap min_firstq;
        maxheap max_firstq;
    } firstq;

    struct{
        double value = 0;
        minheap min_thirdq;
        maxheap max_thirdq;
    } thirdq;

} data_structures;

//variable that is set to the input feature
//used to read the input file
int track_index = 0;


//Stores date strings and calculate functions of each statistical value
class Stats{
    public:
std::string first_date = "";
std::string first_time = "";
std::string last_date;
std::string last_time;

void calculate_mean_std(double new_value){
    //Welford's online algorithm used to calculate mean and standard deviation
    //https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
    data_structures.count++;
    double delta = new_value - data_structures.mean;
    data_structures.mean += delta/data_structures.count;
    double delta2 = new_value - data_structures.mean;
    data_structures.std.M2 += delta*delta2;
    data_structures.std.value = sqrt(data_structures.std.M2/(data_structures.count-1));
    //data_structures.mean = data_structures.mean + (new_value - data_structures.mean)/(data_structures.std.linear_data.size()+1);
}
/*
void calculate_std(double delta){
    

    
    double sum = 0;
    for(int i = 0; i < data_structures.std.linear_data.size(); i++){
        sum += (data_structures.std.linear_data[i] - data_structures.mean)*(data_structures.std.linear_data[i] - data_structures.mean);
    } 
    data_structures.std.value = sqrt(sum/(data_structures.std.linear_data.size() - 1));
    
    return;
}
*/
//State encodings and their corresponding operations for heaps used to calculate median, first quartile and third quartile
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
    double q1 = 0;

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
    double q3 = 0;

    if(state == 0){
        q3 = data_structures.thirdq.min_thirdq.get_min()*0.25 + data_structures.thirdq.max_thirdq.get_max()*0.75;
    }
    else if(state == 1){
        q3 = data_structures.thirdq.min_thirdq.get_min();
    }
    else if(state == 2){
        q3 = data_structures.thirdq.max_thirdq.get_max()*0.25 + data_structures.thirdq.min_thirdq.get_min()*0.75;
    }
    else if(state == 3){
        q3 = (data_structures.thirdq.max_thirdq.get_max() + data_structures.thirdq.min_thirdq.get_min())/2;
    }
    data_structures.thirdq.value = q3;
}
//for debugging purposes
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

//the print function is modified to print the output to a file
void write(int index){
    std::ofstream out;
    if(index == 0){
        out.open("out.txt", std::ios::trunc); //clears the file
    }else{
        out.open("out.txt", std::ios::app); //appends to the file
    }
    std::streambuf *coutbuf = std::cout.rdbuf(); 
    std::cout.rdbuf(out.rdbuf());
    std::cout <<first_date<<","<<first_time<<","<<last_date<<","<<last_time;
    if(options.MEAN){
        std::cout<<"," <<data_structures.mean;
    }if(options.STD){
        std::cout << "," <<data_structures.std.value;
    }if(options.MIN){
        std::cout<<"," <<data_structures.min;
    }if(options.FIRSTQ){
        std::cout << "," <<data_structures.firstq.value;
    }if(options.MEDIAN){
        std::cout << "," <<data_structures.median.value;
    }if(options.THIRDQ){
        std::cout << "," <<data_structures.thirdq.value;
    }if(options.MAX){
        std::cout<< "," <<data_structures.max;
    }
    std::cout << std::endl;
    std::cout.rdbuf(coutbuf);
}


};

Stats stats;

//manager organises the data and calls the functions to calculate the statistics
class manager{
    public:
        
        void add_element_heap_median(double &e){
           if(data_structures.median.min_median.get_size() == data_structures.median.max_median.get_size()){
                data_structures.median.max_median.insert(e);
                double transfer = data_structures.median.max_median.extract_max();
                data_structures.median.min_median.insert(transfer);
           }
           else{
                data_structures.median.min_median.insert(e);
                double transfer = data_structures.median.min_median.extract_min();
                data_structures.median.max_median.insert(transfer);
           }
        }

        //States of sizes  (k, 3k), (k+1, 3k), (k+1, 3k+1),(k+1, 3k+2) are preserved in the heap
        void add_element_heap_firstq(double &e){
            int state = (data_structures.firstq.min_firstq.get_size() + data_structures.firstq.max_firstq.get_size())%4;
            if(state == 0){
                if(e < data_structures.firstq.max_firstq.get_max()){
                    data_structures.firstq.max_firstq.insert(e);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                    double transfer = data_structures.firstq.min_firstq.extract_min();
                    data_structures.firstq.max_firstq.insert(transfer);
                }
            }
            else if(state == 1){
                if(e < data_structures.firstq.max_firstq.get_max()){
                    data_structures.firstq.max_firstq.insert(e);
                    double transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
            else if(state == 2){
                if(e < data_structures.firstq.min_firstq.get_min()){
                    data_structures.firstq.max_firstq.insert(e);
                    double transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
            else if(state == 3){
                if(e < data_structures.firstq.min_firstq.get_min()){
                    data_structures.firstq.max_firstq.insert(e);
                    double transfer = data_structures.firstq.max_firstq.extract_max();
                    data_structures.firstq.min_firstq.insert(transfer);
                }
                else{
                    data_structures.firstq.min_firstq.insert(e);
                }
            }
        }
        //Similar to the firstq heap above
        void add_element_heap_thirdq(double &e){
            int state = (data_structures.thirdq.min_thirdq.get_size() + data_structures.thirdq.max_thirdq.get_size())%4;
            bool at_max_heap;
            if(e < data_structures.thirdq.max_thirdq.get_max()){
                data_structures.thirdq.max_thirdq.insert(e);
                at_max_heap = true;
            }
            else{
                data_structures.thirdq.min_thirdq.insert(e);
                at_max_heap = false;
            }

            if(state == 0){
                if(at_max_heap){
                    double transfer = data_structures.thirdq.max_thirdq.extract_max();
                    data_structures.thirdq.min_thirdq.insert(transfer);
                }
            }
            else if(state == 1){
                if(!at_max_heap){
                    double transfer = data_structures.thirdq.min_thirdq.extract_min();
                    data_structures.thirdq.max_thirdq.insert(transfer);
                }
            }
            else if(state == 2){
                if(!at_max_heap){
                    double transfer = data_structures.thirdq.min_thirdq.extract_min();
                    data_structures.thirdq.max_thirdq.insert(transfer);
                }
            }
            else if(state == 3){
                if(!at_max_heap){
                    double transfer = data_structures.thirdq.min_thirdq.extract_min();
                    data_structures.thirdq.max_thirdq.insert(transfer);
                }
            }
        }
        //calls the functions to calculate the statistics
        //does not run if the option is not set
        void add_element(double &e){
           if(options.MEAN || options.STD){
#if PERF_STATS
               auto start = std::chrono::high_resolution_clock::now();
#endif
               stats.calculate_mean_std(e);
#if PERF_STATS
               auto finish = std::chrono::high_resolution_clock::now();
               perf.mean.time = finish - start;
                perf.mean.count++;
#endif
           }
           /*
           if(options.STD){

               auto start = std::chrono::high_resolution_clock::now();
              data_structures.std.linear_data.push_back(e);
               stats.calculate_std();

               auto finish = std::chrono::high_resolution_clock::now();
               perf.std.time = finish - start;
                perf.std.count++;
           }
           */
           if(options.MIN){

#if PERF_STAT
               auto start = std::chrono::high_resolution_clock::now();
#endif
               //min and max are found directly here
               if(e < data_structures.min){
                   data_structures.min = e;
               }
#if PERF_STAT   
               auto finish = std::chrono::high_resolution_clock::now();
               perf.min.time = finish - start;
                perf.min.count++;
#endif

           }
           if(options.MAX){

#if PERF_STAT
               auto start = std::chrono::high_resolution_clock::now();
#endif
               if(e > data_structures.max){
                   data_structures.max = e;
               }
#if PERF_STAT   
               auto finish = std::chrono::high_resolution_clock::now();
               perf.max.time = finish - start;
                perf.max.count++;
#endif

           }

           if(options.FIRSTQ){
#if PERF_STAT
               auto start = std::chrono::high_resolution_clock::now();
#endif
                add_element_heap_firstq(e); //first element is added 
#if PERF_STAT
                auto finish = std::chrono::high_resolution_clock::now();
                auto start2 = std::chrono::high_resolution_clock::now();
#endif
                stats.calculate_firstq(); //firstq is calculated according to heaps
#if PERF_STAT   
                auto finish2 = std::chrono::high_resolution_clock::now();
                perf.firstq.add_time = finish - start;
                perf.firstq.calculate_time = finish2 - start2;
                perf.firstq.count++;
#endif
           }
           if(options.MEDIAN){
#if PERF_STAT   
               auto start = std::chrono::high_resolution_clock::now();
#endif
                add_element_heap_median(e);
#if PERF_STAT   
                auto finish = std::chrono::high_resolution_clock::now();
                auto start2 = std::chrono::high_resolution_clock::now();
#endif
                stats.calculate_median();
#if PERF_STAT   
                auto finish2 = std::chrono::high_resolution_clock::now();
                perf.median.add_time = finish - start;
                perf.median.calculate_time = finish2 - start2;
                perf.median.count++;
#endif
           }
           if(options.THIRDQ){
#if PERF_STAT   
               auto start = std::chrono::high_resolution_clock::now();
#endif
                add_element_heap_thirdq(e);
#if PERF_STAT   
                auto finish = std::chrono::high_resolution_clock::now();
                auto start2 = std::chrono::high_resolution_clock::now();
#endif
                stats.calculate_thirdq();
#if PERF_STAT   
                auto finish2 = std::chrono::high_resolution_clock::now();
                perf.thirdq.add_time = finish - start;
                perf.thirdq.calculate_time = finish2 - start2;
                perf.thirdq.count++;
#endif
           }
        }
};


void reader(char* file) {
    std::ifstream reader;
    std::string line;
    manager manager;

    reader.open(file);
    getline(reader, line); //gets the first line of input file
    int stat_count = stoi(line); //options are set depending on input file
    for(int i = 0; i < stat_count; i++){
        getline(reader, line);
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

    
    
    getline(reader, line); //skip the line
    getline(reader, line, ','); //get the first element of the line
    int data_count = stoi(line); //get the number of lines after this line
    getline(reader, line); //get the rest of the line and set the feature to analyse
    if(line == "gap"){
        track_index = 2;
    }else if(line == "grp"){
        track_index = 3;
    }else if(line == "v"){
        track_index = 4;
    }else if(line == "gi"){
        track_index = 5;
    }

int print_count = 0; // used to overwrite or append the output file
    for(int i = 0; i < data_count; i++){ //read the commands 
        getline(reader, line);
        if(line == "add"){
            getline(reader, line, ','); //get the date
            //std::cout << line << std::endl;
            if(stats.first_date == ""){
                stats.first_date = line;
            }
            stats.last_date = line;
            getline(reader, line, ','); //get the time
            if(stats.first_time == ""){
                stats.first_time = line;
            }
            stats.last_time = line;
           for(int i = 0; i < track_index - 1; i++){ //skip the elements before the feature to analyse
                getline(reader, line, ',');
            }
           //std::cout <<"Added Value is: " <<line << std::endl;
           double temp = stod(line);
           manager.add_element(temp);
            
        }
        if(line == "print"){
            stats.write(print_count);
            //stats.print();
            print_count++;
        }
    }

    reader.close();
    return;
}


#if PERF_STAT
void print_perf(){
    std::cout << "Mean: " << perf.mean.count << " " << perf.mean.time.count() << std::endl;
    std::cout << "Std: " << perf.std.count << " " << perf.std.time.count() << std::endl;
    std::cout << "Min: " << perf.min.count << " " << perf.min.time.count() << std::endl;
    std::cout << "Firstq: " << perf.firstq.count << " " << perf.firstq.add_time.count() << " " << perf.firstq.calculate_time.count() << std::endl;
    std::cout << "Median: " << perf.median.count << " " << perf.median.add_time.count() << " " << perf.median.calculate_time.count() << std::endl;
    std::cout << "Thirdq: " << perf.thirdq.count << " " << perf.thirdq.add_time.count() << " " << perf.thirdq.calculate_time.count() << std::endl;
    std::cout << "Max: " << perf.max.count << " " << perf.max.time.count() << std::endl;
return;
}
#endif

int main(int argc, char** argv) {
   reader(argv[1]);
    
   //print_perf();
  	return 0;
}

/*
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
*/
