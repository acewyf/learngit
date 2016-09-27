
BEGIN{
intend=""
top_of_line=""
}

function delete_space()
{
	top_of_line=substr($0,1,1)
	while(top_of_line==" " || top_of_line=="\t")
	{
		sub("^[\t]+","")
		sub("^ +","")
		top_of_line=substr($0,1,1)
	}
}	

{
	delete_space()
	if($0~/^\*/ || $0~/^\/\*/)
	{
		print
	}
	else if($0~/{/ || $0~/}/)
	{
		if($0~/}/)
		{
			if($0!~/{/ || $0~/else/)
			{
				intend=substr(intend,2)
				print intend$0
			}
		}
		if($0~/{/)
		{
			if($0!~/}/) 
			{
				print intend$0
				intend=intend"\t"
			}
			else if($0~/else/)	
			{
				intend=intend"\t"
			}
			else
			{
				print intend$0
			}
		}
	}
	
	else
	{
		print intend$0 
	}
}
