Git repo for analysis code for low-pt tracking studies for Exclusive WW analysis

My fork of athena must be checked-out!!  You can do this in your clone of TrkExclusiveWW, but make sure you include your athena folder in .gitignore (if that doesn't automatically happen).  Instructions are as follows to do a sparse checkout:

///////////////////////////////////////

mkdir (name of folder you want athena in)

cd (name of folder you want athena in)

git atlas init-workdir https://:@gitlab.cern.ch:8443/wmccorma/athena.git

cd athena

git checkout 21.0-trk-lowpt-highpu

git atlas addpkg InDetRecExample

git atlas addpkg SiSPSeededTrackFinder

git atlas addpkg SiSpacePointsSeedTool_xk

cd ..

mkdir build

cd build

asetup 21.0,latest,Athena,here

mv CMakeLists.txt ../

cmake ../athena/Projects/WorkDir/

make -j2

source x86_64-slc6-gcc62-opt/setup.sh

cd ../..

Then use your favorite text editor to make a file called ".gitignore".  Add (name of folder you want athena in) to .gitignore

//////////////////////////////////////

You should be able to reco_tf in (name of folder you want athena in)




////////////////////////////////////

FOR ANALYSIS

'analysis' is the subdirector for doing analysis work.  There is an additional README file there.

The actual analysis code is in analysis/LowPtAnalysis/source
Compile in analysis/LowPtAnalysis/build
Run in analysis/LowPtAnalysis/run
