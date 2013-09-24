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

	string ToXMLString() const;

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

	ParameterSet()
	{
	}

	void CopyParameterSet(char const *name)
	{
		mSets.push_back(this);
		mExpanded = true;
		mName = name;
		mParameters = sTemporaryParameterList;	// Yoink!
		sTemporaryParameterList.clear();
	}

	string ToXMLString() const
	{
		string s = Format("\t<ParameterSet Name=\"%s\" ParameterCount=\"%d\">\n", mName.c_str(), mParameters.size());
		for(auto p:mParameters)
		{
			s += p->ToXMLString();
		}
		s += "\t</ParameterSet>\n";
		return s;
	}

	void SetFromXML(XmlDocument *xmlDocument, char const *name);

	void Load(char const *filename);
	void Save(char const *filename);
};

//////////////////////////////////////////////////////////////////////

#define BEGIN_PARAMSET(pname)										\
																	\
struct pname : ParameterSet											\
{																	\
	pname(char const *n) : ParameterSet()							\
	{																\
		CopyParameterSet(n);										\
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

