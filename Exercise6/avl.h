//
// Created by ivan on 16/11/17.
//

#ifndef PARALLELSYSTEMS_AVL_H
#define PARALLELSYSTEMS_AVL_H

#include <iostream>
#include <vector>
#include <string>
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
    int num_elems;

    int get_height(nodeptr & p){
        if(p!=NULL) return p->height;
        else return 0;
    }

    nodeptr srl(nodeptr &p1)
    {
        nodeptr p2 = p1->left;
        p1->left=p2->right;
        p2->right=p1;
        p1->height=std::max(get_height(p1->left),get_height(p1->right))+1;
        p2->height=std::max(get_height(p2->left),p1->height)+1;
        return p2;
    }
    nodeptr srr(nodeptr &p1)
    {
        nodeptr p2 = p1->right;
        p1->right=p2->left;
        p2->left=p1;
        p1->height=std::max(get_height(p1->left),get_height(p1->right))+1;
        p2->height=std::max(get_height(p2->right),p1->height)+1;
        return p2;
    }
    nodeptr drl(nodeptr &p1){
        p1->left = srr(p1->left);
        return srl(p1);
    }
    nodeptr drr(nodeptr &p1){
        p1->right = srl(p1->right);
        return srr(p1);
    }


    void insert(unsigned int value, nodeptr & p){
        if( p==NULL ){
            p = new node;
            p->value = value;
            p->left = NULL;
            p->right = NULL;
            p->height = 0;
            num_elems++;
            return;
        }
        if( value == p->value ) return;
        else if (value<p->value){
            insert(value, p->left);
            if( ( get_height(p->left) - get_height(p->right) ) == 2 ){
                if( value < p->left->value )    p = srl(p);
                else                            p = drl(p);
            }
        } else {
            insert(value, p->right);
            if( ( get_height(p->right) - get_height(p->left) )  == 2 ) {
                if( value > p->right->value )   p = srr(p);
                else                            p = drr(p);
            }
        }
        p->height = std::max( get_height(p->left), get_height(p->right)) + 1;
    }

    int get_num_elems(){
        return num_elems;
    }

    std::string print(nodeptr node, bool offset){
        std::string result="";
        if(offset)
            for(unsigned int i=0;i<std::pow(2,node->height);i++)
                result+=" ";
        for(unsigned int i=0;i<std::pow(2,node->height-1);i++)
            result+=" ";
        result+=node->value;
        return result;
    }

public:

    avlTree() : root(NULL),num_elems(0){}

    ~avlTree() { delete root; }

    void insert(std::vector<unsigned int> values) {
        if(root == NULL) {
            root = new node;
            root->value = values[0];
            root->right = NULL;
            root->left = NULL;
            root->height = 0;
            values.erase( values.begin() );
            num_elems++;
        }
        for(auto value : values) {
            insert(value,root);
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, const avlTree& o){
     /*   stream<<"Height: " <<o.root->height<<std::endl;
        stream<<"Elements: "<<o.get_num_elems()<<std::endl;
        stream<<o.print(o.root,false)<<endl;
        stream<<o.print(o.root->left,false);
        stream<<o.print(o.root->right,true);
       */ return stream;
    }
};


#endif //PARALLELSYSTEMS_AVL_H

