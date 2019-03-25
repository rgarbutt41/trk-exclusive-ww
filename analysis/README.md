# Introduction

Folder for doing analysis.


# Initial setup

To set this up the first time, from the main repository folder:
```
cd analysis/
mkdir build
cd build/
asetup 21.2.69,AnalysisBase
cmake ../source/
make
asetup source ./*/setup.sh
```

Note that having `asetup` preceding the `source` commmand allows the file to be sourced automatically
the next time you setup the release with `asetup --restore`.
Otherwise you need to remember to source this file every time you log in and setup the release.

# Code structure

Actual analysis code is in TrkExclusiveWW/analysis/source


# Running instructions

To run the analysis go to: make a folder called e.g. `TrkExclusiveWW/analysis/run`, and cd into it
Run with:
```
runBasicPerf.py --submission-dir=submitDir
```

where `--submission-dir=submitDir` specifies the output directory.  This must be renamed every time (or else delete the old submitDir)

If you  make a new steering macro, please put it it TrkExclusiveWW/analysis/source/LowPtAnalysis/python, you must make it an executable with:
```
chmod +x <macro.py>
```
You must then go to `TrkExclusiveWW/analysis/build` and do
```
cmake ../source/
make
source x86_64-slc6-gcc62-opt/setup.sh
```

Right now the code just accesses an AOD from Simone's directory and makes a very simple NTuple with some event and Tau information

The retrieved AOD is: `/global/projecta/projectdirs/atlas/spgriso/code/trk-exclusive-ww-3/run/AOD.pool.root`



