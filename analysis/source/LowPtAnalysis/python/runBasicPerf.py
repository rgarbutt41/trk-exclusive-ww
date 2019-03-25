#!/usr/bin/env python

# Read the submission directory as a command line argument. You can
# extend the list of arguments with your private ones later on.
import optparse
parser = optparse.OptionParser()
parser.add_option( '-s', '--submission-dir', dest = 'submission_dir',
                   action = 'store', type = 'string', default = 'submitDir',
                   help = 'Submission directory for EventLoop' )
( options, args ) = parser.parse_args()

# Set up (Py)ROOT.
import ROOT
ROOT.xAOD.Init().ignore()

# Set up the sample handler object. See comments from the C++ macro
# for the details about these lines.
import os
sh = ROOT.SH.SampleHandler()
sh.setMetaString( 'nc_tree', 'CollectionTree' )
#inputFilePath = os.getenv( 'ALRB_TutorialData' ) + '/r9315/'
#inputFilePath = '/global/projecta/projectdirs/atlas/spgriso/code/trk-exclusive-ww-3/run'
inputFilePath = '/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/trk-exclusive-ww-athena/run/Mar19_arraytest_1'
print inputFilePath
#ROOT.SH.ScanDir().filePattern( 'AOD.11182705._000001.pool.root.1' ).scan( sh, inputFilePath )
ROOT.SH.ScanDir().filePattern( 'AOD.3.pool.root' ).scan( sh, inputFilePath )
sh.printContent()

# Create an EventLoop job.
job = ROOT.EL.Job()
job.sampleHandler( sh )
job.options().setDouble( ROOT.EL.Job.optMaxEvents, 500 )

job.outputAdd (ROOT.EL.OutputStream ('ANALYSIS'))


# Create the algorithm's configuration.
from AnaAlgorithm.DualUseConfig import createAlgorithm
alg = createAlgorithm ( 'BasicPerf', 'AnalysisAlg' )


# later on we'll add some configuration options for our algorithm that go here

# Add our algorithm to the job
job.algsAdd( alg )



# Run the job using the direct driver.
driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )
