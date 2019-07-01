# The name of the package:
atlas_subdir (LowPtAnalysis)

# Add the shared library:
atlas_add_library (LowPtAnalysisLib
  LowPtAnalysis/*.h Root/*.cxx
  PUBLIC_HEADERS LowPtAnalysis
  LINK_LIBRARIES AnaAlgorithmLib xAODEventInfo xAODTau xAODMuon xAODEgamma xAODTruth xAODTracking)

if (XAOD_STANDALONE)
 # Add the dictionary (for AnalysisBase only):
 atlas_add_dictionary (LowPtAnalysisDict
  LowPtAnalysis/LowPtAnalysisDict.h
  LowPtAnalysis/selection.xml
  LINK_LIBRARIES LowPtAnalysisLib)
endif ()

if (NOT XAOD_STANDALONE)
  # Add a component library for AthAnalysis only:
  atlas_add_component (LowPtAnalysis
    src/components/*.cxx
    LINK_LIBRARIES LowPtAnalysisLib)
endif ()

# Install files from the package:
atlas_install_joboptions( share/*_jobOptions.py )
atlas_install_scripts( python/runBasicPerf.py )
atlas_install_scripts( python/runTruthAnalysis_eljob.py )
