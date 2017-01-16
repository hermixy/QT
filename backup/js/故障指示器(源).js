var obj = new Object;

obj.fSrc = function(mItem)
{
var mValue = mItem.fGetValue();

if(mValue > 0)
{
	mItem.fSendLinkage(true);
} 
else
{
	mItem.fSendLinkage(false);
}

}

result = obj;