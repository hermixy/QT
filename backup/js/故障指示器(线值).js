var obj = new Object;

obj.fPlay = function(mItem)
{
var mValue = mItem.fGetValue();

if(mValue > 0)
{
	mItem.fSetLineColor(255, 0, 0);
} 
else if(mValue <= 0)
{
	mItem.fSetLineColor(0, 255, 0);
}

mItem.fUpdate();
}

result = obj;