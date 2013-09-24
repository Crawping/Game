//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

struct Parameter
{
	static int sNextParameterIndex;

	string mName;
	float mValue;
	float mMinValue;
	float mMaxValue;
	float mDefaultValue;
	uint32 mIndex;

	Parameter()
		: mIndex(++sNextParameterIndex)
	{
	}

	Parameter(char const *name, float val, float minVal, float maxVal)
		: mName(name)
		, mValue(val)
		, mMinValue(minVal)
		, mMaxValue(maxVal)
		, mDefaultValue(val)
		, mIndex(++sNextParameterIndex)
	{
	}

	string ToXML() const;

	operator float()
	{
		return mValue;
	}

	float operator *= (float f)
	{
		return mValue *= f;
	}

	float operator = (float const &f)
	{
		return mValue = f;
	}
};

//////////////////////////////////////////////////////////////////////

struct ParameterSet
{
	static vector<ParameterSet *> mSets;
	static vector<Parameter *> sTemporaryParameterList;

	vector<Parameter *> mParameters;
	string mName;
	bool mExpanded;

	void CopyParameterSet(char const *name);
	string ToXML() const;
	void FromXML(XmlDocument *xmlDocument);
};

//////////////////////////////////////////////////////////////////////

struct ParameterSetCollection
{
	vector<ParameterSet *>	mSets;
	tstring					mFilename;

	void Load();
	void Save();

	ParameterSetCollection(TCHAR const *filename)
		: mFilename(filename)
	{
	}

	void AddParameterSet(ParameterSet *p)
	{
		mSets.push_back(p);
	}
};

//////////////////////////////////////////////////////////////////////

#define BEGIN_PARAMSET(pname, displayName)							\
																	\
struct pname : ParameterSet											\
{																	\
	pname(char const *name)											\
	{																\
		CopyParameterSet(name);										\
	}																\
	pname()															\
	{																\
		CopyParameterSet(displayName);								\
	}

//////////////////////////////////////////////////////////////////////

#define PARAM(name, d, mn, mx)										\
																	\
struct s_##name : Parameter											\
{																	\
	s_##name() : Parameter(#name, float(d), float(mn), float(mx))	\
	{																\
		sTemporaryParameterList.push_back(this);					\
	}																\
};																	\
s_##name name

//////////////////////////////////////////////////////////////////////

#define END_PARAMSET()												\
};

