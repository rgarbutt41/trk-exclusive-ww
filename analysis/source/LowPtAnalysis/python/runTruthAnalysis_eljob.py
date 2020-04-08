#!/usr/bin/env python

# Read the submission directory as a command line argument. You can
# extend the list of arguments with your private ones later on.
import optparse
parser = optparse.OptionParser()
parser.add_option( '-s', '--submission-dir', dest = 'submission_dir',
                   action = 'store', type = 'string', default = 'submitDir',
                   help = 'Submission directory for EventLoop' )
parser.add_option( '-i', '--input-dir', dest = 'input_dir',
                   action = 'store', type = 'string', default = '/global/projecta/projectdirs/atlas/spgriso/data/ExclWW/data/EVNT/',
                   help = 'Input directory with xAOD files' )
parser.add_option('-d', '--debug', dest = 'debug',
                  action = 'store', type='int', default=0,
                  help = "Enable DEBUG printout: 0=INFO, 1=DEBUG, 2=VERBOSE")
parser.add_option('', '--maxn', dest = 'maxn',
                  action = 'store', type='int', default=0,
                  help = "Limit maximum number of events to run on")
parser.add_option('', '--lowpt', dest ='min_pt',
                   action = 'store', type='int',default=100,
                   help = "Low-pT tracking  value in MeV, default 100 MeV")
( options, args ) = parser.parse_args()

# Set up (Py)ROOT.
import ROOT
ROOT.xAOD.Init().ignore()

# Set up the sample handler object. See comments from the C++ macro
# for the details about these lines.
import os
sh = ROOT.SH.SampleHandler()
sh.setMetaString( 'nc_tree', 'CollectionTree' )
inputFilePath = options.input_dir
print("Input folder: %s" % inputFilePath)
ROOT.SH.ScanDir().filePattern( '*.pool.root*' ).scan( sh, inputFilePath )
sh.printContent()

# Create an EventLoop job.
job = ROOT.EL.Job()
job.sampleHandler( sh )
if (options.maxn > 0):
    job.options().setDouble( ROOT.EL.Job.optMaxEvents, options.maxn )
job.outputAdd (ROOT.EL.OutputStream ('ANALYSIS'))


# Create the algorithm's configuration.
from AnaAlgorithm.DualUseConfig import createAlgorithm
alg = createAlgorithm ( 'TruthAnalysis', 'TruthAnalysisAlg')

# later on we'll add some configuration options for our algorithm that go here
if (options.debug == 1):    
    alg.OutputLevel = ROOT.MSG.DEBUG
elif (options.debug == 2):    
    alg.OutputLevel = ROOT.MSG.VERBOSE

# Tweak settings, if necessary
#alg.input_trk_eff_file = "/global/homes/s/spgriso/code/AtlasExclWW/trk-exclusive-ww/analysis/data/trk_eff_pt.root" Pre_PU_Fakes
#alg.input_trk_eff_file = "/global/homes/s/spgriso/code/AtlasExclWW/trk-exclusive-ww/analysis/data/trk_eff_LowPtRoI.root" 
#alg.input_trk_eff_file = "/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/analysis/data/hist-all.root" 

#alg.input_trk_eff_file = "/global/homes/s/spgriso/code/AtlasExclWW/trk-exclusive-ww/analysis/data/trk_eff_pt.root" #Trk-eff of previous analysis

alg.input_trk_eff_file = "/global/cfs/projectdirs/atlas/wmccorma/ExclWW/trk-exclusive-ww/analysis/data/hist-lowpt_trkeff.root" #2D trk-eff

alg.input_trk_eff_pt_eta_file = "/global/cfs/projectdirs/atlas/wmccorma/ExclWW/Trk_eff_output/links_of_hists/hist-all_v3.root" #Track efficiency for TProfile2D



#alg.input_pu_file = "/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/analysis/data/ZPU.root" #Pileup file.
#alg.filter_by_selections=True
alg.tracks_min_pt=  options.min_pt # pT tracking value

alg.input_pu_file = "/global/cfs/projectdirs/atlas/wmccorma/ExclWW/Production_InclWW_Rebase_2_05_20/ExclWW_Analysis/PU_Fakes_withlowpt.root"

# Add our algorithm to the job
job.algsAdd( alg )

# Run the job using the direct driver.
driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )
