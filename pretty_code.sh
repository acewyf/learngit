<<<<<<< HEAD
#=====================================================
#	USAGE:	 ./pretty_code src_dir dest_dir
#	CREATED: 09/28/2016 18:08
#
#=====================================================
for file in `ls $1/*`
do
	filename=`echo "$file" | awk -F '/' '{print $NF}'`
	awk -f pretty_code.awk $file > $2/$filename
done
=======
awk -f pretty_code.awk $1
>>>>>>> 190490f3982ce3ff4ab3f1d8c01d462761a9748f
