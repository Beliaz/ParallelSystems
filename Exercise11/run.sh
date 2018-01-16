# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N real

# Execute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Redirect output stream to this file.
#$ -o real.log

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

module load intel/15.0

for threads in 1 2 4 8
do
    export OMP_NUM_THREADS=$threads

    echo "-----------------------------------------------------"
    echo $threads " thread(s)"
    echo "-----------------------------------------------------"
    
    ./real_old_gcc | grep -E "benchmk|Verification"

    echo

    ./real | grep -E "benchmk|Verification"

    echo
    echo "====================================================="
    echo 
done

module unload intel/15.0

