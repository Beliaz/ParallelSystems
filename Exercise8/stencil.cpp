//
// Created by ivan on 04/12/17.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <mpi.h>
#include <cmath>

constexpr auto N = 512;
constexpr auto SIZE = N * N;
constexpr auto EPSILON = 20;
static int num_procs;
class Grid
{
private:
    double * data;

public:

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

    std::vector<std::vector<double> > get_borders(unsigned int from_x,
                        unsigned int from_y,
                        unsigned int to_x,
                        unsigned int to_y)
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
    void set_borders(double * borders,
                                                  unsigned direction,
                                                  unsigned int from_x,
                                                  unsigned int from_y,
                                                  unsigned int to_x,
                                                  unsigned int to_y)
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
};


//Does the job and returns the delta Epsilon accumulated over all comparisons.
double iteration(Grid *source, Grid *target,
                 const unsigned int from_x,
                 const unsigned int from_y,
                 const unsigned int to_x,
                 const unsigned int to_y) {
    double d_epsilon = 0;
    for (long i = from_x; i < to_x; i++) {
        for (long j = from_y; j < to_y; j++) {

            const auto current = source->get( i , j );
            const auto new_value = source->get_five( i , j );
            target->set( i , j , new_value );
            d_epsilon += current - new_value > 0 ? current - new_value : new_value - current;

        }
    }
    return d_epsilon;
}

int main(int argc, char **argv) {
    int my_rank, blocks, from_x, from_y, to_x, to_y;
    double *borders = new double[4];
    Grid *grid1,*grid2;


    borders[0] = 1.0;
    borders[1] = 0.5;
    borders[2] = 0;
    borders[3] = -0.5;

    grid1=new Grid(borders);
    grid2=new Grid(borders);


    unsigned iterations = 0;


    //Start to measure time
    using clock = std::chrono::high_resolution_clock;

    const auto start = clock::now();

    // MPI initializations
    //Begin Parallel Block
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    blocks = sqrt(num_procs);

    if( (SIZE % num_procs) != 0 && (blocks != (sqrt(num_procs)*sqrt(num_procs))) ) {
        if (my_rank==0)
            std::cerr<<"Matrix size not divisible by number of processors: "<<num_procs<<std::endl;
        MPI_Finalize();
        exit(-1);
    }

    const auto xpos=my_rank / blocks;
    const auto ypos=my_rank % blocks;


    const auto left_proc = (xpos * blocks) + ypos - 1;
    const auto right_proc = (xpos * blocks) + ypos + 1;
    const auto top_proc = ( (xpos - 1) * blocks) + ypos;
    const auto bottom_proc = ( (xpos + 1) * blocks) + ypos;

    std::cout<<"my rank: "<<my_rank<<", top: "<<top_proc
             <<", left: "<<left_proc<<", bottom: "<<bottom_proc<<", right: "<<right_proc<<std::endl;

    const auto elems_per_block = N / blocks;

    from_x = xpos * elems_per_block;
    from_y = ypos * elems_per_block;
    to_x = (xpos + 1) * elems_per_block;
    to_y = (ypos + 1) * elems_per_block;

    if(xpos==0)
        from_x++;
    if(ypos==0)
        from_y++;
    if(xpos==blocks-1)
        to_x--;
    if(ypos==blocks-1)
        to_y--;

    // Always do two iterations, as the arrays have to switch every time.
    // This way, it is not needed to keep track which was the last
    while (true) {

        iteration(grid1,grid2,from_x,from_y,to_x,to_y);

        std::vector<std::vector<double> > send = grid2->get_borders(from_x,from_y,to_x,to_y);
        double * recv=new double[to_x-from_x];


        if(xpos != 0) {
            std::cout<<my_rank<<" sending top border to "<<top_proc<<std::endl;
            MPI_Send(send[0].data(), elems_per_block, MPI_DOUBLE, top_proc, 0,
                     MPI_COMM_WORLD);

        }if(xpos != blocks-1)
        {
            std::cout<<my_rank<<" recieving bottom border from "<<bottom_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,bottom_proc,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid2->set_borders(recv,2,from_x,from_y,to_x,to_y);
            std::cout<<my_rank<<" sending botttom border to "<<bottom_proc<<std::endl;
            MPI_Send(send[2].data(),elems_per_block,MPI_DOUBLE,bottom_proc,2,MPI_COMM_WORLD);
        }
        if(xpos != 0)
        {
            std::cout<<my_rank<<" recieving top border from "<<top_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,top_proc,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid2->set_borders(recv,0,from_x,from_y,to_x,to_y);
        }
        if(ypos != 0) {
            std::cout<<my_rank<<" sending left border to "<<left_proc<<std::endl;

            MPI_Send(send[3].data(), elems_per_block, MPI_DOUBLE, left_proc, 3,
                     MPI_COMM_WORLD);
        }
        if(ypos != blocks-1)
        {
            std::cout<<my_rank<<" recieving right border from "<<right_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,right_proc,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid2->set_borders(recv,1,from_x,from_y,to_x,to_y);
            std::cout<<my_rank<<" sending right border to "<<right_proc<<std::endl;
            MPI_Send(send[1].data(),elems_per_block,MPI_DOUBLE,right_proc,1,MPI_COMM_WORLD);
        }
        if(ypos != 0) {
            std::cout<<my_rank<<" recieving left border from "<<left_proc<<std::endl;

            MPI_Recv(recv, elems_per_block, MPI_DOUBLE, left_proc, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            grid2->set_borders(recv,3,from_x,from_y,to_x,to_y);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double d_epsilon = iteration(grid2,grid1,from_x,from_y,to_x,to_y);
        send = grid1->get_borders(from_x,from_y,to_x,to_y);

        if(xpos != 0) {
            std::cout<<my_rank<<" sending top border to "<<top_proc<<std::endl;
            MPI_Send(send[0].data(), elems_per_block, MPI_DOUBLE, top_proc, 0,
                     MPI_COMM_WORLD);

        }if(xpos != blocks-1)
        {
            std::cout<<my_rank<<" recieving bottom border from "<<bottom_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,bottom_proc,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid1->set_borders(recv,2,from_x,from_y,to_x,to_y);
            std::cout<<my_rank<<" sending botttom border to "<<bottom_proc<<std::endl;
            MPI_Send(send[2].data(),elems_per_block,MPI_DOUBLE,bottom_proc,2,MPI_COMM_WORLD);
        }
        if(xpos != 0)
        {
            std::cout<<my_rank<<" recieving top border from "<<top_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,top_proc,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid1->set_borders(recv,0,from_x,from_y,to_x,to_y);
        }
        if(ypos != 0) {
            std::cout<<my_rank<<" sending left border to "<<left_proc<<std::endl;

            MPI_Send(send[3].data(), elems_per_block, MPI_DOUBLE, left_proc, 3,
                     MPI_COMM_WORLD);
        }
        if(ypos != blocks-1)
        {
            std::cout<<my_rank<<" recieving right border from "<<right_proc<<std::endl;

            MPI_Recv(recv,elems_per_block,MPI_DOUBLE,right_proc,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            grid1->set_borders(recv,1,from_x,from_y,to_x,to_y);
            std::cout<<my_rank<<" sending right border to "<<right_proc<<std::endl;
            MPI_Send(send[1].data(),elems_per_block,MPI_DOUBLE,right_proc,1,MPI_COMM_WORLD);
        }
        if(ypos != 0) {
            std::cout<<my_rank<<" recieving left border from "<<left_proc<<std::endl;

            MPI_Recv(recv, elems_per_block, MPI_DOUBLE, left_proc, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            grid1->set_borders(recv,3,from_x,from_y,to_x,to_y);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        iterations += 2;

        if (d_epsilon < EPSILON) {
            break;
        }
    }

    //End Parallel Block
    MPI_Finalize();

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            clock::now() - start).count();

    std::cout << elapsed << " ms, " << iterations << " iter";

    if(elapsed > 0)
    {
        std::cout << ", " << std::scientific << std::setprecision(3)
                  << iterations * SIZE / static_cast<double>(elapsed) * 1000
                  << " cells/s";
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}