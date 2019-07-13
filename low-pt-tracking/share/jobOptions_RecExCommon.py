
# --- brem recovery
doBremRecovery   = True
# --- calo seeded brem
doCaloSeededBrem = True

#--------------------------------------------------------------
# Import config
#--------------------------------------------------------------
from AthenaCommon.AthenaCommonFlags import athenaCommonFlags
from RecExConfig.RecFlags import rec
from RecExConfig.RecAlgsFlags import recAlgs

#--------------------------------------------------------------
# Event related parameters and input files
#--------------------------------------------------------------

# Exclusive dilepton
#athenaCommonFlags.FilesInput = ["/global/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/mc16_13TeV.363754.Herwig7EvtGen_BudnevQED_ggTOmumu_45M100_LeptonFilter.recon.RDO.e7175_e5984_s3126_r10201/RDO.16716257._000003.pool.root.1"]
# Inclusive WW no PileUp
#athenaCommonFlags.FilesInput = ["/global/project/projectdirs/atlas/adimitri/Samples_exclusiveWW/InclusiveWW_361600_fromHITS_noPileUp/file.RDO.2.pool.root"]
# ttbar FP-Truth
athenaCommonFlags.FilesInput = ["/global/projecta/projectdirs/atlas/spgriso/data/ExclWW/data/RDO/mc16_13TeV.410471.PhPy8EG_A14_ttbar_hdamp258p75_allhad.recon.RDO.e6337_s3126_r11193/RDO.16844823._000053.pool.root.1"]

# Sample options setup (override conditions, etc..) that fails autoconfiguration
rec.Commissioning.set_Value_and_Lock(True)
from AthenaCommon.BeamFlags import jobproperties as athenaBeamFlags
athenaBeamFlags.Beam.numberOfCollisions.set_Value_and_Lock(20.0);

#from AthenaCommon.GlobalFlags import jobproperties as athenaGlobalFlags
#athenaGlobalFlags.Global.DetDescrVersion.set_Value_and_Lock("ATLAS-R2-2016-01-00-01")
#athenaGlobalFlags.Global.ConditionsTag.set_Value_and_Lock("OFLCOND-MC16-SDR-20")
#athenaGlobalFlags.Global.DatabaseInstance.set_Value_and_Lock("auto")
#svcMgr += CfgMgr.DBReplicaSvc(UseCOOLSQLite=False)
#rec.RunNumber=310000

# --- number of events to process
athenaCommonFlags.EvtMax     =  10
athenaCommonFlags.SkipEvents =   0

# --- disable error protection of RecExCommon
athenaCommonFlags.AllowIgnoreConfigError.set_Value_and_Lock(False)


#--------------------------------------------------------------
# Additional Detector Setup
#--------------------------------------------------------------
import MagFieldServices.SetupField

from AthenaCommon.DetFlags import DetFlags 

# --- switch on InnerDetector
DetFlags.ID_setOn()

# --- turn off Calo
if doCaloSeededBrem:
  DetFlags.Calo_setOn()
else:
  DetFlags.Calo_setOff()

# --- and switch off Muons
DetFlags.Muon_setOff()

#--------------------------------------------------------------
# Control
#--------------------------------------------------------------

# --- Set output level threshold (2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )
rec.OutputLevel.set_Value_and_Lock       (INFO)

# --- controls what is written out. ESD includes AOD, so it's normally enough
# --- ESD writing temporarily disabled post 2013 migration - to be reinstated! 

from ParticleBuilderOptions.AODFlags import AODFlags
AODFlags.TrackParticleSlimmer=False 
AODFlags.TrackParticleLastHitAndPerigeeSlimmer=False

rec.doWriteESD.set_Value_and_Lock        (False)###
rec.doAOD.set_Value_and_Lock             (True)
rec.doWriteAOD.set_Value_and_Lock        (True)
rec.doDPD.set_Value_and_Lock             (False)
rec.doCBNT.set_Value_and_Lock            (False)
rec.doWriteTAG.set_Value_and_Lock        (False)

# --- turn on InDet
rec.doInDet.set_Value_and_Lock           (True)
# --- turn off calo
if not doCaloSeededBrem:
  rec.doCalo.set_Value_and_Lock           (False)
else:
  rec.doCalo.set_Value_and_Lock           (True)
# --- turn off muons
rec.doMuon.set_Value_and_Lock            (False) 
# --- turn off forward detectors
rec.doForwardDet.set_Value_and_Lock      (False)
# --- turn off trigger
rec.doTrigger.set_Value_and_Lock         (False)

# --- turn off combined reconstruction
rec.doEgamma.set_Value_and_Lock          (False)
rec.doMuonCombined.set_Value_and_Lock    (False)
rec.doTau.set_Value_and_Lock             (False)
rec.doJetMissingETTag.set_Value_and_Lock (False)

# --- turn of calo stuff we don't need anyway
from CaloRec.CaloRecFlags import jobproperties
jobproperties.CaloRecFlags.doCaloEMTopoCluster.set_Value_and_Lock(False)
jobproperties.CaloRecFlags.doCaloTopoTower.set_Value_and_Lock    (False)

# --- turn of jets (Hack!!!)
from JetRec.JetRecFlags import jetFlags
jetFlags.Enabled.set_Value_and_Lock          (False)

# --- turn off egamma Brem
recAlgs.doEgammaBremReco.set_Value_and_Lock  (False)
# --- turn off Eflow and missing ET
recAlgs.doEFlow.set_Value_and_Lock           (False)
recAlgs.doEFlowJet.set_Value_and_Lock        (False)
recAlgs.doMissingET.set_Value_and_Lock       (False)
recAlgs.doMissingETSig.set_Value_and_Lock    (False)
recAlgs.doObjMissingET.set_Value_and_Lock    (False)
# --- turn off combined muons
recAlgs.doMuGirl.set_Value_and_Lock          (False)
recAlgs.doMuTag.set_Value_and_Lock           (False)
recAlgs.doMuidLowPt.set_Value_and_Lock       (False)
recAlgs.doMuonIDCombined.set_Value_and_Lock  (False)
recAlgs.doMuonIDStandAlone.set_Value_and_Lock(False)
recAlgs.doMuonSpShower.set_Value_and_Lock    (False)
recAlgs.doStaco.set_Value_and_Lock           (False)
recAlgs.doCaloTrkMuId.set_Value_and_Lock     (False)
recAlgs.doTileMuID.set_Value_and_Lock        (False)
# --- trigger
recAlgs.doTrigger.set_Value_and_Lock         (False)

recAlgs.doTrackParticleCellAssociation.set_Value_and_Lock(False)

#--------------------------------------------------------------
# Printout config
#--------------------------------------------------------------

from AthenaCommon.GlobalFlags import globalflags
print "globalflags configuration:"
print globalflags

print "detflags configuration:"
DetFlags.Print()

print "rec configuration:"
print rec

#--------------------------------------------------------------
# Load InDet configuration
#--------------------------------------------------------------

# --- setup InDetJobProperties (default)
from InDetRecExample.InDetJobProperties import InDetFlags
InDetFlags.doTruth.set_Value_and_Lock (globalflags.DataSource == 'geant4' and globalflags.InputFormat() == 'pool')

# --- low-pT tracking for exclusive WW
#InDetFlags.doLowPt.set_Value_and_Lock(True)
InDetFlags.doLowPtRoI.set_Value_and_Lock(True)

# --- enable brem recovery
# InDetFlags.doBremRecovery.set_Value_and_Lock                       (doBremRecovery)
# --- calo seeding does not work standalone 
# InDetFlags.doCaloSeededBrem.set_Value_and_Lock                     (doCaloSeededBrem)

# --- cut level
#InDetFlags.priVtxCutLevel.set_Value_and_Lock                       (3)

# --- enable forward tracks
# InDetFlags.doForwardTracks.set_Value_and_Lock                      (True)

# --- enable low mu run setup
# InDetFlags.doLowMuRunSetup.set_Value_and_Lock                      (True)

# --- activate monitorings
InDetFlags.doMonitoringGlobal.set_Value_and_Lock                   (False)
InDetFlags.doMonitoringPrimaryVertexingEnhanced.set_Value_and_Lock (False)
InDetFlags.doMonitoringPixel.set_Value_and_Lock                    (False)
InDetFlags.doMonitoringSCT.set_Value_and_Lock                      (False)
InDetFlags.doMonitoringTRT.set_Value_and_Lock                      (False)
InDetFlags.doMonitoringAlignment.set_Value_and_Lock                (False)

# --- Make sure new dynamic folder scheme is locked to false for now
from AtlasGeoModel.InDetGMJobProperties import GeometryFlags;
GeometryFlags.useDynamicAlignFolders.set_Value_and_Lock            (not (globalflags.DataSource == 'geant4'))

# activate the print InDetXYZAlgorithm statements
InDetFlags.doPrintConfigurables.set_Value_and_Lock                 (True)

# Activate TIDE Ambi
#InDetFlags.doTIDE_Ambi.set_Value_and_Lock                          (True)

# --- test new cut level
# InDetFlags.cutLevel.set_Value_and_Lock                             (11)


# IMPORTANT NOTE: initialization of the flags and locking them is done in InDetRec_jobOptions.py!
# This way RecExCommon just needs to import the properties without doing anything else!
# DO NOT SET JOBPROPERTIES AFTER THIS LINE! The change will be ignored!

#--------------------------------------------------------------
# load master joboptions file
#--------------------------------------------------------------

include ("RecExCommon/RecExCommon_topOptions.py")


