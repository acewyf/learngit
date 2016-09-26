BEGIN
{
intend=""
left_bkt="{"
long_node="/*"
short_node="*"
}

{
#	if($0~/\$short_node/)
	if($0~/\*/)
	{
		print
	}
	else if($0~/{/)
	{
		print "\t"$0
	}
	else
	{
		print "aaa"$0
	}
}
