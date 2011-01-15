rm ./total_desc_libsvm.txt
gawk -f raw2libsvm.awk -v CLASS=0 raw_particle_desc.txt > "total_desc_libsvm.txt"
gawk -f raw2libsvm.awk -v CLASS=1 raw_non-particle1_desc.txt >> "total_desc_libsvm.txt"
gawk -f raw2libsvm.awk -v CLASS=1 raw_non-particle2_desc.txt >> "total_desc_libsvm.txt"
gawk -f raw2libsvm.awk -v CLASS=1 raw_unclassified_desc.txt >> "total_desc_libsvm.txt"
