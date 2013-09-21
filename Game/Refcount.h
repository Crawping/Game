//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class RefCount
{
public:

	RefCount() : mRefCount(1)
	{
	}

	virtual ~RefCount()
	{
	}

	void AddRef()
	{
		++mRefCount;
	}

	int Release()
	{
		int rc = mRefCount--;
		if(mRefCount == 0)
		{
			delete this;
		}
		return rc;
	}

private:

	int mRefCount;
};