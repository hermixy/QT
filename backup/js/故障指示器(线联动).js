var obj = new Object;

obj.fDst = function(mItem)
{
	if(mItem.fGetLinkageRecv() == mItem.fGetLinkageActive())
	{
		mItem.fSetValue(1);
		mItem.fStartAuto();
	}
	else
	{
		mItem.fSetValue(0);
	}
	
	mItem.fUpdate();
}

result = obj;