#Intro

This folder contains scripts for running simulation and reconstruction.

For now, the e.g. RDO files are hard coded in the .sh files

The .sh files contain actual reco_tf commands

The .slr files run the .sh files within SLURM

#Running

To actually run, do e.g.:

sbatch -p shared-chos -t 10:00:00 example_slr.slr