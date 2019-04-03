# Introduction

Git repo for analysis code for low-pt tracking studies for Exclusive WW analysis

Patrick's fork of athena must be checked-out!!  You can do this in your clone of TrkExclusiveWW, but make sure you include your athena folder in .gitignore (if that doesn't automatically happen).  Instructions are as follows to do a sparse checkout:

# Low-pt tracking Athena code setup

## First-time setup instructions

```
git clone https://gitlab.cern.ch/berkeleylab/trk-exclusive-ww.git
mkdir low-pt-tracking
cd low-pt-tracking
git atlas init-workdir https://:@gitlab.cern.ch:8443/wmccorma/athena.git
cd athena
git checkout 21.0-trk-lowpt-highpu
git atlas addpkg InDetRecExample
git atlas addpkg SiSPSeededTrackFinder
git atlas addpkg SiSpacePointsSeedTool_xk
cd ..
mkdir build
cd build
asetup 21.0,latest,Athena,here #see below for specific releases
rm CMakeLists.txt
cmake ../athena/Projects/WorkDir/
make -j2
source x86_64-slc6-gcc62-opt/setup.sh
cd ../..
```

## Setup instruction (every new shell)
```
cd low-pt-tracking/build
asetup 21.0,latest,Athena,here #see below for specific releases
source ./*/setup.sh
```

## Releases
Current working releases:

| Release | Description |
| ------- | ----------- |
| 21.0.77 | standard reconstruction release for ATLMCPROD-6852 |


# Code structure

After you've setup the code as above:
Inside `trk-exclusive-ww` is the full code:
- `low-pt-tracking` for tracking code inside athena
  with `run` folder where you can run reconstruction scripts, a set of useful scripts for launching reconstruction are available in the `scripts` folder
- `analysis` is the subdirector for doing performance analysis work
  with the actual analysis code is in `source`, compile in `build` and run in `run

# Setup performance analysis code

## Initial setup

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


## Running instructions

To run the analysis go to: make a folder called e.g. `trk-exclusive-ww/analysis/run`, and cd into it
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


