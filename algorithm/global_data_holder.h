#pragma once

#include "util.h"
#include <map>
#include "PatternImageInfo.h"

class GlobalDataHolder
{
public:
	void init();

	void loadImageList(std::string filename);
	void loadXml(std::string filename);
	void saveXml(std::string filename)const;
public:
	std::vector<PatternImageInfo> m_imgInfos;
	std::string m_rootPath;
	int m_curIndex;
	int m_curIndex_imgIndex;
	std::string m_xmlExportPureName;
};

extern GlobalDataHolder g_dataholder;