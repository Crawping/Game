//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class IFile
{
protected:

	virtual ~IFile()
	{
	}

	virtual bool IsValid() const = 0;
	virtual long Size() = 0;
	virtual long Position() = 0;
	virtual void Seek(long offset, int mode = SEEK_SET) = 0;
	virtual long Read(void *p, long s) = 0;
	virtual long Write(void const *p, long s) = 0;
};

//////////////////////////////////////////////////////////////////////

class File : IFile
{
public:

	//////////////////////////////////////////////////////////////////////

	enum AccessMode
	{
		AccessRead,
		AccessWrite,
		AccessAppend
	};

	//////////////////////////////////////////////////////////////////////

	File(TCHAR const *name, AccessMode AccessMode)
	{
		TCHAR const *access = null;
		switch(AccessMode)
		{
			case AccessRead:
				access = TEXT("rb");
				break;

			case AccessWrite:
				access = TEXT("wb");
				break;

			case AccessAppend:
				access = TEXT("ab");
				break;
		}
		if(access != null)
		{
			mHandle = _wfsopen(name, access, _SH_DENYWR);
			if(!IsValid())
			{
				MessageBox(null, Format(TEXT("Error %d opening file %s (%s)"), errno, name, access).c_str(), TEXT("File Error"), MB_ICONEXCLAMATION);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////

	bool IsValid() const
	{
		return mHandle != null;
	}

	//////////////////////////////////////////////////////////////////////

	long Size()
	{
		long len = 0;
		if(IsValid())
		{
			Seek(0, SEEK_END);
			len = Position();
			Seek(0);
		}
		return len;
	}

	//////////////////////////////////////////////////////////////////////

	long Position()
	{
		if(IsValid())
		{
			return ftell(mHandle);
		}
		else
		{
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////

	void Seek(long offset, int mode = SEEK_SET)
	{
		if(IsValid())
		{
			fseek(mHandle, (long)offset, mode);
		}
	}

	//////////////////////////////////////////////////////////////////////

	long Read(void *p, long s)
	{
		if(IsValid())
		{
			return fread_s(p, s, sizeof(byte), s, mHandle);
		}
		else
		{
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////

	long Write(void const *p, long s)
	{
		if(IsValid())
		{
			return fwrite(p, s, sizeof(byte), mHandle);
		}
		else
		{
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////

	~File()
	{
		if(IsValid())
		{
			fclose(mHandle);
			mHandle = null;
		}
	}

	//////////////////////////////////////////////////////////////////////

private:

	FILE *mHandle;
};
