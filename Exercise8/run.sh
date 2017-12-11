#!/bin/bash

# Execute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N mpi_stencil

# Redirect output stream to this file.
#$ -o output.dat

# Join the error stream to the output stream.
#$ -j yes


# Specify the amount of virtual memory given to each MPI process
# in the job.
#$ -l h_vmem=2G

# Use the parallel environment "openmpi-fillup", which assigns as many processes
# as available on each host. Start 16 MPI processes across an arbitrary number of
# hosts. For each process, SGE will reserve one CPU-core.
#$ -pe openmpi-fillup 64

module load gcc/5.1.0
module load openmpi/2.1.1

mpirun -np $NSLOTS ./stencil

module unload openmpi/2.1.1
module unload gcc/5.1.0
