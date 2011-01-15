#$1 FOLD_CNT
#$2 FOLD_NUM

gawk -f predict2tpos.awk -v FOLD_NUM=$2 -v FOLD_CNT=$1 "test_desc_libsvm$2-$1_2.txt.predict" > "predict_tpos_$2-$1_2.txt"

