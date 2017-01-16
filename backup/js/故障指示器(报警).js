var obj = new Object;

obj.fAlarm = function(mItem)
{
var mValue = mItem.fGetValue();

if(mValue > 0)
{
	mItem.fStartAlarm();
}

}

result = obj;