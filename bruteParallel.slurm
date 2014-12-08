#!/bin/bash -x
#SBATCH --job-name="DES_Brute_Force_Decryption"
#SBATCH --nodes=16
#SBATCH --ntasks-per-node=8
#Multiply nn and xx (denoted below as nn*xx) and use that in the line
#SBATCH --ntasks=128
#SBATCH --exclusive
#SBATCH --export=ALL
#SBATCH --time=00:59:59
#SBATCH -o bruteParallel.out
#SBATCH -e bruteParallel.err
# Go to the directoy from which our job was launched
cd $SLURM_SUBMIT_DIR
# Create a short JOBID base on the one provided by the scheduler
JOBID='echo $SLURM_JOBID'
# Save a copy of our environment and script
cat $0 > script.$JOBID
printenv > env.$JOBID
# Run the job.
# The echo will go into the standard output for this job
# The standard output file will end up in the directory
# from which the job was launched.
echo "running job"
srun bruteParallel
echo "job has finished"
