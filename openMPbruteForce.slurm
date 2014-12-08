#!/bin/bash -x
#SBATCH --job-name="DES_OpenMP_Brute_Force"
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=10
#SBATCH --ntasks=10
#SBATCH --exclusive
#SBATCH --export=ALL
#SBATCH --time=00:59:59
#SBATCH -o bruteForceOMP.out
#SBATCH -e bruteForceOMP.err
#SBATCH -p mganesh
cd $SLURM_SUBMIT_DIR
JOBID=‘echo $SLURM_JOBID’
export OMP_NUM_THREADS=
./openMPbruteForce
echo "job has finished"
