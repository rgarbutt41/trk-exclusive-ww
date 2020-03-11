#!/bin/bash
mkdir Add Nom Low
for k in Add Nom Low;
do
    cd $k
    mkdir DY Ztautau Zmumu InclWW ExclWW
    cd InclWW
    mkdir Individual_Samples
    cd ../
    for num in {50..500..50};
    do
	for i in DY Ztautau Zmumu InclWW ExclWW;
#for i in Ztautau ExclWW;
	do
	    if [ "$i" = "DY" ];
	    then
		cd $i
		echo $num
		touch Ratios.txt
		#sh /global/projecta/projectdirs/atlas/rgarbutt/AnalysisCode/analysis/run/Macros/TruthAnalysis.sh $num DYmumu
		cd ../
	    else
		cd $i
		touch Ratios.txt
	    #sh /global/projecta/projectdirs/atlas/rgarbutt/AnalysisCode/analysis/run/Macros/TruthAnalysis.sh $num $i
	    cd ../
	    fi
	done
    done
    cd ../
done