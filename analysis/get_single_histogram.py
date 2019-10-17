import ROOT

filename = "/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/analysis/run/test_noROI/hist-links_file.root"
fi = ROOT.TFile(filename,"read")

theplot = fi.Get("Reco_eff_vs_track_pt_nearHS_andsmalld0")

outfile = ROOT.TFile("trk_eff_doLowPt.root","RECREATE")
theplot.SetName("h_trk_eff_pt")
theplot.Write()

#h_trk_eff_pt
