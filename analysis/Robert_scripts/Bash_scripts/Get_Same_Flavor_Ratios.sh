#!/bin/bash
for k in InclWW DY Zmumu Ztautau;
do
    cd $k
    rm Ratios.txt
    rm Errors.txt
    touch Ratios.txt
    touch Errors.txt
    pwd
    cd ../
done
for num in {100..500..50};
do
    for i in InclWW DY Zmumu Ztautau;
    do
	if [ "$i" = "DY" ];
	then
	    cd $i
	    pwd
	    sh /global/cfs/projectdirs/atlas/rgarbutt/trk-exclusive-ww/analysis/Robert_scripts/Bash_scripts/Get_Ratio.sh $num DYmumu $1
	    cd ../
	else
	    cd $i
	    pwd
	    sh /global/cfs/projectdirs/atlas/rgarbutt/trk-exclusive-ww/analysis/Robert_scripts/Bash_scripts/Get_Ratio.sh $num $i $1
	    cd ../
	fi
    done
done