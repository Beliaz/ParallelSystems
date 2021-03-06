//
// Created by ivan on 03/11/17.
//

#ifndef PARALLELSYSTEMS_MERGE_SORT_H
#define PARALLELSYSTEMS_MERGE_SORT_H

#include <vector>
#include <random>
#include <iostream>
#include <omp.h>
#include <string>


std::vector<double> init(unsigned n) {
    std::vector<double> retval = std::vector<double>(n);
    static std::default_random_engine rng;
    static std::uniform_real_distribution<> rnd(0,100);
    for(unsigned int i=0;i<n;i++)
        retval[i]=rnd(rng);
    return retval;
}


void merge(double * begin_left,
           double * middle,
           double * end_right,
           double * output) {

    for(unsigned int i=0,j=0;i<output->size();i++)
        if(*(begin_left+i)<*(middle+j)){

        }

}

void insertion_sort (double * begin, double * end){
    int j;
    double temp;
    int size=(end-begin);
    for (unsigned int i = 1; i < size; i++){
        j = i;

        while (j > 0 && *(begin+j) < *(begin+j-1)){
            temp = *(begin+j);
            *(begin+j) = *(begin+j-1);
            *(begin+j-1) = temp;
            j--;
        }
    }
}

#if defined(PAR_OPT)


void par_sort(double * begin, double * end, double * output) {
    int size=(end-begin);
    double * middle=begin + (size/2);
    if(size>32) {

#pragma omp task untied mergeable
        par_sort(begin,middle,output);
#pragma omp task untied mergeable
        par_sort(middle,end,output+(size/2));
#pragma omp taskwait
        merge(begin,middle,end,output);
    } else if (size>1)
        insertion_sort(begin,end);
    else return;
}
void sort(std::vector<double> * _arr) {
    double * output = _arr->data();
#pragma omp parallel
    {
#pragma omp single
        par_sort(array,array+_arr->size(),_arr);
    }
}

#else

void seq_sort(double * begin, double * end) {
    int size=(end-begin);
    double * middle=begin + (size/2);
    if(size>17) {
        seq_sort(begin, middle);
        seq_sort(middle, end);
        merge(begin,middle,end);
    }
    else if (size>1) return insertion_sort(begin,end);
    else return;
}

void sort(std::vector<double> * _arr)
{
    double * array = _arr->data();
    seq_sort(array,array+_arr->size());
}


#endif


bool is_sorted(const std::vector<double> _arr)
{
    for(unsigned int i=1;i<_arr.size();i++)
        if(_arr[i]<_arr[i-1]) {
            std::cout<<_arr[i]<<" < "<<_arr[i-1]<<std::endl;
            return false;
        }
    return true;
}

std::ostream & operator<<(std::ostream & Str, std::vector<double> const & _arr) {
    Str << "Array: " <<_arr.size()<<std::endl;
    Str << "{ ";
    for(auto value : _arr)
        Str << value << " , ";
    Str <<" }";
    return Str;
}

#endif //PARALLELSYSTEMS_MERGE_SORT_H
