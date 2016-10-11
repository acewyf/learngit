#=====================================================
#	USAGE:	 ./pretty_code src_dir 
#	CREATED: 09/28/2016 18:08
#
#=====================================================
homedir=`basename $1`
for file in `find $1 -type f`
do
	filename=`basename $file`
	pathname=`dirname $file`
	#�½�һ�����Ŵ���Ŀ¼ƽ����formatĿ¼������Ŀ¼�ṹ����ʽ����Ĵ������δ���
	newpathname=`echo $pathname | awk 'sub(/'$homedir'/,"format"){print}'` 
	code_jug=`echo "$filename" | grep -E ".cpp$|.hpp$|.pc$" | wc -l`
	if [ $code_jug == 1 ]
	then
		if [ ! -d $newpathname ]
		then
			mkdir -p $newpathname
		fi
		awk -f format_code.awk $file > $newpathname/$filename
	fi
done
