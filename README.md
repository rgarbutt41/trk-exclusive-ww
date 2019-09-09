# Introduction

Git repository for analysis code for low-pt tracking studies for Exclusive WW analysis


# Code structure

Inside `trk-exclusive-ww` is the full code:
- `low-pt-tracking` for tracking code inside athena
  with `run` folder where you can run reconstruction scripts, a set of useful scripts for launching reconstruction are available in the `scripts` folder
- `analysis` is the subdirector for performance analysis work
  with the actual analysis code is in `source`, compile in `build` and run in `run`


# Low-pt tracking Athena code setup

Patrick's fork of athena must be checked-out!!  You can do this in your clone of TrkExclusiveWW, but make sure you include your athena folder in .gitignore (if that doesn't automatically happen).  Instructions are as follows to do a sparse checkout:

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
git atlas addpkg InDetRecToolInterfaces
git atlas addpkg InDetAmbiTrackSelectionTool
git atlas addpkg TrkAmbiguityProcessor
cd ..
mkdir build
cd build
asetup Athena,21.0.91,slc6,here
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
| 21.0.97 | Synchronized for development with recent release |
| 21.0.77 | standard reconstruction release for ATLMCPROD-6852 |

e.g. asetup Athena,21.0.97,slc6,here

Note: when changing release rememeber to re-run cmake and clean-up the build folder first

## Running Reco_tf to make AODs out of RDOs
An example of how to use low-pt tracking:
```
Reco_tf.py --digiSteeringConf 'StandardInTimeOnlyTruth' --conditionsTag 'default:OFLCOND-MC16-SDR-25' --valid 'True' --pileupFinalBunch '6' --numberOfHighPtMinBias '0.2595392' --autoConfiguration 'everything' --numberOfLowPtMinBias '99.2404608' --steering 'doRDO_TRIG' --preInclude 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInlcude.PileUpBunchTrainsMC16c_2017_Config1.py,RunDependentSimData/configLumi_run310000.py' --postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'RAWtoESD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib;xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' 'ESDtoDPD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' --postInclude 'default:PyJobTransforms/UseFrontier.py' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False)' 'RAWtoESD:from InDetRecExample.InDetJobProperties import InDetFlags;InDetFlags.doLowPtRoI.set_Value_and_Lock(True);InDetFlags.LowPtRoIStrategy.set_Value_and_Lock(2);' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' --geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' --inputRDOFile "/global/project/projectdirs/atlas/adimitri/MC_request/TruthPileUp/InclusiveWW/Test_FullInTimeTruth_ver02/file.RDO.pool.root" --outputAODFile file.AOD.pool.root --maxEvents 10
```
Here of course, the --inputRDOFile, --outputAODFile, and --maxEvents should be changed to suit your preferences.  In the RAWtoESD part of the preselect, you turn on the low-pt tracking with
```
'RAWtoESD:from InDetRecExample.InDetJobProperties import InDetFlags;InDetFlags.doLowPtRoI.set_Value_and_Lock(True);'
```
You can specify the strategy used to find the region of interest with low-pt tracking with InDetFlags.LowPtRoIStrategy.set_Value_and_Lock(2).  1 is track-based, 2 is HS truth-based, and 3 is file-based.  If you want to use a file, you must specify, e.g., InDetFlags.LowPtRoIFile.set_Value_and_Lock("/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/low-pt-tracking/roi_test.txt").

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

## Code structure

The analysis code is in `source` which consists from:
- `LowPtAnalysis` for low pt studies
- `Exclusivity` for exclusivity studies macros

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



