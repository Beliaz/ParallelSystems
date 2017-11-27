# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N comparison

# Execute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Redirect output stream to this file.
#$ -o output.dat

# Join the error stream to the output stream.
#$ -j yes

# Send status information to this email address.
#$ -M florian.tischler@outlook.com

# Send me an e-mail when the job has finished.
#$ -m e

# Use the parallel environment "openmp" with 8 job slots. Each requested
# job slot will be assigned one core on the execution host.
#$ -pe openmp 8

#$ -l h_vmem=2G

n_1d="20000"
n_2d="150"
n_3d="64"

#module load gcc/5.1.0

for threads in 1 2 4 8
do
    export OMP_NUM_THREADS=$threads

    echo
    echo $threads " thread(s) -------------------------------------------------"
    echo 

    printf "stencil_1:$threads:1D:"$n_1d": "; ./stencil_1 1 1 $n_1d 273 1000
    printf "stencil_2:$threads:1D:"$n_1d": "; ./stencil_2 1 $n_1d 273 1000
    
    echo

    printf "stencil_1:$threads:2D:"$n_2d": "; ./stencil_1 1 2 $n_2d 2000 10 273 1000
    printf "stencil_2:$threads:2D:"$n_2d": "; ./stencil_2 2 $n_2d 273 1000 2000 10

    echo

    printf "stencil_1:$threads:3D:"$n_3d": "; ./stencil_1 1 3 $n_3d 2000 10 273 1000 8000 200
    printf "stencil_2:$threads:3D:"$n_3d": "; ./stencil_2 3 $n_3d 273 1000 2000 10 200 8000
done

#module unload gcc/5.1.0