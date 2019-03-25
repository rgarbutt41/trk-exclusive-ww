#
# WARNING:
# Execution driver. Submit the submission script ending with .slr
#
##################################################
# Settings
##################################################

ASetupStr='Athena,21.0.77,here'

BASEDIR="/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/trk-exclusive-ww-athena/run"

SUBDIR="Mar19_test2_reco"


##################################################
# END of Settings
##################################################

#Setup ATLAS software on PDSF
shopt -s expand_aliases
# source /common/atlas/scripts/setupATLAS.sh
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
setupATLAS
lsetup asetup

echo "Setting up ATLAS software $ASetupStr in $BASEDIR/"
cd $BASEDIR/
echo asetup ${ASetupStr}
asetup ${ASetupStr}


echo "------ ENVIRONMENT ---------"
printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

CPYDIR=$BASEDIR/$SUBDIR
echo "Output: ${CPYDIR}"
mkdir -pv ${CPYDIR}


#cd $TMPDIR
WRKDIR=$PWD
echo "Work dir: ${PWD}"

cd $CPYDIR

pwd

# Setup job settings and run it 
Reco_tf.py --digiSteeringConf 'StandardSignalOnlyTruth' --conditionsTag 'default:OFLCOND-MC16-SDR-25' --valid 'True' --pileupFinalBunch '6' --numberOfHighPtMinBias '0.2595392' --autoConfiguration 'everything' --numberOfLowPtMinBias '99.2404608' --steering 'doRDO_TRIG' --preInclude 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInlcude.PileUpBunchTrainsMC16c_2017_Config1.py,RunDependentSimData/configLumi_run310000.py' --postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' --postInclude 'default:PyJobTransforms/UseFrontier.py' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True); from LArDigitization.LArDigitizationFlags import jobproperties;jobproperties.LArDigitizationFlags.useEmecIwHighGain.set_Value_and_Lock(False)' 'ESDtoAOD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.AODEDMSet.set_Value_and_Lock("AODSLIM");' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2216,76,260' --geometryVersion 'default:ATLAS-R2-2016-01-00-01' --numberOfCavernBkg '0' \
--inputRDOFile '/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.363754.Herwig7EvtGen_BudnevQED_ggTOmumu_45M100_LeptonFilter.recon.RDO.e7175_e5984_s3126_r10201/RDO.16716257._000002.pool.root.1' \
--outputAODFile 'AOD.pool.root' \
--maxEvents 20 


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