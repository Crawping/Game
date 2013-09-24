//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

vector<ParameterSet *> ParameterSet::mSets;					// all the parameter sets
vector<Parameter *> ParameterSet::sTemporaryParameterList;	// TEMP! real ones are stored in the set, this is a placeholder filled in during construction

int Parameter::sNextParameterIndex = 0;

//////////////////////////////////////////////////////////////////////

void ParameterSet::CopyParameterSet(char const *name)
{
	mName = name;
	mExpanded = true;
	mSets.push_back(this);
	mParameters = sTemporaryParameterList;	// Yoink!
	sTemporaryParameterList.clear();
}

//////////////////////////////////////////////////////////////////////

string Parameter::ToXML() const
{
	return Format("\t\t<Parameter Name=\"%s\" Value=\"%f\" MinValue=\"%f\" MaxValue=\"%f\" DefaultValue=\"%f\"/>\n",
		mName.c_str(), mValue, mMinValue, mMaxValue, mDefaultValue);
}

//////////////////////////////////////////////////////////////////////

string ParameterSet::ToXML() const
{
	string s = Format("\t<ParameterSet Name=\"%s\" ParameterCount=\"%d\" Expanded=\"%d\">\n", mName.c_str(), mParameters.size(), mExpanded ? 1 : 0);
	for(auto p:mParameters)
	{
		s += p->ToXML();
	}
	s += "\t</ParameterSet>\n";
	return s;
}

//////////////////////////////////////////////////////////////////////

void ParameterSet::FromXML(XmlDocument *xmlDocument)
{
	XmlNode *root = xmlDocument->first_node("xml");
	if(root != null)
	{
		XmlNode *node = root->first_node("ParameterSet");
		while(node != null)
		{
			XmlAttribute *name = node->first_attribute("Name");
			XmlAttribute *count = node->first_attribute("ParameterCount");
			XmlAttribute *expanded = node->first_attribute("Expanded");
			if(name != null && count != null)
			{
				if(strcmp(mName.c_str(), name->value()) == 0)
				{
					if(expanded != null)
					{
						mExpanded = atoi(expanded->value()) != 0;
					}
					int paramCount = atoi(count->value());
					if(paramCount > 0)
					{
						int found = 0;
						XmlNode *param = node->first_node("Parameter");
						while(param != null)
						{
							XmlAttribute *name = param->first_attribute("Name");
							XmlAttribute *value = param->first_attribute("Value");
							XmlAttribute *minValue = param->first_attribute("MinValue");
							XmlAttribute *maxValue = param->first_attribute("MaxValue");
							XmlAttribute *defaultValue = param->first_attribute("DefaultValue");

							if(name != null && value != null && minValue != null && maxValue != null && defaultValue != null)
							{
								for(auto p: mParameters)
								{
									if(p->mName.compare(name->value()) == 0)
									{
										p->mValue = (float)atof(value->value());
										p->mMinValue = (float)atof(minValue->value());
										p->mMaxValue = (float)atof(maxValue->value());
										p->mDefaultValue = (float)atof(defaultValue->value());
										break;
									}
								}
								++found;
								if(found > paramCount)
								{
									// error
								}
							}
							param = param->next_sibling();
						}
					}
					break;
				}
			}
			node = node->next_sibling();
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ParameterSetCollection::Load()
{
	size_t size;
	void *file = LoadFile(mFilename.c_str(), &size);
	if(file != null)
	{
		XmlDocument xml;
		try
		{
			xml.parse<0>((char *)file);
			for(auto set: mSets)
			{
				set->FromXML(&xml);
			}
		}
		catch(rapidxml::parse_error e)
		{
			TRACE("Error parsing XML: %s\n", e.what());
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ParameterSetCollection::Save()
{
	string s("<xml>\n");
	for(auto set: mSets)
	{
		s += set->ToXML();
	}
	s += "</xml>";
	SaveFile(mFilename.c_str(), s.c_str(), s.size());
}