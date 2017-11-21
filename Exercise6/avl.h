//
// Created by ivan on 16/11/17.
//

#ifndef PARALLELSYSTEMS_AVL_H
#define PARALLELSYSTEMS_AVL_H

#include <vector>
#include <string>
#include <bitset>
#include <functional>
#include <algorithm>
#include <omp.h>
#include <cmath>

using nodeptr = struct node*;

struct node
{
    unsigned int value;
    node* left;
    node* right;
    int height;
};

class avl_tree
{
    nodeptr root_;

public:
    explicit avl_tree(const nodeptr root)
        : root_(root)
    {
    }

private:
    static int get_height(nodeptr& p)
    {
        if (p != nullptr)
            return p->height;

        return 0;
    }

    nodeptr simple_right_rotation(nodeptr& p1) const
    {
        auto p2 = p1->left;

        p1->left = p2->right;
        p2->right = p1;

        p1->height = std::max(get_height(p1->left), get_height(p1->right)) + 1;
        p2->height = std::max(get_height(p2->left), p1->height) + 1;

        return p2;
    }

    nodeptr simple_left_rotation(nodeptr& p1) const
    {
        auto p2 = p1->right;

        p1->right = p2->left;
        p2->left = p1;

        p1->height = std::max(get_height(p1->left), get_height(p1->right)) + 1;
        p2->height = std::max(get_height(p2->right), p1->height) + 1;

        return p2;
    }

    nodeptr double_right_rotation(nodeptr& p1) const
    {
        p1->left = simple_left_rotation(p1->left);
        return simple_right_rotation(p1);
    }

    nodeptr double_left_rotation(nodeptr& p1) const
    {
        p1->right = simple_right_rotation(p1->right);
        return simple_left_rotation(p1);
    }

    static nodeptr find(nodeptr& p, const unsigned int value)
    {
        if (p->value == value)
            return p;

         return value < p->value
            ? find(p->left, value)
            : find(p->right, value);
    }

    bool insert(const unsigned int value, nodeptr& p) const;

    void balance(nodeptr& p) const
    {
        if (p->left != nullptr)
            balance(p->left);
        if (p->right != nullptr)
            balance(p->right);
        if (p->left == nullptr && p->right == nullptr)
        {
            p->height = 0;
            return;
        }

        if (get_height(p->left) - get_height(p->right) == 2)
        {
            if (p->left->right == nullptr || get_height(p->left->left) > get_height(p->left->right))
                p = simple_right_rotation(p);
            else
                p = double_right_rotation(p);
        }

        if (get_height(p->right) - get_height(p->left) == 2)
        {
            if (p->right->left == nullptr || get_height(p->right->right) > get_height(p->right->left))
                p = simple_left_rotation(p);
            else
                p = double_left_rotation(p);
        }

        p->height = std::max(get_height(p->left), get_height(p->right)) + 1;
    }

    int check_order(const nodeptr node, unsigned int prev) const
    {
        if (node != nullptr)
        {
            prev = check_order(node->left, prev);

            if (prev > node->value)
            {
                return std::numeric_limits<int>::max();
            }

            return check_order(node->right, node->value);
        }
        return prev;
    }

public:

    avl_tree() : root_(nullptr)
    {
    }

    ~avl_tree()
    {
        delete root_;
    }

    void insert(std::vector<unsigned int> values);
    void insert(std::vector<unsigned int> values, nodeptr & p, const std::function<bool(unsigned int)> fun) const;

    bool check_order() const
    {
        return check_order(root_, 0) == std::numeric_limits<int>::max() ? false : true;
    }
};

#if defined(PARALLEL)

inline void avl_tree::insert(const unsigned int value, nodeptr & p) const 
{
    if (p == nullptr)
    {
        p = new node;
        p->value = value;
        p->left = nullptr;
        p->right = nullptr;
        p->height = 0;
        return;
    }

    if (value<p->value)
        insert(value, p->left);
    else
        insert(value, p->right);
}

inline void avl_tree::insert(std::vector<unsigned int> values)
{
    if (root_ == nullptr) 
    {
        root_ = new node;
        root_->value = values[0];
        root_->right = nullptr;
        root_->left = nullptr;
        root_->height = 0;
        values.erase(values.begin());
    }

    const auto block = omp_get_num_threads() * 100;
    for (auto j = 0u; j < values.size(); j += block) 
    {
        // Only works because statistically we have every value 8 times in our
        // vector because vector of size(n) with values from 0 to n/8

        #pragma omp parallel for private(root) schedule(static)
        for (auto i = 0; i < block; i++)
            insert(values[i + j], root_);

        balance(root_);
    }
}

#elif defined(PARALLEL_STABLE)

inline void avl_tree::insert(std::vector<unsigned int> values, nodeptr & p, const std::function<bool(unsigned int)> fun) const
{
    for (const auto value : values)
        if (fun(value))
            insert(value, p);
}

inline bool avl_tree::insert(const unsigned int value, nodeptr & p) const
{
    if (p == nullptr)
    {
        p = new node;

        p->value = value;
        p->left = nullptr;
        p->right = nullptr;
        p->height = 1;

        return true;
    }

    if (value == p->value)
        return false;

    if (value<p->value)
    {
        insert(value, p->left);

        if (get_height(p->left) - get_height(p->right) == 2) {
            if (value < p->left->value)  p = simple_right_rotation(p);
            else                        p = double_right_rotation(p);
        }

        p->height = std::max(get_height(p->left), get_height(p->right)) + 1;
    }
    else
    {
        insert(value, p->right);

        if ((get_height(p->right) - get_height(p->left)) == 2) {
            if (value > p->right->value) p = simple_left_rotation(p);
            else                        p = double_left_rotation(p);
        }

        p->height = std::max(get_height(p->left), get_height(p->right)) + 1;
    }

    return true;
}

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

inline void avl_tree::insert(std::vector<unsigned int> values) 
{
    if (root_ == nullptr)
    {
        root_ = new node;
        root_->value = values[0];
        root_->right = nullptr;
        root_->left = nullptr;
        root_->height = 0;
        values.erase(values.begin());
    }

    //determine cores to use
    const auto num_threads = omp_get_max_threads();
    const auto parallel_deepness = static_cast<unsigned>(std::log2(num_threads));
    const auto num_parallel_trees = static_cast<unsigned>(std::pow(2, parallel_deepness));
    
    //root nodes of the sub trees
    nodeptr parall_tree_roots[num_parallel_trees];

    for (unsigned int i = 0; i<num_parallel_trees * 4; i++) 
    {
        if (!insert(values[0], root_))
            i--;

        values.erase(values.begin());
    }

    for (unsigned int i = 0, j = 0; i<num_parallel_trees; i += 2, j++) 
    {
        auto temp = root_;

        std::bitset<128> bin(i);

        for (auto k = 1u; k < parallel_deepness; k++)
        {
            temp = bin[parallel_deepness - k] == 0 
                ? temp->left
                : temp->right;
        }

        parall_tree_roots[i] = temp->left;
        parall_tree_roots[i + 1] = temp->right;
    }

    #pragma omp parallel for schedule(static,1)
    for (unsigned int i = 0; i < num_parallel_trees; i++)
    {
        insert(
            values,  
            // root node of sub tree
            parall_tree_roots[i],
            //lamba check if var in range
            [&](unsigned int value)
        {
            auto temp = root_;
            for (auto j = 0u; j < parallel_deepness; j++)
                if (value < temp->value)
                    temp = temp->left;
                else
                    temp = temp->right;
            if (temp->value == parall_tree_roots[i]->value)
                return true;
            else
                return false;
        });
    }

    //final balanceing
    balance(root_);
}

#else

inline bool avl_tree::insert(const unsigned value, nodeptr& p) const
{
    if (p == nullptr)
    {
        p = new node;
        p->value = value;
        p->left = nullptr;
        p->right = nullptr;
        p->height = 0;

        return true;
    }

    if (value == p->value)
        return false;

    if (value < p->value)
    {
        insert(value, p->left);
        if (get_height(p->left) - get_height(p->right) == 2)
        {
            if (value < p->left->value) p = simple_right_rotation(p);
            else p = double_right_rotation(p);
        }
    }
    else
    {
        insert(value, p->right);
        if (get_height(p->right) - get_height(p->left) == 2)
        {
            if (value > p->right->value) p = simple_left_rotation(p);
            else p = double_left_rotation(p);
        }
    }

    return true;
}

inline void avl_tree::insert(std::vector<unsigned int> values)
{
    if (root_ == nullptr)
    {
        root_ = new node;
        root_->value = values[0];
        root_->right = nullptr;
        root_->left = nullptr;
        root_->height = 0;
        values.erase(values.begin());
    }
    for (auto j = 0u; j < values.size(); j++)
        insert(values[j], root_);
}


#endif

#endif //PARALLELSYSTEMS_AVL_H
