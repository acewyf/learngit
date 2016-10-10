

BEGIN{
#定义缩进变量
intend=""
#定义行首
top_of_line=""
}

function delete_space_bak()
{
	#行首第一个字符赋给top_of_line
	top_of_line=substr($0,1,1)
	#如果第一个字符是空格或者tab，就将其删除，循环操作直至第一个字符不是空格或tab
	while(top_of_line==" " || top_of_line=="\t")
	{
		#将新的行首赋给top_of_line
		top_of_line=substr($0,1,1)
		#sub为替换函数，将tab替换为空，即删除tab
		sub(/^[\t]+/,"")
		#同上，删除空格
		sub(/^ +/,"")
	}
}

function delete_space()
{
	sub(/^([ \t]+)+/,"")
}

{
	#处理每一行之前先将前面的空白全部删除
	delete_space()
	#处理长段注释情况，直接无缩进打印
	if($0~/^*/ || $0~/^\/\*/)
	{
		print
	}
	
	#处理//注释情况
	else if($0~/^\/\//)
	{
		#如果//和后面内容之间有空格，直接无缩进打印
		if($1=="//")
		{			
			print
		}
		#如果//后紧跟内容，打印对应个数tab
		else
		{
			print intend$0
		}
	}

	#处理大括号情况
	else if($0~/{/ || $0~/}/)
	{
		#匹配右大括号，不匹配左大括号，缩进变量减少一个tab，打印对应tab加内容
		#匹配左右大括号，匹配字符串else，缩进变量减少一个tab，打印对应tab加内容	
		if($0~/}/)
		{
			if($0!~/{/ || $0~/else/)
			{
				intend=substr(intend,2)
				print intend$0
			}
		}
		
		#匹配左大括号，且本行没有右大括号，先打印，缩进变量增加一个tab
		#同时匹配左右大括号，且匹配字符串else，那么缩进变量增加一个tab（打印在上面匹配右大括号已做过）
		#同时匹配左右大括号但没有匹配字符串else，打印对应tab加内容		
	 	if($0~/{/)
		{
			if($0!~/}/)
			{
				print intend$0
				intend=intend"\t"
			}
			else if($0~/}/ && $0~/else/)
			{
				intend=intend"\t"
			}
			else
			{
				print intend$0
			}
		}

	}
	
	#其他情况，即普通代码和注释，打印对应tab加内容	
	else
	{
		print intend$0
	}
}




