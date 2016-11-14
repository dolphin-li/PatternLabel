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
	void loadPatternXml(QString filename);
	void savePatternXml(QString filename)const;
	void uniquePatterns();

	void exportPatternTrainingData(const QStringList& labeledXmls);
protected:
	void loadLastRunInfo();
	void saveLastRunInfo()const;
	static bool loadXml_tixml(QString filename, QString root, std::vector<PatternImageInfo>& imgInfos);
	static bool saveXml_tixml(QString filename, QString root, const std::vector<PatternImageInfo>& imgInfos);
	static bool loadXml_qxml(QString filename, QString root, std::vector<PatternImageInfo>& imgInfos);
	static bool saveXml_qxml(QString filename, QString root, const std::vector<PatternImageInfo>& imgInfos);
	static void autoSetGenders(std::vector<PatternImageInfo>& infos, QString fileBaseName);
public:
	std::vector<PatternImageInfo> m_imgInfos;
	QString m_rootPath;
	int m_curIndex;
	int m_curIndex_imgIndex;
	QString m_xmlExportPureName;

	mutable QString m_lastRun_RootDir;
	mutable int m_lastRun_imgId;

	////
	std::vector<PatternImageInfo> m_patternInfos;
	QMap<QString, QPair<PatternImageInfo*,int>> m_namePatternMap;
	mutable QString m_lastRun_PatternDir;
	QString m_inputPatternXmlName;

	bool m_addPatternMode;
};

extern GlobalDataHolder g_dataholder;