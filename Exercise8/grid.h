//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_GRID_H
#define PARALLELSYSTEMS_GRID_H

#include <iostream>
#include <vector>
#include <cmath>

constexpr auto N = 512;
constexpr auto SIZE = N * N;

class Grid
{
private:
    double * data;
    double * data2;
public:

    unsigned int xpos;
    unsigned int ypos;
    unsigned int from_x;
    unsigned int to_x;
    unsigned int from_y;
    unsigned int to_y;
    unsigned int blocksize;


    void set_from_to(unsigned int from_x, unsigned int to_x, unsigned int from_y, unsigned int to_y)
    {
        this->from_x=from_x;
        this->to_x=to_x;
        this->from_y=from_y;
        this->to_y=to_y;
        this->blocksize=to_x-from_x;
    }

    Grid(double * borders)
    {
        data = new double[ SIZE ];

        for (long i = 0; i < SIZE; i++) {
            data[i] = 0;
        }

        for (long i = 0; i < N; i++) {
            data[i] = borders[0];
            data[(SIZE) - 1 - i] = borders[2];
        }
        for(long i = 1; i < N -1; i++) {
            data[(N-1)+i*N] = borders[1];
            data[(i*N)] = borders[3];
        }

    }

    double get(unsigned int row, unsigned int column){
        return data[ (row * N) + column ];
    }

    double get_five(unsigned int row, unsigned int column){
        return (get(row,column) +
                get(row - 1, column) +
                get(row + 1, column) +
                get(row, column - 1) +
                get(row, column +1 ) ) / 5.0;
    }

    void set(unsigned int row, unsigned int column, double value){
        data[ (row * N) + column ] = value;
    }


    const void print() {
        for (long i = 0; i < N; i++) {
            for(long j = 0; j < N; j++)
                std::cout << data[ (i * N) + j ] << " | ";
            std::cout << std::endl;
        }
    }

    std::vector<std::vector<double> > get_block_borders()
    {
        int size=to_x-from_x;
        std::vector<std::vector<double> > borders = std::vector<std::vector<double> >();
        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));
        for (int i = 0; i < size; ++i) {
            borders[0][i] = get(from_x , from_y + i);
            //      borders[0][size + counter++] = get(from_x + 1, from_y + i);
        }
        for (int i = 0; i < size; ++i) {
            borders[1][i] = get(from_x + i, to_y);
            //     borders[1][size + counter++] = get(from_x + i, to_y - 1);
        }
        for (int i = 0; i < size; ++i) {
            borders[2][i] = get(to_x, from_y + i);
            //    borders[2][size + counter++] = get(to_x - 1, from_y + i);
        }
        for (int i = 0; i < size; ++i) {
            borders[3][i] = get(from_x + i, from_y);
            //   borders[3][size + counter++] = get(from_x + i, from_y + 1);
        }
        return borders;
    }

    void set_block_borders(double * borders,
                     unsigned direction)
    {
        int size=to_x-from_x;

        switch (direction)
        {
            case 0:
                for (int i = 0; i < size; ++i)
                    set(from_x - 1, from_y + i,borders[i]);
                break;
            case 1:
                for (int i = 0; i < size; ++i)
                    set(from_x + i, to_y + 1,borders[i]);
                break;
            case 2:
                for (int i = 0; i < size; ++i)
                    set(to_x + 1, from_y + i,borders[i]);
                break;
            case 3:
                for (int i = 0; i < size; ++i)
                    set(from_x + i, from_y - 1,borders[i]);
                break;
        }
    }

    void set_block(int my_rank, int blocks)
    {
        xpos=my_rank / blocks;
        ypos=my_rank % blocks;
        int elems_per_block = N / blocks;
        this->set_from_to(xpos * elems_per_block, (xpos + 1) * elems_per_block, ypos * elems_per_block, (ypos + 1) * elems_per_block);
    }

};

#endif //PARALLELSYSTEMS_GRID_H
