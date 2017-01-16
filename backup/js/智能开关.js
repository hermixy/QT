var obj = new Object;

obj.play = function(mItem)
{
var mValue = mItem.fGetValue();

if(mValue == 0)
{
	mItem.fSetLineColor(0, 255, 0);
	mItem.fSetBackGroundColor(0, 255, 0);
}
else
{	
	mItem.fSetLineColor(255, 0, 0);
	mItem.fSetBackGroundColor(255, 0, 0);
}

mItem.fUpdate();
}

result = obj;