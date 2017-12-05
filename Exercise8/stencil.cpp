//
// Created by ivan on 04/12/17.
//

#include <iostream>
#include <sstream>
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

    double *get_borders(unsigned int from_x,
                        unsigned int from_y,
                        unsigned int to_x,
                        unsigned int to_y)
    {
        int size=to_x-from_x;
        double * borders = new double[size * 4];
        int counter=0;
        for (int i = 0; i < size; ++i) {
            borders[counter++] = get(from_x, from_y + i);
        }
        for (int i = 0; i < size; ++i) {
            borders[counter++] = get(from_x + i, to_y);
        }
        for (int i = 0; i < size; ++i) {
            borders[counter++] = get(to_x, from_y + i);
        }
        for (int i = 0; i < size; ++i) {
            borders[counter++] = get(from_x +i, from_y);
        }
        return borders;
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

    const auto xpos=my_rank % blocks;
    const auto ypos=my_rank / blocks;

    const auto left_proc = (ypos * blocks) + xpos + 1;
    const auto right_proc = (ypos * blocks) + xpos - 1;
    const auto top_proc = ( (ypos - 1) * blocks) + xpos;
    const auto bottom_proc = ( (ypos + 1) * blocks) + xpos;

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
        double * border = grid2->get_borders(from_x,from_y,to_x,to_y);
        for (int i = 0; i < (to_x-from_x) * 4 ; ++i) {
            std::cout<<border[i]<<", ";
        }
        MPI_Send(&grid2,SIZE,MPI_DOUBLE,top_proc,0,MPI_COMM_WORLD);
        MPI_Send(&grid2,SIZE,MPI_DOUBLE,left_proc,1,MPI_COMM_WORLD);
        MPI_Send(&grid2,SIZE,MPI_DOUBLE,bottom_proc,2,MPI_COMM_WORLD);
        MPI_Send(&grid2,SIZE,MPI_DOUBLE,right_proc,3,MPI_COMM_WORLD);

        MPI_Recv(&grid2,SIZE,MPI_DOUBLE,top_proc,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&grid2,SIZE,MPI_DOUBLE,left_proc,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&grid2,SIZE,MPI_DOUBLE,bottom_proc,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&grid2,SIZE,MPI_DOUBLE,right_proc,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        double d_epsilon = iteration(grid2,grid1,from_x,from_y,to_x,to_y);

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