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

struct node{
    unsigned int value;
    node * left;
    node * right;
    int height;
};

typedef struct node * nodeptr;
class avlTree
{
private:

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
        else if (value<p->value) {
            insert(value, p->left);
        }
        else {
            insert(value, p->right);

        }
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
        std::cout<<"Insert Par"<<std::endl;

        int block = omp_get_num_threads() * 100;
        for(int j=0;j<values.size();j+=block) {
            #pragma omp parallel for private(block) shared(root) schedule(static)
            for (unsigned int i = 0; i < block; i++)
                insert(values[i+j], root);

            balance(root);
        }
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
        std::cout<<"Insert Seq"<<std::endl;
        for(int j=0;j<values.size();j++)
                insert(values[j], root);
    }

#endif

    bool check_order(){
        return ((check_order(root,0)==std::numeric_limits<int>::max())?false:true);
    }

};


#endif //PARALLELSYSTEMS_AVL_H

