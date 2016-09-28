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
