#!/bin/bash
mkdir Add Nom Low
for k in  Nom; #Add Low;
do
    cd $k
    mkdir InclWW ExclWW #DY Ztautau Zmumu InclWW ExclWW
    cd InclWW
    mkdir Individual_Samples
    cd ../
    for num in {100..500..50};
    do
	for i in InclWW ExclWW #DY Ztautau Zmumu InclWW ExclWW;
	do
	    if [ "$i" = "DY" ];
	    then
		cd $i
		echo $num
		touch Ratios.txt
		sh /global/cfs/projectdirs/atlas/rgarbutt/trk-exclusive-ww/analysis/Robert_scripts/Bash_scripts/TruthAnalysis.sh $num DYmumu
		cd ../
	    else
		cd $i
		touch Ratios.txt
	    sh /global/cfs/projectdirs/atlas/rgarbutt/trk-exclusive-ww/analysis/Robert_scripts/Bash_scripts/TruthAnalysis.sh $num $i
	    cd ../
	    fi
	done
    done
    cd ../
done