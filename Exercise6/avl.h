//
// Created by ivan on 16/11/17.
//

#ifndef PARALLELSYSTEMS_AVL_H
#define PARALLELSYSTEMS_AVL_H

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <math.h>
#include <bitset>
#include <functional>
#include <algorithm>

struct node{
    unsigned int value;
    node * left;
    node * right;
    int height;
};

typedef struct node * nodeptr;
class avlTree
{
    nodeptr root;

    int get_height(nodeptr & p){
        if(p!=NULL) return p->height;
        else return 0;
    }


    nodeptr simple_right_rotation(nodeptr &p1)
    {
        nodeptr p2 = p1->left;
        p1->left=p2->right;
        p2->right=p1;
        p1->height=std::max(get_height(p1->left),get_height(p1->right))+1;
        p2->height=std::max(get_height(p2->left),p1->height)+1;
        return p2;
    }
    nodeptr simple_left_rotation(nodeptr &p1)
    {
        nodeptr p2 = p1->right;
        p1->right=p2->left;
        p2->left=p1;
        p1->height=std::max(get_height(p1->left),get_height(p1->right))+1;
        p2->height=std::max(get_height(p2->right),p1->height)+1;
        return p2;
    }
    nodeptr double_right_rotation(nodeptr &p1){
        p1->left = simple_left_rotation(p1->left);
        return simple_right_rotation(p1);
    }
    nodeptr double_left_rotation(nodeptr &p1){
        p1->right = simple_right_rotation(p1->right);
        return simple_left_rotation(p1);
    }


    nodeptr find(nodeptr &p, unsigned int value){
        if(p->value==value) return p;
        else if(value<p->value) return find(p->left,value);
        else return find(p->right,value);
    }

#if defined(PARALLEL)
    void insert(unsigned int value, nodeptr & p){
        if( p==NULL ){
            p = new node;
            p->value = value;
            p->left = NULL;
            p->right = NULL;
            p->height = 0;
            return;
        }
        if( value == p->value )
            return;
        else if (value<p->value)
            insert(value, p->left);
        else
            insert(value, p->right);
    }
#elif defined(PARALLEL_STABLE)

    void insert(std::vector<unsigned int> values, nodeptr & p, std::function<bool(unsigned int)> fun)
    {
        for (const auto value : values)
            if (fun(value))
                insert(value, p);
    }
    
    bool insert(unsigned int value, nodeptr & p){
        if( p==NULL ){
            p = new node;
            p->value = value;
            p->left = NULL;
            p->right = NULL;
            p->height = 1;
            return true;
        }
        if( value == p->value )
            return false;
        else if (value<p->value) 
        {
            insert(value, p->left);

            if(get_height(p->left)-get_height(p->right) == 2) {
                if(value < p->left->value)  p=simple_right_rotation(p);
                else                        p=double_right_rotation(p);
            }

            p->height=std::max(get_height(p->left),get_height(p->right))+1;
        }
        else 
        {
            insert(value, p->right);

            if ((get_height(p->right) - get_height(p->left))==2) {
                if(value > p->right->value) p=simple_left_rotation(p);
                else                        p=double_left_rotation(p);
            }

            p->height=std::max(get_height(p->left),get_height(p->right))+1;
        }
        return true;
    }


#else
    void insert(unsigned int value, nodeptr & p){
        if( p==NULL ){
            p = new node;
            p->value = value;
            p->left = NULL;
            p->right = NULL;
            p->height = 0;
            return;
        }
        if( value == p->value )
            return;
        else if (value<p->value) {
            insert(value, p->left);
            if(get_height(p->left)-get_height(p->right) == 2) {
                if(value < p->left->value)  p=simple_right_rotation(p);
                else                        p=double_right_rotation(p);
            }
        }
        else {
            insert(value, p->right);
            if ((get_height(p->right) - get_height(p->left))==2) {
                if(value > p->right->value) p=simple_left_rotation(p);
                else                        p=double_left_rotation(p);
            }
        }
    }

#endif
    void balance(nodeptr & p)
    {

        if(p->left!=NULL)
            balance(p->left);
        if(p->right!=NULL)
            balance(p->right);
        if(p->left==NULL && p->right==NULL) {
            p->height=0;
            return;
        }

        if( ( get_height(p->left) - get_height(p->right) ) == 2 ){
            if( p->left->right == NULL || get_height(p->left->left) > get_height(p->left->right) )
                p = simple_right_rotation(p);
            else
                p = double_right_rotation(p);
        }


        if( ( get_height(p->right) - get_height(p->left) )  == 2 ) {
            if( p->right->left == NULL || get_height(p->right->right) > get_height(p->right->left) )
                p = simple_left_rotation(p);
            else
                p = double_left_rotation(p);
        }

        p->height=std::max(get_height(p->left),get_height(p->right))+1;
    }

    int check_order(nodeptr node, unsigned int prev){
        if(node!=NULL)
        {
            prev=check_order(node->left,prev);
            if(prev>node->value){
                return std::numeric_limits<int>::max();
            }
            return check_order(node->right,node->value);
        } else return prev;
    }

public:

    avlTree() : root(NULL){}

    ~avlTree() { delete root; }

#if defined(PARALLEL)

    void insert(std::vector<unsigned int> values) {
        if(root == NULL) {
            root = new node;
            root->value = values[0];
            root->right = NULL;
            root->left = NULL;
            root->height = 0;
            values.erase( values.begin() );
        }

        const auto block = omp_get_num_threads() * 100;
        for(auto j = 0u;j<values.size();j+=block) {
        // Only works because statistically we have every value 8 times in our
        // vector because vector of size(n) with values from 0 to n/8

            #pragma omp parallel for private(root) schedule(static)
            for (auto i = 0; i < block; i++)
                insert(values[i+j], root);

            balance(root);
        }
    }

#elif defined(PARALLEL_STABLE)


/*
 * first determine how many cores to use, max(x , 2^x < omp_get_max_threads)
 * create tree with deepness x
 *
 * parrall insert the whole set into the 2^x subnodes, skip values that are to large or to small
 *
 * example:
 * 5 cores => use 4 of them, x=2
 * tree:
 *              9
 *      4               15
 *  2       6       11      19
 *
 *
 *  thread 1 works on node 2 and use only vars lower 4
 *  thread 2 works on node 6 and use only vars greater 4 smaller 9
 *  thread 3 works on node 11 and use only vars greater 9 smaller 15
 *  thread 4 works on node 19 and use only vars greater 15
 */

    void insert(std::vector<unsigned int> values) {
        if(root == NULL) {
            root = new node;
            root->value = values[0];
            root->right = NULL;
            root->left = NULL;
            root->height = 0;
            values.erase( values.begin() );
        }

        //determine cores to use
        const auto num_threads = omp_get_max_threads();
        const auto parallel_deepness = static_cast<unsigned>(std::log2(num_threads));
        const auto num_parallel_trees = static_cast<unsigned>(std::pow(2,parallel_deepness));


        //root nodes of the sub trees
        nodeptr parall_tree_roots[num_parallel_trees];

        for(unsigned int i=0;i<num_parallel_trees*4;i++) {
            if(insert(values[0], root)==false) i--;
            values.erase( values.begin() );
        }
        for(unsigned int i=0,j=0;i<num_parallel_trees;i+=2,j++) {
            nodeptr temp=root;
            std::bitset<128> bin(i);
            for(auto j=1u;j<parallel_deepness; j++)
                if(bin[parallel_deepness-j]==0)
                    temp = temp->left;
                else
                    temp = temp->right;
            parall_tree_roots[i]=temp->left;
            parall_tree_roots[i+1]=temp->right;
        }
#pragma omp parallel for schedule(static,1)
                for(unsigned int i=0;i<num_parallel_trees;i++)
                    insert(values,
                           // root node of sub tree
                           parall_tree_roots[i],
                           //lamba check if var in range
                           [&](unsigned int value){
                               nodeptr temp=root;
                               for(auto j=0u;j<parallel_deepness; j++)
                                   if(value<temp->value)
                                       temp = temp->left;
                                   else
                                       temp = temp->right;
                               if(temp->value==parall_tree_roots[i]->value)
                                   return true;
                               else
                                   return false;
                           });

        //final balanceing
                balance(root);
    }

#else

    void insert(std::vector<unsigned int> values) {
        if(root == NULL) {
            root = new node;
            root->value = values[0];
            root->right = NULL;
            root->left = NULL;
            root->height = 0;
            values.erase( values.begin() );
        }
        for(auto j=0u;j<values.size();j++)
                insert(values[j], root);
    }

#endif

    bool check_order(){
        return ((check_order(root,0)==std::numeric_limits<int>::max())?false:true);
    }

};


#endif //PARALLELSYSTEMS_AVL_H

