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
	virtual size_t Size() = 0;
	virtual size_t Position() = 0;
	virtual void Seek(size_t offset, int mode = SEEK_SET) = 0;
	virtual size_t Read(void *p, size_t s) = 0;
	virtual size_t Write(void const *p, size_t s) = 0;
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

	size_t Size()
	{
		size_t len = 0;
		if(IsValid())
		{
			Seek(0, SEEK_END);
			len = Position();
			Seek(0);
		}
		return len;
	}

	//////////////////////////////////////////////////////////////////////

	size_t Position()
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

	void Seek(size_t offset, int mode = SEEK_SET)
	{
		if(IsValid())
		{
			_fseeki64(mHandle, (size_t)offset, mode);
		}
	}

	//////////////////////////////////////////////////////////////////////

	size_t Read(void *p, size_t s)
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

	size_t Write(void const *p, size_t s)
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
