#
# WARNING:
# Execution driver. Submit the submission script ending with .slr
#
##################################################
# Settings
##################################################

ASetupStr='Athena,21.0.77,here'

BASEDIR="/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/trk-exclusive-ww-athena/run"
OUTDIR="/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/trk-exclusive-ww-athena/run/Apr02_batchcoritest"
#SUBDIR="arraytest_03_20_runfolder"

if ! [ -z $1 ]; then
 SUBDIR=$1
fi

#SampleList=(/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.363754.Herwig7EvtGen_BudnevQED_ggTOmumu_45M100_LeptonFilter.recon.RDO.e7175_e5984_s3126_r10201/RDO.*.pool.root.1)
#SAMPLE=/global/homes/a/adimitri/exclusiveWW/Tracking/run/tmp.RDO
#SAMPLE=/global/projecta/projectdirs/atlas/spgriso/code/trk-exclusive-ww-3/data/RDO.16716257._000002.pool.root.1
SAMPLE=/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.361600.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvlv.simul.HITS.e4616_s3126/HITS.13536993._000010.pool.root.1
#SAMPLE=""
#if ! [ -z "${SLURM_ARRAY_TASK_ID}" ] && ! [ ${SLURM_ARRAY_TASK_ID} == "undefined" ]; then
#    SAMPLE=${SampleList[$((SLURM_ARRAY_TASK_ID-1))]}
#
#    if [ -z "$SAMPLE" ]; then
#	echo "ERROR: Invalid sample for SLURM_ARRAY_TASK_ID=${SLURM_ARRAY_TASK_ID}"
#	exit 1
#    fi
#fi
#echo "Starting batch job. Processing sample: $SAMPLE"

##################################################
# END of Settings
##################################################

#Setup ATLAS software on PDSF
shopt -s expand_aliases
source /global/project/projectdirs/atlas/scripts/setupATLAS.sh
# export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
# alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
setupATLAS
#lsetup asetup

echo "Setting up ATLAS software $ASetupStr in $BASEDIR/"
cd $BASEDIR/

pwd
rm -rf $OUTDIR
mkdir $OUTDIR
cd $OUTDIR
pwd

echo asetup ${ASetupStr}
asetup ${ASetupStr}

export FRONTIER_SERVER="(serverurl=http://atlasfrontier-ai.cern.ch:8000/atlr)(serverurl=http://lcgft-atlas.gridpp.rl.ac.uk:3128/frontierATLAS)"

echo "------ ENVIRONMENT ---------"
#printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

#CPYDIR=$BASEDIR/${SUBDIR}_${SLURM_ARRAY_TASK_ID}
#echo "Output: ${CPYDIR}"
#mkdir -pv ${CPYDIR}


#cd $TMPDIR
#WRKDIR=$PWD
#echo "Work dir: ${PWD}"

#cd $CPYDIR

pwd


echo "About to run reconstruction#"
env
which

# Setup job settings and run it 
#Reco_tf.py --digiSteeringConf 'StandardSignalOnlyTruth' --conditionsTag 'default:OFLCOND-MC16-SDR-25' --valid 'True' --pileupFinalBunch '6' --numberOfHighPtMinBias '0.2595392' --autoConfiguration 'everything' --numberOfLowPtMinBias '99.2404608' --steering 'doRDO_TRIG' --preInclude 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInlcude.PileUpBunchTrainsMC16c_2017_Config1.py,RunDependentSimData/configLumi_run310000.py' --postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' --postInclude 'default:PyJobTransforms/UseFrontier.py' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False);from InDetRecExample.InDetJobProperties import InDetFlags; InDetFlags.doLowPt.set_Value_and_Lock(True);' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' --geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' \
Reco_tf.py --digiSteeringConf 'StandardSignalOnlyTruth' --conditionsTag 'default:OFLCOND-MC16-SDR-25' --valid 'True' --pileupFinalBunch '6' --numberOfHighPtMinBias '0.2595392' --autoConfiguration 'everything' --numberOfLowPtMinBias '99.2404608' --steering 'doRDO_TRIG' --preInclude 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInlcude.PileUpBunchTrainsMC16c_2017_Config1.py,RunDependentSimData/configLumi_run310000.py' --postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' --postInclude 'default:PyJobTransforms/UseFrontier.py' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False);' 'RAWtoESD:from InDetRecExample.InDetJobProperties import InDetFlags; InDetFlags.doLowPt.set_Value_and_Lock(True);' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' --geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' --maxEvents 10 \
--inputHITSFile $SAMPLE \
--outputAODFile AOD.batchcoritest.pool.root


#echo copying

#cp AOD.${SLURM_ARRAY_TASK_ID}.pool.root $OUTDIR

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
