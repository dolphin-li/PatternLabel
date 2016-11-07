#pragma once

#include "util.h"
#include <map>
#include "PatternImageInfo.h"

class GlobalDataHolder
{
public:
	void init();

	void loadImageList(QString filename);
	void loadJdImageList(QString filename);
	void loadXml(QString filename);
	void saveXml(QString filename)const;

	// collect all labeled pattern xmls within the folder
	// ignore those "other" types and merge valid types.
	void collect_labelded_patterns(QString folder);
protected:
	void loadLastRunInfo();
	void saveLastRunInfo()const;
	bool loadXml_tixml(QString filename);
	bool saveXml_tixml(QString filename)const;
	bool loadXml_qxml(QString filename);
	bool saveXml_qxml(QString filename)const;
public:
	std::vector<PatternImageInfo> m_imgInfos;
	QString m_rootPath;
	int m_curIndex;
	int m_curIndex_imgIndex;
	QString m_xmlExportPureName;

	mutable QString m_lastRun_RootDir;
	mutable int m_lastRun_imgId;
};

extern GlobalDataHolder g_dataholder;