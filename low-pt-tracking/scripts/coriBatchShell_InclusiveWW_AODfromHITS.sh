#
# WARNING:
# Execution driver. Submit the submission script ending with .slr
#
##################################################
# Settings
##################################################

ASetupStr='Athena,21.0.53,here'

#List of dynamically needed expansions (in the code with __XXX__)
# BASE_DIR -> Test area
# OUTDIR -> Base output directory
# SUBDIR -> optional sub-directory

BASEDIR="/global/homes/a/adimitri/exclusiveWW/Tracking/run_coriBatch"
OUTDIR="/global/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/InclusiveWW_361600_fromHITS_noPileUp"
SUBDIR="reco_AOD_ver5"

if ! [ -z $1 ]; then
 BASEDIR=$1
 OUTDIR=$1
fi
if ! [ -z $2 ]; then
 SUBDIR=$2
fi

SAMPLE="/global/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.361600.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvlv.simul.HITS.e4616_s3126/HITS.13536993._000040.pool.root.1"
myMaxEvents=100 #define initial number of events
mySkipEvents=0 #define initial number of events to skip

if ! [ -z "${SLURM_ARRAY_TASK_ID}" ] ; then
    #Split input file into 100 events chunks
    mySkipEvents=$(((SLURM_ARRAY_TASK_ID-1)*100))
    SUBDIR="${SUBDIR}_Job${SLURM_ARRAY_TASK_ID}"
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

#Setup ATLAS software on PDSF
shopt -s expand_aliases
source /global/project/projectdirs/atlas/scripts/setupATLAS.sh
setupATLAS #-c slc6+batch

echo "Setting up ATLAS software $ASetupStr in $BASEDIR"
cd $BASEDIR
mkdir -pv $SUBDIR
cd $SUBDIR
echo asetup ${ASetupStr}
asetup ${ASetupStr}
export FRONTIER_SERVER="(serverurl=http://atlasfrontier-ai.cern.ch:8000/atlr)(serverurl=http://lcgft-atlas.gridpp.rl.ac.uk:3128/frontierATLAS)"

echo "------ ENVIRONMENT ---------"
printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

CPYDIR=$OUTDIR/$SUBDIR
echo "Output: ${CPYDIR}"
mkdir -pv ${CPYDIR}

WRKDIR=$PWD
echo "Work dir: ${PWD}"

#cd $CPYDIR

pwd
echo "Executing the job:"

# Setup job settings and run it 
Reco_tf.py \
--digiSteeringConf 'StandardSignalOnlyTruth' \
--conditionsTag 'default:OFLCOND-MC16-SDR-20' \
--valid 'True' \
--autoConfiguration 'everything' \
--steering 'doRDO_TRIG' \
--preInclude 'RDOtoRDOTrigger:RecExPers/RecoOutputMetadataList_jobOptions.py' \
--postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' \
--postInclude 'default:PyJobTransforms/UseFrontier.py' \
--preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False)' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' 'all:from ParticleBuilderOptions.AODFlags import AODFlags; AODFlags.ThinInDetForwardTrackParticles.set_Value_and_Lock(True)' \
--triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2179,51,207' \
--geometryVersion 'default:ATLAS-R2-2016-01-00-01' \
--numberOfCavernBkg '0' \
--inputHITSFile ${SAMPLE} \
--outputAODFile file.AOD.${SLURM_ARRAY_TASK_ID}.pool.root \
--outputRDOFile file.RDO.${SLURM_ARRAY_TASK_ID}.pool.root \
--maxEvents ${myMaxEvents} \
--skipEvents=${mySkipEvents} \
--jobNumber 361600 \
--DataRunNumber 361600 \
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
mv $WRKDIR/log.* $CPYDIR/
echo ""
echo "Content of destination: $CPYDIR"
ls -lh $CPYDIR/
