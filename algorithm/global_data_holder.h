#pragma once

#include "util.h"
#include <map>
#include "PatternImageInfo.h"

class GlobalDataHolder
{
public:
	void init();

	void loadImageList(std::string filename);
	void loadJdImageList(std::string filename);
	void loadXml(std::string filename);
	void saveXml(std::string filename)const;
protected:
	void loadLastRunInfo();
	void saveLastRunInfo()const;
public:
	std::vector<PatternImageInfo> m_imgInfos;
	std::string m_rootPath;
	int m_curIndex;
	int m_curIndex_imgIndex;
	std::string m_xmlExportPureName;

	mutable std::string m_lastRun_RootDir;
	mutable int m_lastRun_imgId;
};

extern GlobalDataHolder g_dataholder;