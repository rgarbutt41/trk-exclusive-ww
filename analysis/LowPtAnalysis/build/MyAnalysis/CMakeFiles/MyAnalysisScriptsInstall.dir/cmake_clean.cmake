file(REMOVE_RECURSE
  "../x86_64-slc6-gcc62-opt/include/MyAnalysis"
  "../x86_64-slc6-gcc62-opt/bin/ATestRun_eljob.py"
  "../x86_64-slc6-gcc62-opt/bin/ATestRun_eljob.py"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/MyAnalysisScriptsInstall.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
