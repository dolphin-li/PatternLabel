#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "tinyxml\tinyxml.h"
class PatternImageInfo
{
public:
	PatternImageInfo();
	~PatternImageInfo();

	void clear();
	int numImages()const;
	std::string getImageName(int i)const;
	void clearImages();
	void addImage(const std::string& name);
	void setBaseName(const std::string& name);
	std::string getBaseName()const;
	void setUrl(const std::string& url);
	std::string getUrl()const;
	std::string getAttributeType(const std::string& typeName)const;
	void setAttributeType(const std::string& typeName, const std::string& type);
	void toXml(TiXmlNode* parent)const;
	void fromXml(std::string rootFolder, TiXmlElement* parent);
public:
	static bool initialized() { return s_mapInitialized; }
	static int numAttributes() { return (int)s_typeSet.size(); }
	static std::vector<std::string> attributeNames();
	static const std::vector<std::string>& attributeTypes(const std::string& name);
protected:
	static bool constructTypeMaps();
	void setDefaultTypes();
private:
	std::string m_baseName;
	std::string m_url;
	std::vector<std::string> m_imgNames;
	std::map<std::string, int> m_types;

	static bool s_mapInitialized;
	// E.G., cloth types, collar types, ...
	static std::map<std::string, std::vector<std::string>> s_typeSet;
};