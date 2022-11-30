#ifndef HEAP_H
#define HEAP_H

#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>

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
        std::cout << "largest: " << largest << "left: " << left<< "right: " << right<< std::endl;
        std::cout << "left child: " << maxheap::data[left] << "right: " << maxheap::data[right]<< std::endl;

        std::cout << "largest child: " << maxheap::data[largest] << std::endl;
        std::cout << "index: " << index << std::endl;

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
        print();
        max_heapify( 1);
        print();
        return max;
    }
    void insert(float value){
        heap_size++;
        data.push_back(value);
        //build_max_heap();
        print();
        
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

#endif
