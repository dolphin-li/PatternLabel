#include "PatternImageInfo.h"
#include "util.h"
PatternImageInfo::PatternImageInfo()
{
	setDefaultTypes();
}

PatternImageInfo::~PatternImageInfo()
{

}

void PatternImageInfo::clear()
{
	m_imgNames.clear();
	setDefaultTypes();
}

int PatternImageInfo::numImages()const
{
	return (int)m_imgNames.size();
}

std::string PatternImageInfo::getImageName(int i)const
{
	return m_imgNames.at(i);
}

void PatternImageInfo::clearImages()
{
	m_imgNames.clear();
}

void PatternImageInfo::addImage(const std::string& name)
{
	m_imgNames.push_back(name);
}

void PatternImageInfo::setBaseName(const std::string& name)
{
	m_baseName = name;
}

std::string PatternImageInfo::getBaseName()const
{
	return m_baseName;
}

void PatternImageInfo::setUrl(const std::string& url)
{
	m_url = url;
}

std::string PatternImageInfo::getUrl()const
{
	return m_url;
}

std::string PatternImageInfo::getAttributeType(const std::string& typeName)const
{
	int id = m_types.at(typeName);
	return s_typeSet.at(typeName).at(id);
}

void PatternImageInfo::setAttributeType(const std::string& typeName, const std::string& type)
{
	const auto& types = s_typeSet.at(typeName);
	auto iter = std::find(types.begin(), types.end(), type);
	if (iter == types.end())
		throw std::exception(("non-defined type: " + type).c_str());
	m_types.at(typeName) = int(iter - types.begin());
}

void PatternImageInfo::setDefaultTypes()
{
	m_types.clear();
	for (const auto& type : s_typeSet)
	{
		m_types.insert(std::make_pair(type.first, 0));
	}
}

void PatternImageInfo::toXml(TiXmlNode* parent)const
{
	std::string baseName = getBaseName();
	for (size_t i = 0; i < baseName.size(); i++)
	{
		if (baseName[i] == ' ' || baseName[i] == '(' || baseName[i] == ')'
			|| baseName[i] == '\'' || baseName[i] == '\"')
			baseName[i] = '-';
	}
	parent->SetValue(baseName.c_str());
	TiXmlElement* base = new TiXmlElement("baseName");
	parent->LinkEndChild(base);
	base->SetAttribute("value", m_baseName.c_str());
	TiXmlElement* url = new TiXmlElement("url");
	parent->LinkEndChild(url);
	url->SetAttribute("value", m_url.c_str());
	for (auto name : m_imgNames)
	{
		std::string p, n, e;
		ldp::fileparts(name, p, n, e);
		TiXmlElement* img = new TiXmlElement("image");
		parent->LinkEndChild(img);
		img->SetAttribute("value", (n + e).c_str());
	}
	for (const auto& iter : m_types)
	{
		TiXmlElement* ele = new TiXmlElement(iter.first.c_str());
		parent->LinkEndChild(ele);
		ele->SetAttribute("value", getAttributeType(iter.first).c_str());
	} // m_types
}

void PatternImageInfo::fromXml(std::string rootFolder, TiXmlElement* parent)
{
	clear();
	for (auto p_iter = parent->FirstChildElement(); p_iter; p_iter = p_iter->NextSiblingElement())
	{
		std::string name = p_iter->Value();
		std::string att = p_iter->Attribute("value");
		if (name == "url")
			m_url = att;
		else if (name == "baseName")
		{
			m_baseName = att;
		}
		else if (name == "image")
		{
			addImage(att);
		}
		else for (auto& type : m_types)
		{
			if (name == type.first)
			{
				auto& types = attributeTypes(type.first);
				auto iter = std::find(types.begin(), types.end(), att);
				if (iter == types.end())
					throw std::exception(("error: invalid type" + att).c_str());
				type.second = int(iter - types.begin());
			} // end if name
		} // end for type
	} // end for p_iter

	for (auto& img : m_imgNames)
	{
		img = ldp::fullfile(ldp::fullfile(rootFolder, m_baseName), img);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
std::map<std::string, std::vector<std::string>> PatternImageInfo::s_typeSet;
bool PatternImageInfo::s_mapInitialized = PatternImageInfo::constructTypeMaps();

bool PatternImageInfo::constructTypeMaps()
{
	TiXmlDocument doc;
	if(!doc.LoadFile("__attributes.xml"))
	{
		printf("error: __attributes.xml parsing error!\n");
		return false;
	}

	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		s_typeSet.insert(std::make_pair(doc_iter->Value(), std::vector<std::string>()));
		auto& tSet = s_typeSet[doc_iter->Value()];
		for (auto type_iter = doc_iter->FirstChildElement(); type_iter; type_iter = type_iter->NextSiblingElement())
		{
			if (type_iter->Value() == std::string("type"))
			{
				tSet.push_back(type_iter->GetText());
			}
		} // end for type_iter
	} // end for doc_iter

	return true;
}

std::vector<std::string> PatternImageInfo::attributeNames()
{
	std::vector<std::string> names;
	for (auto iter : s_typeSet)
		names.push_back(iter.first);
	return names;
}

const std::vector<std::string>& PatternImageInfo::attributeTypes(const std::string& name)
{
	return s_typeSet.at(name);
}
