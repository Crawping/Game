//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

vector<ParameterSet *> ParameterSet::mSets;			// all the parameter sets
vector<Parameter *> ParameterSet::sTemporaryParameterList;		// TEMP! real ones are stored in the set, this is a placeholder filled in during construction
int Parameter::sNextParameterIndex = 0;

//////////////////////////////////////////////////////////////////////

string Parameter::ToXMLString() const
{
	return Format("\t\t<Parameter Name=\"%s\" Value=\"%f\" MinValue=\"%f\" MaxValue=\"%f\" DefaultValue=\"%f\"/>\n",
		mName.c_str(), mValue, mMinValue, mMaxValue, mDefaultValue);
}

//////////////////////////////////////////////////////////////////////

void ParameterSet::SetFromXML(XmlDocument *xmlDocument, char const *pname)
{
	XmlNode *root = xmlDocument->first_node("xml");
	if(root != null)
	{
		XmlNode *node = root->first_node("ParameterSet");
		while(node != null)
		{
			XmlAttribute *name = node->first_attribute("Name");
			XmlAttribute *count = node->first_attribute("ParameterCount");
			if(name != null && count != null)
			{
				if(strcmp(pname, name->value()) == 0)
				{
					mName = string(name->value());
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

