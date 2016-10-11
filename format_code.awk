

BEGIN{
#定义缩进变量
intend=""
}


#删除所有行首的空白字符
function delete_space()
{
	sub(/^([ \t]+)+/,"")
}

{
	#处理每一行之前先将前面的空白全部删除
	delete_space()
	#处理长段注释情况，直接无缩进打印
	if($0~/^\*/ || $0~/^\/\*/)
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




