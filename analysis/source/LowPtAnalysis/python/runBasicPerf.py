#!/usr/bin/env python

# Read the submission directory as a command line argument. You can
# extend the list of arguments with your private ones later on.
import optparse
parser = optparse.OptionParser()
parser.add_option( '-s', '--submission-dir', dest = 'submission_dir',
                   action = 'store', type = 'string', default = 'submitDir',
                   help = 'Submission directory for EventLoop' )
parser.add_option( '-i', '--input-dir', dest = 'input_dir',
                   action = 'store', type = 'string', default = '/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/trk-exclusive-ww-athena/run/Mar19_arraytest_1',
                   help = 'Input directory with xAOD files' )
parser.add_option('-d', '--debug', dest = 'debug',
                  action = 'store', type='int', default=0,
                  help = "Enable DEBUG printout: 0=INFO, 1=DEBUG, 2=VERBOSE")
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
job.options().setDouble( ROOT.EL.Job.optMaxEvents, 15000 )
job.outputAdd (ROOT.EL.OutputStream ('ANALYSIS'))


# Create the algorithm's configuration.
from AnaAlgorithm.DualUseConfig import createAlgorithm
alg = createAlgorithm ( 'BasicPerf', 'AnalysisAlg')

# later on we'll add some configuration options for our algorithm that go here
if (options.debug == 1):    
    alg.OutputLevel = ROOT.MSG.DEBUG
elif (options.debug == 2):    
    alg.OutputLevel = ROOT.MSG.VERBOSE

# Add our algorithm to the job
job.algsAdd( alg )



# Run the job using the direct driver.
driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )
