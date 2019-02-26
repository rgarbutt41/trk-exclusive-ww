# Introduction

Git repo for analysis code for low-pt tracking studies for Exclusive WW analysis

My fork of athena must be checked-out!!  You can do this in your clone of TrkExclusiveWW, but make sure you include your athena folder in .gitignore (if that doesn't automatically happen).  Instructions are as follows to do a sparse checkout:

# Athena code setup

## First-time setup instructions

```
mkdir trk-exclusive-ww-athena
cd trk-exclusive-ww-athena
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
mv CMakeLists.txt ../
cmake ../athena/Projects/WorkDir/
make -j2
source x86_64-slc6-gcc62-opt/setup.sh
cd ../..
```

## Setup instruction (every new shell)
```
cd trk-exclusive-ww-athena/build
asetup 21.0,latest,Athena,here #see below for specific releases
source ./*/setup.sh
```

## Releases
Current working releases:

| Release | Description |
| ------- | ----------- |
| 21.0.77 | standard reconstruction release for ATLMCPROD-6852 |


## Running reconstruction

After you've setup the code as above:

```
cd trk-exclusive-ww-athena/
mkdir run
cd run/
```

Here you can run reconstruction scripts, a set of useful scripts for launching reconstruction are available in the `scripts` folder.


# Performance analysis

'analysis' is the subdirector for doing analysis work.  There is an additional README file there.

The actual analysis code is in `analysis/source`

Compile in `analysis/build`

Run in `analysis/run`
