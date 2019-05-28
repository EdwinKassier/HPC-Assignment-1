#!/bin/sh

######################################################################################################
#                                                                                                    #
# This example submission script contains several important directives, please examine it thoroughly #
#                                                                                                    #
######################################################################################################

# The line below indicates which accounting group to log your job against - must be icts
#SBATCH �account=icts

# The line below selects the group of nodes you require - you will all run on curie
#SBATCH �partition=curie

# The line below means you need 1 worker node and a total of 2 cores
#SBATCH �nodes=2 �ntasks=4
#  To evenly split over two nodes use: --ntasks-per-node=2

# The line below indicates the wall time your job will need, 10 hours for example. NB, this is a mandatory directive!
# Note that 10:00 is 10 minutes
#SBATCH --time=30:00

# The line below means your job will be canceled if it consumes more than 4GB of RAM per requested core. Max 9000.
#SBATCH --mem-per-cpu=4000

# A sensible name for your job, try to keep it short
#SBATCH --job-name="MyJob"

#Modify the lines below for email alerts. Valid type values are NONE, BEGIN, END, FAIL, REQUEUE, ALL 
#SBATCH --mail-user=michelle.kuttel@uct.ac.za
#SBATCH --mail-type=BEGIN,END,FAIL

# NB, for more information read https://computing.llnl.gov/linux/slurm/sbatch.html

# Use module to gain easy access to software, typing module avail lists all packages.
# Example:
# module load python/anaconda-python-2.7
module load compilers/gcc820
module loadmpi/openmpi-4.0.1

# Your science stuff goes here...
mkdir Output_Tests
echo "SBATCH --nodes=4 --ntasks=1" > Output_Tests/descr.text
make
export OMP_DYNAMIC=TRUE
make run1 > Output_Tests/Output1.txt
make run2 > Output_Tests/Output2.txt