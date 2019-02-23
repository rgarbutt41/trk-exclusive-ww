Folder for doing analysis.


///////////////////////

To set this up

In TrkExclusiveWW/analysis/LowPtAnalysis/build do:
asetup 21.2.48,AnalysisBase
cmake ../source/
make
source x86_64-slc6-gcc62-opt/setup.sh



/////////////////////////////

Actual analysis code is in TrkExclusiveWW/analysis/LowPtAnalysis/source



///////////////////////////

To run the analysis go to: TrkExclusiveWW/analysis/LowPtAnalysis/run
Run with:
ATestRun_eljob.py --submission-dir=submitDir

where '--submission-dir=submitDir' specifies the output directory.  This must be renamed every time (or else delete the old submitDir)

If you  make a new steering macro, you must make it an executable with:
chmod +x <macro.py>
You must then go to TrkExclusiveWW/analysis/LowPtAnalysis/build and do
cmake ../source/
make
source x86_64-slc6-gcc62-opt/setup.sh



////////////////////////////

Right now the code just accesses an AOD from Simone's directory and makes a very simple NTuple with some event and Tau information

The retrieved AOD is: /global/projecta/projectdirs/atlas/spgriso/code/trk-exclusive-ww-3/run/AOD.pool.root



