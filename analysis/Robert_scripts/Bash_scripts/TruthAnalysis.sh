#Run the TruthAnalysis python script on the specified sample
#Takes the min pt value to be analyzed
#Produces folders with name excl/incl_(value)_min_pt
#Input name of interaction as follows: inclWW, DYmumu
 
#!/bin/bash
if [ $2 == "InclWW" ]; then
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/adimitri/Samples_InclusiveWW/user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0a_EXT0/ -s ./Individual_Samples/incl_$1_min_pt-1 --lowpt $1
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/adimitri/Samples_InclusiveWW/user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0d_EXT0/ -s ./Individual_Samples/incl_$1_min_pt-2 --lowpt $1
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/adimitri/Samples_InclusiveWW/user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0e_EXT0/ -s ./Individual_Samples/incl_$1_min_pt-3 --lowpt $1
   hadd incl_$1_min_pt.root ./Individual_Samples/incl_$1_min_pt-1/hist-user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0a_EXT0.root ./Individual_Samples/incl_$1_min_pt-2/hist-user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0d_EXT0.root ./Individual_Samples/incl_$1_min_pt-3/hist-user.adimitri.mc15_13TeV.600009.PhPy8EG_CT10nloME_AZnloC6L1_WWlvlv_LepCTFilter11.TRUTH0e_EXT0.root
elif [ $2 == "DYmumu" ]; then
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/spgriso/data/ExclWW/data/DAOD_TRUTH0/user.spagan.mc16_13TeV.363282.DYmumu_110M_CTFilter11.private.DAOD_TRUTH0.e7744_e5984.v1_EXT0 -s DYmumu_$1_min_pt --lowpt $1
elif [ $2 == "Ztautau" ]; then
   runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/spgriso/data/ExclWW/data/DAOD_TRUTH0/user.spagan.mc16_13TeV.3632812.Ztautau_TauFl25l18_CTF11.private.DAOD_TRUTH0.e7744_e5984.v1_EXT0 -s Ztautau_$1_min_pt --lowpt $1
elif [ $2 == "Zmumu" ]; then
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/spgriso/data/ExclWW/data/DAOD_TRUTH0/user.spagan.mc16_13TeV.363280.Zmumu_60M110_CTFilter11.private.DAOD_TRUTH0.e7744_e5984.v1_EXT0 -s Zmumu_$1_min_pt --lowpt $1
elif [ $2 == "DYee" ]; then
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/spgriso/data/ExclWW/data/DAOD_TRUTH0/ -s DYee_$1_min_pt --lowpt $1
elif [ $2 == "DYtautau" ]; then 
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/spgriso/data/ExclWW/data/DAOD_TRUTH0/user.spagan.mc16_13TeV.3632812.Ztautau_TauFl25l18_CTF11.private.DAOD_TRUTH0.e7744_e5984.v1_EXT0 -s Ztautau_$1_min_pt --lowpt $1
elif [ $2 == "ExclWW" ]; then
    runTruthAnalysis_eljob.py -i  /global/cfs/projectdirs/atlas/adimitri/Samples_InclusiveWW/user.adimitri.364819.Herwig7EvtGen_BudnevQED_WWlvlv_LeptonFilter.evgen.DAOD_TRUTH0.e7829.v1_EXT0 -s excl_$1_min_pt --lowpt $1
elif [ $2 == "LowMassDY" ]; then
    runTruthAnalysis_eljob.py -i /global/cfs/projectdirs/atlas/adimitri/Samples_LowMassDY/Truth0/ -s LowMassDY_$1_min_pt --lowpt $1
fi

    