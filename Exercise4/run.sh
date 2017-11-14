# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N run

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

for num_samples in "100000" "1000000" "100000000" 
do
    echo $num_samples " Samples =============="
    
    for threads in 1 2 4 8
    do
        export OMP_NUM_THREADS=$threads

        echo $threads " - Threads ------------------------"

	echo "gcc"
	./build/pi_par_1_gcc $num_samples
	./build/pi_par_1_gcc $num_samples
	./build/pi_par_1_gcc $num_samples
	echo 

	echo "icc"
	./build/pi_par_1_icc $num_samples
	echo 

	echo "pgi"
	./build/pi_par_1_pgi $num_samples
	echo 

	echo "gcc"
	./build/pi_par_2_gcc $num_samples
	./build/pi_par_2_gcc $num_samples
	./build/pi_par_2_gcc $num_samples
	echo

	echo "icc"
	./build/pi_par_2_icc $num_samples
	echo

	echo "pgi"
	./build/pi_par_2_pgi $num_samples
	echo
    done
done
	
