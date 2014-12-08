#!/bin/bash -x
#SBATCH --job-name="DES_OpenMP_Brute_Forc10"
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=10
#SBATCH --ntasks=10
#SBATCH --exclusive
#SBATCH --export=ALL
#SBATCH --time=00:59:59
#SBATCH -o bruteForceOMP10.out
#SBATCH -e bruteForceOMP10.err
#SBATCH -p mganesh
cd $SLURM_SUBMIT_DIR
JOBID=‘echo $SLURM_JOBID’
export OMP_NUM_THREADS=10
./openMPbruteForce
echo "job has finished"
