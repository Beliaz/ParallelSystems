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

std::vector<double> merge(std::vector<double> left, std::vector<double> right)
{
    std::vector<double> result = std::vector<double>(left.size()+right.size());
    for(unsigned int i=0;i<result.size();i++)
        if(left[0]>right[0] && !right.empty()) {
            result[i]=right[0];
            right.erase(right.begin());
        } else if(!left.empty()){
            result[i]=left[0];
            left.erase(left.begin());
        } else {
            result[i]=right[0];
            right.erase(right.begin());
        }
    return result;
}

std::vector<double> insertion_sort (const std::vector<double> _arr){
    int j, temp;
    std::vector<double> retval = _arr;

    for (unsigned int i = 0; i < retval.size(); i++){
        j = i;

        while (j > 0 && retval[j] < retval[j-1]){
            temp = static_cast<int>(retval[j]);
            retval[j] = retval[j-1];
            retval[j-1] = temp;
            j--;
        }
    }
    return retval;
}

#if defined(PAR_OPT)
bool split=true;
std::vector<double> sort_rec(const std::vector<double> _arr) {
    if(_arr.size()>7) {
        if(split) {
            return merge(sort_rec(std::vector<double>(_arr.begin(), _arr.begin() + (_arr.size() / 2))),
                         sort_rec(std::vector<double>(_arr.begin() + (_arr.size() / 2), _arr.end())));
        } else {
            split=false;
            std::vector<double> result1, result2;
#pragma omp parallel sections num_threads(2)
            {

#pragma omp section
                {
                    result1 = sort_rec(std::vector<double>(_arr.begin(), _arr.begin() + (_arr.size() / 2)));
                }
#pragma omp section
                {
                    result2 = sort_rec(std::vector<double>(_arr.begin() + (_arr.size() / 2), _arr.end()));
                }
            }
            return merge(result1, result2);
        }
    } else if (_arr.size()>1)
        return insertion_sort(_arr);
    else return _arr;
}

std::vector<double> sort(const std::vector<double> _arr) {

    omp_set_nested(1);
    return sort_rec(_arr);
}


#elif defined(SEQ_OPT)

std::vector<double> sort(const std::vector<double> _arr)
{
    if(_arr.size()>7) return merge(sort(std::vector<double>(_arr.begin(), _arr.begin() + (_arr.size()/2))),
                                   sort(std::vector<double>(_arr.begin() + (_arr.size()/2), _arr.end())));
    else if (_arr.size()>1) return insertion_sort(_arr);
    else return _arr;
}

#else

std::vector<double> sort(const std::vector<double> _arr)
{
    if(_arr.size()>1) return merge(sort(std::vector<double>(_arr.begin(), _arr.begin() + (_arr.size()/2))),
                                   sort(std::vector<double>(_arr.begin() + (_arr.size()/2), _arr.end())));
    else return _arr;
}

#endif


bool is_sorted(const std::vector<double> _arr)
{
    for(unsigned int i=1;i<_arr.size();i++)
        if(_arr[i]<_arr[i-1])
            return false;
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
