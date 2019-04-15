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

#BASEDIR="/global/homes/a/adimitri/exclusiveWW/Tracking/run_coriBatch/"
#OUTDIR="/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/InclusiveWW_361600_fromHITS_noPileUp/"
#SUBDIR="reco_AOD_ver2"

#if ! [ -z $1 ]; then
# BASEDIR=$1
# OUTDIR=$1
#fi
#if ! [ -z $2 ]; then
# SUBDIR=$2
#fi

OUTDIR=/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/low-pt-tracking/run/ttbar_pileup_noflag_pileuptruth_${SLURM_ARRAY_TASK_ID}

SAMPLE="/global/projecta/projectdirs/atlas/spgriso/data/ExclWW/data/RDO/mc16_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.recon.RDO.e6337_s3126_r11193/RDO.16844823._000053.pool.root.1"

#SampleList=(/global/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.361600.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvlv.simul.HITS.e4616_s3126/HITS.13536993._0000*)
#
#SAMPLE=""
#if ! [ -z "${SLURM_ARRAY_TASK_ID}" ] && ! [ ${SLURM_ARRAY_TASK_ID} == "undefined" ]; then
#    SAMPLE=${SampleList[$((SLURM_ARRAY_TASK_ID-1))]}
#
#    if [ -z "$SAMPLE" ]; then
#        echo "ERROR: Invalid sample for SLURM_ARRAY_TASK_ID=${SLURM_ARRAY_TASK_ID}"
#        exit 1
#    fi
#fi
#echo "Starting batch job. Processing sample: $SAMPLE"


myMaxEvents=100 #define initial number of events
mySkipEvents=0 #define initial number of events to skip

if ! [ -z "${SLURM_ARRAY_TASK_ID}" ] ; then
    #Split input file into 100 events chunks
    mySkipEvents=$(((SLURM_ARRAY_TASK_ID-1)*100))
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
setupATLAS

echo "Setting up ATLAS software $ASetupStr in $BASEDIR/"
#cd $BASEDIR/
#echo asetup ${ASetupStr}
#asetup ${ASetupStr}
#export FRONTIER_SERVER="(serverurl=http://atlasfrontier-ai.cern.ch:8000/atlr)(serverurl=http://lcgft-atlas.gridpp.rl.ac.uk:3128/frontierATLAS)"

mkdir $OUTDIR
cd $OUTDIR/
echo asetup ${ASetupStr}
asetup ${ASetupStr}
export FRONTIER_SERVER="(serverurl=http://atlasfrontier-ai.cern.ch:8000/atlr)(serverurl=http://lcgft-atlas.gridpp.rl.ac.uk:3128/frontierATLAS)"

echo "------ ENVIRONMENT ---------"
printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

#CPYDIR=$BASEDIR/$SUBDIR
#echo "Output: ${CPYDIR}"
#mkdir -pv ${CPYDIR}

#WRKDIR=$PWD
#echo "Work dir: ${PWD}"

#cd $CPYDIR

pwd

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
--preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False)' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' \
--triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' \
--geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' \
--inputRDOFile ${SAMPLE} \
--outputAODFile file.AOD.${SLURM_ARRAY_TASK_ID}.pool.root \
--maxEvents ${myMaxEvents} \
--skipEvents=${mySkipEvents}
#>& log.RecoTf


#cp file.AOD.${SLURM_ARRAY_TASK_ID}.pool.root /global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/low-pt-tracking/run/Inclusive_noflag_AODs/

##compress log file
#gzip $WRKDIR/log.*

##List output
#echo ""
#echo "List of local output files:"
#echo "ls -lh ${WRKDIR}/"
#ls -lh $WRKDIR/


##now copy the output files and log file to output location
#echo ""
#echo "Now copying to ${CPYDIR}"
#mv $WRKDIR/*root* $CPYDIR/
#mv $WRKDIR/log.* $CPYDIR/
#echo ""
#echo "Content of destination: $CPYDIR"
#ls -lh $CPYDIR/
