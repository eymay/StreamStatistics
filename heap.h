#ifndef HEAP_H
#define HEAP_H

#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>
class minheap{
    std::vector<float> data;
    int heap_size = 0;
public:

    minheap(){
        data.push_back(0);
    }
    int parent(int i){
        return std::floor(i/2);
    }
    int left(int i){
        return 2*i;
    }
    int right(int i){
        return 2*i+1;
    }

    void min_heapify(int index){
        int left = minheap::left(index);
        int right = minheap::right(index);
        int smallest;
        if(left <= minheap::heap_size &&  minheap::data[left] < minheap::data[index]){
            smallest = left;
        }
        else{
            smallest = index;
        }

        if(right <= minheap::heap_size && minheap::data[right] < minheap::data[smallest]){
            smallest = right;
        }
        /*
        std::cout << "smallest: " << smallest << "left: " << left<< "right: " << right<< std::endl;
        std::cout << "left child: " << minheap::data[left] << "right: " << minheap::data[right]<< std::endl;

        std::cout << "smallest child: " << minheap::data[smallest] << std::endl;
        std::cout << "index: " << index << std::endl;
        */
        if(smallest != index){
            std::swap(minheap::data[index], minheap::data[smallest]);
            min_heapify(smallest);
        }
    }

    void build_min_heap(int n){
        minheap::heap_size = n;
        for(int i = std::floor(minheap::heap_size/2); i >= 1; i--){
            min_heapify( i);
        }
        return;
    }
    float extract_min(){
        if(minheap::data.size() < 1){
            std::cout << "heap underflow" << std::endl;
        }
        float min = minheap::data[1];
        minheap::data[1] = minheap::data[minheap::heap_size ];
        heap_size--;
        minheap::data.pop_back();
        //print();
        min_heapify( 1);
        //print();
        return min;
    }
    void insert(float value){
        heap_size++;
        data.push_back(value);
        //build_min_heap();
        //print();
        
        int index = heap_size;
        while(index > 1){
            int parent = minheap::parent(index);
            if(data[parent] > data[index]){
                float temp = data[parent];
                data[parent] = data[index];
                data[index] = temp;
                index = parent;
            }
            else{
                break;
            }
        }
        
        return;
    }
    int get_size(){
       return heap_size;
    }
    float get_min(){
        return data[1];
    }
void print(){
    for(int i = 0; i <= heap_size; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}
bool check_heap(){
    for(int i = 2; i <= heap_size; i++){
        if(data[i] < data[minheap::parent(i)]){
        std::cout << "Child at index "<< i << " " << data[i] <<  std::endl;
            std::cout << "Parent at index "<< parent(i)  << " " << data[parent(i)]<< std::endl;
            return false;
        }
    }
    return true;
    }
};

class maxheap{
    std::vector<float> data;
    int heap_size = 0;
public:

    maxheap(){
        data.push_back(0);
    }
    int parent(int i){
        return std::floor(i/2);
    }
    int left(int i){
        return 2*i;
    }
    int right(int i){
        return 2*i+1;
    }

    void max_heapify(int index){
        int left = maxheap::left(index);
        int right = maxheap::right(index);
        int largest;
        if(left <= maxheap::heap_size &&  maxheap::data[left] > maxheap::data[index]){
            largest = left;
        }
        else{
            largest = index;
        }

        if(right <= maxheap::heap_size && maxheap::data[right] > maxheap::data[largest]){
            largest = right;
        }
        /*
        std::cout << "largest: " << largest << "left: " << left<< "right: " << right<< std::endl;
        std::cout << "left child: " << maxheap::data[left] << "right: " << maxheap::data[right]<< std::endl;

        std::cout << "largest child: " << maxheap::data[largest] << std::endl;
        std::cout << "index: " << index << std::endl;
        */
        if(largest != index){
            std::swap(maxheap::data[index], maxheap::data[largest]);
            max_heapify(largest);
        }
    }

    void build_max_heap(int n){
        maxheap::heap_size = n;
        for(int i = std::floor(maxheap::heap_size/2); i >= 1; i--){
            max_heapify( i);
        }
        return;
    }
    float extract_max(){
        if(maxheap::data.size() < 1){
            std::cout << "heap underflow" << std::endl;
        }
        float max = maxheap::data[1];
        maxheap::data[1] = maxheap::data[maxheap::heap_size ];
        heap_size--;
        maxheap::data.pop_back();
        //print();
        max_heapify( 1);
        //print();
        return max;
    }
    void insert(float value){
        heap_size++;
        data.push_back(value);
        //build_max_heap();
        //print();
        
        int index = heap_size;
        while(index > 1){
            int parent = maxheap::parent(index);
            if(data[parent] < data[index]){
                float temp = data[parent];
                data[parent] = data[index];
                data[index] = temp;
                index = parent;
            }
            else{
                break;
            }
        }
        
        return;
    }
    int get_size(){
       return heap_size;
    }
    float get_max(){
        return data[1];
    }
void print(){
    for(int i = 0; i <= heap_size; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}
    int size(){
        return data.size();
    }
bool check_heap(){
    for(int i = 2; i <= heap_size; i++){
        if(data[i] > data[maxheap::parent(i)]){
        std::cout << "Child at index "<< i << " " << data[i] <<  std::endl;
            std::cout << "Parent at index "<< parent(i)  << " " << data[parent(i)]<< std::endl;
            return false;
        }
    }
    return true;
    }
};
class minmax_heap{
 std::vector<float> data;
    int heap_size = 0;
public:

    minmax_heap(){
        data.push_back(0);
    }
    int parent(int i){
        return std::floor(i/2);
    }
    int grand_parent(int i){
        return std::floor(i/4);
    }
    int left(int i){
        return 2*i;
    }
    int right(int i){
        return 2*i+1;
    }

    void minmax_heapify(int index){
        int left = minmax_heap::left(index);
        int right = minmax_heap::right(index);
        int largest;
        if(left <= minmax_heap::heap_size &&  minmax_heap::data[left] > minmax_heap::data[index]){
            largest = left;
        }
        else{
            largest = index;
        }

        if(right <= minmax_heap::heap_size && minmax_heap::data[right] > minmax_heap::data[largest]){
            largest = right;
        }
        std::cout << "largest: " << largest << "left: " << left<< "right: " << right<< std::endl;
        std::cout << "left child: " << minmax_heap::data[left] << "right: " << minmax_heap::data[right]<< std::endl;

        std::cout << "largest child: " << minmax_heap::data[largest] << std::endl;
        std::cout << "index: " << index << std::endl;

        if(largest != index){
            std::swap(minmax_heap::data[index], minmax_heap::data[largest]);
            minmax_heapify(largest);
        }
    }
    void pushdown_min(int index){
        int min_child;
        int child_foundinfo = 0;
        if(left(index) <= minmax_heap::heap_size){
            min_child = left(index);
            child_foundinfo = 1;
        }
        if(right(index) <= minmax_heap::heap_size &&
        minmax_heap::data[right(index)] < minmax_heap::data[min_child]){
                min_child = right(index);
                child_foundinfo = 1;
        }
        int left_gchild = left(left(index));
        for(int i = 0; i < 4 && left_gchild + i < minmax_heap::heap_size; i++){
            if(minmax_heap::data[left_gchild + i] < minmax_heap::data[min_child]){
                min_child = left_gchild + i;
                child_foundinfo = 2;
            }
        }
        if(child_foundinfo == 2){
            std::swap(minmax_heap::data[index], minmax_heap::data[min_child]);
            if(minmax_heap::data[min_child] > minmax_heap::data[parent(min_child)]){
                std::swap(minmax_heap::data[min_child], minmax_heap::data[parent(min_child)]);
            }
            pushdown_min(min_child);
        }
        else if(child_foundinfo == 1){
            std::swap(minmax_heap::data[index], minmax_heap::data[min_child]);
        }

    }
    void pushdown(int index){
        int level = std::floor(std::log2(index));
        if (level % 2 == 0){
            pushdown_min( index);
        }else{
           // pushdown_max(index);
        }
    }
    void build_minmax_heap(int n){
        minmax_heap::heap_size = n;
        for(int i = std::floor(minmax_heap::heap_size/2); i >= 1; i--){
            pushdown( i);
        }
        return;
    }
void print(){
    for(int i = 0; i <= heap_size; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}
    int size(){
        return data.size();
    }
};

#endif
