#!/bin/bash
#SBATCH --job-name=myjob       # The job name
#SBATCH --error=myjob.e%j      # error file name (%j expands to jobID)
#SBATCH --output=myjob.o%j     # output and error file name (%j expands to jobID)
#SBATCH --partition=comp       # Use the command "sinfo" to get the name on your HPC facility (comp on LNS)
#SBATCH --time=24:00:00        # Maximum running time
#SBATCH --nodefile=nodes.txt   # A file with the (preferred) list of nodes to allocate the job
#
# MPI
#SBATCH --nodes=4              # Number of nodes (tasks will try to split among these nodes)
#SBATCH --ntasks=64            # Number of cpus or tasks (may or may not on different machines). This is the same as ranks on MPI. If --nodes was not specified then this will try to use one node per task
#SBATCH --ntasks-per-node=16   # Maximum number of tasks per node (max 24 on LNS)
#
# OpenMP
##SBATCH --ntasks=1             # Number of cpus or tasks (may or may not on different machines). This is the same as mpitasks. If --nodes was not specified then this will try to use one node per task
##SBATCH --cpus-per-task=16     # Use this for omp applications to force the allocation of cpus per task (cpu is a processor)


EXECUTABLE="./bin/roberto_demo_nasch_npbcwcw"
PARAMETERS="--vmaxSpeed 2 --zebra false  --alphaMin 0.1 --alphaMax 0.1 --alpha_pMin 0 --alpha_pMax 0.63 --alpha_pStep 0.02 --betaMin 0.02 --betaMax 1 --betaStep 0.02"

echo "--------------------------------------------"
echo "General information"
echo "SLURM_JOB_ID:" $SLURM_JOB_ID
echo "SLURM_JOB_NAME:" $SLURM_JOB_NAME
echo "SBATCH_TIMELIMIT:"$SBATCH_TIMELIMIT
echo "SLURM_JOB_PARTITION:"$SLURM_JOB_PARTITION
echo "SLURM_JOB_NODELIST:"$SLURM_JOB_NODELIST
echo "SBATCH_ARRAY_INX:"$SBATCH_ARRAY_INX
echo ""
echo "--------------------------------------------"
echo "CPUs, nodes and tasks"
echo "SLURM_JOB_NUM_NODES:" $SLURM_JOB_NUM_NODES
echo "SLURM_NTASKS:" $SLURM_NTASKS
echo "SLURM_NTASKS_PER_NODE:"$SLURM_NTASKS_PER_NODE
echo "SLURM_CPUS_PER_TASK:" $SLURM_CPUS_PER_TASK
echo "--------------------------------------------"
echo "GPUs, nodes and tasks"
echo "SLURM_GPUS:"$SLURM_GPUS
echo "SLURM_GPUS_PER_NODE:"$SLURM_GPUS_PER_NODE
echo "SLURM_GPUS_PER_TASK:"$SLURM_GPUS_PER_TASK
echo "SLURM_NTASKS_PER_GPU:"$SLURM_NTASKS_PER_GPU
echo ""
echo "--------------------------------------------"
echo "Memory"
echo "SLURM_MEM_PER_NODE:"$SLURM_MEM_PER_NODE
echo "SLURM_MEM_PER_CPU:"$SLURM_MEM_PER_CPU
echo "SLURM_MEM_PER_GPU:"$SLURM_MEM_PER_GPU
echo ""
echo "--------------------------------------------"
echo "EXECUTABLE:"$EXECUTABLE
echo "PARAMETERS:"$PARAMETERS
echo "--------------------------------------------"
echo ""

# Load your modules here
module purge
module load compilers/gcc/gnu/5.4.0
module load compilers/cmake/3.16.1
module load tools/openmpi/gnu/1.8.4

# Run your task here
mpirun -np $SLURM_NTASKS $EXECUTABLE $PARAMETERS
