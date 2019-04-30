#
# WARNING:
# Execution driver. Submit the submission script ending with .slr
#
##################################################
# Settings
##################################################

ASetupStr='Athena,21.0.77,here'

#List of dynamically needed expansions (in the code with __XXX__)
# BASE_DIR -> Test area
# OUTDIR -> Base output directory
# SUBDIR -> optional sub-directory

BASEDIR="/global/homes/s/spgriso/code/AtlasExclWW/trk-exclusive-ww/low-pt-tracking/build"
OUTDIR="/global/projecta/projectdirs/atlas/spgriso/data/ExclWW/data/private-AOD/"
SUBDIR="mc16_13TeV.410471.ttbar_allhad.minbias-reco.v2"

if ! [ -z $1 ]; then
 BASEDIR=$1
 OUTDIR=$1
fi
if ! [ -z $2 ]; then
 SUBDIR=$2
fi

SAMPLE="/global/projecta/projectdirs/atlas/spgriso/data/ExclWW/data/RDO/mc16_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.recon.RDO.e6337_s3126_r11193/RDO.16844823._000053.pool.root.1"
myMaxEvents=50 #define initial number of events
mySkipEvents=0 #define initial number of events to skip

if ! [ -z "${SLURM_ARRAY_TASK_ID}" ] ; then
    #Split input file into 50 events chunks
    mySkipEvents=$(((SLURM_ARRAY_TASK_ID-1)*50))
fi
echo "=============================="
echo "sample = $SAMPLE"
echo "myMaxEvents = ${myMaxEvents}"
echo "mySkipEvents = ${mySkipEvents}"
echo "=============================="

echo "Starting batch job. Processing sample: $SAMPLE"

##################################################
# END of Settings
##################################################

#Setup ATLAS software on CORI
shopt -s expand_aliases
source /global/project/projectdirs/atlas/scripts/setupATLAS.sh
setupATLAS #-c slc6+batch

echo "Setting up ATLAS software $ASetupStr in $BASEDIR"
cd $BASEDIR
echo asetup ${ASetupStr}
asetup ${ASetupStr}
#source x86_64-slc6-gcc62-opt/setup.sh
export FRONTIER_SERVER="(serverurl=http://atlasfrontier-ai.cern.ch:8000/atlr)(serverurl=http://lcgft-atlas.gridpp.rl.ac.uk:3128/frontierATLAS)"

echo "------ ENVIRONMENT ---------"
printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

CPYDIR=$OUTDIR/$SUBDIR
echo "Output: ${CPYDIR}"
mkdir -pv ${CPYDIR}

RANDOM_TRAILER=`cat /dev/urandom | tr -cd 'a-f0-9' | head -c 4`
WRKDIR="$SCRATCH/$SUBDIR-${RANDOM_TRAILER}"
mkdir -pv $WRKDIR
cd $WRKDIR
echo "Work dir: ${PWD}"

pwd
echo "Executing the job:"

# Setup job settings and run it 
Reco_tf.py \
--digiSteeringConf 'StandardInTimeOnlyTruth' \
--conditionsTag 'default:OFLCOND-MC16-SDR-25' \
--valid 'True' \
--pileupFinalBunch '6' \
--numberOfHighPtMinBias '0.2595392' \
--autoConfiguration 'everything' \
--numberOfLowPtMinBias '99.2404608' \
--steering 'doRDO_TRIG' \
--preInclude 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInlcude.PileUpBunchTrainsMC16c_2017_Config1.py,RunDependentSimData/configLumi_run310000.py' \
--postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'RAWtoESD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib;xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' 'ESDtoDPD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' \
--postInclude 'default:PyJobTransforms/UseFrontier.py' \
--preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False)' 'RAWtoESD:from InDetRecExample.InDetJobProperties import InDetFlags;InDetFlags.doMinBias.set_Value_and_Lock(True);' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' \
--triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' \
--geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' \
--inputRDOFile ${SAMPLE} \
--outputAODFile file.AOD.${SLURM_ARRAY_TASK_ID}.pool.root \
--maxEvents ${myMaxEvents} \
--skipEvents=${mySkipEvents}
>& log.RecoTf

echo "Job done."

##compress log file
gzip $WRKDIR/log.*

##List output
echo ""
echo "List of local output files:"
echo "ls -lh ${WRKDIR}/"
ls -lh $WRKDIR/


##now copy the output files and log file to output location
echo ""
echo "Now copying to ${CPYDIR}"
mv $WRKDIR/*root* $CPYDIR/
for pippo in $WRKDIR/log.*; do
  newName=`echo $pippo | sed 's/gz$/'${SLURM_ARRAY_TASK_ID}'.gz/g'`
  mv $pippo $newName
done
mv $WRKDIR/log.* $CPYDIR/
echo ""
echo "Content of destination: $CPYDIR"
ls -lh $CPYDIR/
