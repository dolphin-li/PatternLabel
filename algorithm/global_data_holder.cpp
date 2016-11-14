#include "global_data_holder.h"

#include <fstream>
#include <sstream>
#include "qtxlsx\xlsxdocument.h"
#include <qxml.h>
#include <qxmlstream.h>
#include <QFile>
#include <QFileInfo>
#include <qdir.h>
#define CHECK_FILE(result, filename) \
if (!(result))\
	throw std::exception(("open file error: " + filename).toStdString().c_str());
#define CHECK_FILE_EXIST(result, filename) \
if (!(result))\
	throw std::exception(("file not exist: " + filename).toStdString().c_str());
#define CHECK_BOOL(cond)\
if (!(cond))\
	throw std::exception((std::string("CHECK failed: ") + #cond).c_str());
GlobalDataHolder g_dataholder;

void GlobalDataHolder::init()
{
	m_curIndex = -1;
	m_curIndex_imgIndex = -1;
	m_xmlExportPureName = "patterns.xml";
	m_lastRun_RootDir = "//dongping-pc1/sewingPatterns/burdastyle_data";
	m_lastRun_PatternDir = "//dongping-pc1/sewingPatterns/burdastyle_data";
	m_lastRun_imgId = 0;
	m_addPatternMode = false;
	loadLastRunInfo();
}

void GlobalDataHolder::loadLastRunInfo()
{
	std::wifstream stm(L"__lastruninfo.txt");
	if (stm.fail())
		return;
	std::wstring str;
	std::getline(stm, str);
	m_lastRun_RootDir = QString().fromStdWString(str);
	stm >> m_lastRun_imgId;
	if (m_lastRun_imgId < 0)
		m_lastRun_imgId = 0;
	str = L"";
	while (!stm.eof())
	{
		std::getline(stm, str);
		if (!QString().fromStdWString(str).trimmed().isEmpty())
			break;
	} 
	if (!str.empty())
		m_lastRun_PatternDir = QString().fromStdWString(str);
	stm.close();
}

void GlobalDataHolder::saveLastRunInfo()const
{
	std::wofstream stm(L"__lastruninfo.txt");
	if (stm.fail())
		return;
	stm << m_lastRun_RootDir.toStdWString() << std::endl;
	stm << m_lastRun_imgId << std::endl;
	stm << m_lastRun_PatternDir.toStdWString() << std::endl;
	stm.close();
}

void GlobalDataHolder::loadImageList(QString filename)
{
	m_imgInfos.clear();

	std::wifstream fstm_f(filename.toStdWString());
	CHECK_FILE(!fstm_f.fail(), filename);
	std::wstringstream fstm_s;
	std::copy(std::istreambuf_iterator<wchar_t>(fstm_f),
		std::istreambuf_iterator<wchar_t>(),
		std::ostreambuf_iterator<wchar_t>(fstm_s));
	fstm_f.close();
	
	QFileInfo finfo(filename);
	m_xmlExportPureName = finfo.baseName() + ".xml";
	m_rootPath = finfo.absolutePath();
	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteFilePath() != linfo.absoluteFilePath())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
	saveLastRunInfo();

	std::wstring lineBuffer;
	do
	{
		PatternImageInfo info;
		std::getline(fstm_s, lineBuffer);
		info.setBaseName(QString().fromStdWString(lineBuffer));
		std::getline(fstm_s, lineBuffer);
		info.setUrl(QString().fromStdWString(lineBuffer));
		std::getline(fstm_s, lineBuffer);
		int nImgs = 0;
		std::wstringstream stm(lineBuffer);
		stm >> nImgs;
		std::vector<std::wstring> imgs;
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath.toStdWString(), 
			info.getBaseName().toStdWString()), imgs, L"jpg");
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath.toStdWString(), 
			info.getBaseName().toStdWString()), imgs, L"png");
		if (nImgs != imgs.size())
			std::cout << "warning: size of " << info.getBaseName().toStdString() << " not matched" << std::endl;
		for (int i = 0; i < nImgs; i++)
			std::getline(fstm_s, lineBuffer);
		for (auto img : imgs)
			info.addImage(QString().fromStdWString(img));
		if (info.getBaseName() != "")
		{
			m_imgInfos.push_back(info);
		}
		m_curIndex = 0;
		m_curIndex_imgIndex = 0;
	} while (!fstm_s.eof());
	autoSetGenders(m_imgInfos, m_xmlExportPureName);
	saveXml(QDir::cleanPath(m_rootPath + QDir::separator() + m_xmlExportPureName));
}

void GlobalDataHolder::autoSetGenders(std::vector<PatternImageInfo>& infos, QString fileBaseName)
{
	for (auto& info : infos)
	{
		if (info.getAttributeType("gender-types") == "unknown")
		{
			auto b = info.getBaseName();
			if (b.contains("_w") || b.contains("_nv"))
			{
				info.setAttributeType("gender-types", "female");
				continue;
			}
			if (b.contains("_m") || b.contains("_nan"))
			{
				info.setAttributeType("gender-types", "male");
				continue;
			}
			b = fileBaseName;
			if (b.contains("_w") || b.contains("_nv"))
			{
				info.setAttributeType("gender-types", "female");
				continue;
			}
			if (b.contains("_m") || b.contains("_nan"))
			{
				info.setAttributeType("gender-types", "male");
				continue;
			}
		}
	}
}

void GlobalDataHolder::loadXml(QString filename)
{
	m_imgInfos.clear();
	QFileInfo finfo(filename);
	m_xmlExportPureName = finfo.baseName() + ".xml";
	m_rootPath = finfo.absolutePath();
	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteFilePath() != linfo.absoluteFilePath())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
	saveLastRunInfo();

	if (!loadXml_qxml(filename, m_rootPath, m_imgInfos))
	{
		CHECK_FILE(loadXml_tixml(filename, m_rootPath, m_imgInfos), filename);
		CHECK_FILE(saveXml_tixml(filename + ".backup", m_rootPath, m_imgInfos), filename);
		CHECK_FILE(saveXml_qxml(filename, m_rootPath, m_imgInfos), filename);
	}

	autoSetGenders(m_imgInfos, m_xmlExportPureName);

	if (!PatternImageInfo::getPatternXmlName().isEmpty())
	{
		QFileInfo finfo(PatternImageInfo::getPatternXmlName());
		if (finfo.exists())
		{
			loadPatternXml(finfo.absoluteFilePath());

			for (auto& info : m_imgInfos)
			{
				auto& iter = m_namePatternMap.find(info.getJdMappedPattern());
				if (iter != m_namePatternMap.end())
					iter.value().second++;
			}
		} // end if finfo
		else
			std::cout << "warning, pattern not exists: " << finfo.absoluteFilePath().toStdString() << std::endl;
	} // end if has pattern xml name
}

void GlobalDataHolder::saveXml(QString filename)const
{
	QFileInfo finfo(filename);
	QFileInfo linfo(m_lastRun_RootDir);
	if (finfo.absolutePath() != linfo.absoluteFilePath())
		m_lastRun_RootDir = finfo.absolutePath();
	CHECK_FILE(saveXml_qxml(filename, finfo.absolutePath(), m_imgInfos), filename);
	saveLastRunInfo();
}

inline QString findAppropriateJdImgPath(QString root, QString xlslName, QString imgName)
{
	const static int nExts = 1;
	const static QString exts[nExts] = {
		".jpg"
		//, ".png"
	};
	for (int iExt = 0; iExt < nExts; iExt++)
	{
		QFileInfo info;
		QDir dir;
		dir = QDir::cleanPath(root + QDir::separator() + xlslName);
		info.setFile(dir, imgName + exts[iExt]);
		if (info.exists())
			return info.absoluteFilePath();
		dir = QDir::cleanPath(root + QDir::separator() + xlslName + QDir::separator() + "image");
		info.setFile(dir, imgName + exts[iExt]);
		if (info.exists())
			return info.absoluteFilePath();
		dir = QDir::cleanPath(root + QDir::separator() + "image");
		info.setFile(dir, imgName + exts[iExt]);
		if (info.exists())
			return info.absoluteFilePath();
	}
	return "";
}

inline QString jdImage2IdMask(QString img)
{
	QFileInfo info(img);
	QDir dir(info.absolutePath());
	QString s = dir.absoluteFilePath(info.baseName() + "_label.png");
	QFileInfo info1(s);
	if (info1.exists())
		return info1.absoluteFilePath();
	return "";
}

inline QString jdBasename(QString imgFullName, QString root)
{
	QFileInfo info(imgFullName);
	return info.absolutePath().right(info.absolutePath().size() - QDir::cleanPath(root).size() - 1);
}

inline void convertJdAttNameAndType(QString str, QString& jdAttName, QString& jdType)
{
	int seg = str.indexOf(65306); //NOTE: this is uniode ':'
	jdAttName = str.left(seg).trimmed(); 
	jdType = str.right(str.size()-seg-1).trimmed();
	int pos = jdType.indexOf('.');
	if (pos >= 0) // ignore those numbers
		jdType = "";
}

void GlobalDataHolder::loadJdImageList(QString filename)
{
	m_imgInfos.clear();	
	QFileInfo finfo(filename);
	if (!finfo.baseName().toLower().endsWith("_imgid"))
		throw std::exception("xlsx file must be ends with \"_imgid\"");
	QString imgRelFolder = finfo.baseName().left(finfo.baseName().size()-6);
	m_xmlExportPureName = finfo.baseName() + ".xml";
	m_rootPath = finfo.absolutePath();
	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteFilePath() != linfo.absoluteFilePath())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
	saveLastRunInfo();
	
	// load xlsx
	QXlsx::Document doc(filename);
	QXlsx::Worksheet* sheet = (QXlsx::Worksheet*)doc.sheet("Sheet1");
	if (!sheet)
		throw std::exception("no valid Sheet1");
		
	const int fr = sheet->dimension().firstRow();
	const int lr = sheet->dimension().lastRow();
	const int fc = sheet->dimension().firstColumn();
	const int lc = sheet->dimension().lastColumn();
	if (lc - fc + 1 != 5)
		throw std::exception("xlsx: cols must be 5");
	PatternImageInfo* curInfo = nullptr;
	QVector<QString> values(lc-fc+1, "");
	for (int row = fr + 1; row < lr; ++row)
	{
		values.fill("");
		for (int col = fc; col <= lc; col++)
		{
			auto cell = sheet->cellAt(row, col);
			if (cell)
				values[col-fc] = cell->value().toString();
		}

		if (!values[0].isEmpty())
		{
			QString img = findAppropriateJdImgPath(m_rootPath, imgRelFolder, values[4]);
			if (!img.isEmpty())
			{
				m_imgInfos.push_back(PatternImageInfo());
				curInfo = &m_imgInfos.back();
				curInfo->setJdTitle(values[0]);
				curInfo->setJdId(values[1]);
				curInfo->setUrl(values[2]);
				curInfo->addImage(img);
				curInfo->setBaseName(jdBasename(img, m_rootPath));
				auto mask = jdImage2IdMask(img);
				if (!mask.isEmpty())
					curInfo->addImage(mask);
				auto mapped = PatternImageInfo::jdAttributeMapped("cloth-types", imgRelFolder);
				if (!mapped.first.isEmpty() && !mapped.second.isEmpty())
					curInfo->setAttributeType(mapped.first, mapped.second);
			}
			else
				curInfo = nullptr;
		} // end if not empty value0

		if (curInfo && !values[3].isEmpty())
		{
			QString jdName, jdType;
			convertJdAttNameAndType(values[3], jdName, jdType);
			if (!jdType.isEmpty())
			{
				// useful when construct __attributes.xml
				//PatternImageInfo::addJdAttributeMap(jdName, "", jdType, "");

				auto mapped = PatternImageInfo::jdAttributeMapped(jdName, jdType);
				if (!mapped.first.isEmpty() && !mapped.second.isEmpty())
					curInfo->setAttributeType(mapped.first, mapped.second);
			}
		} // end if value[3]
	} // end for row
	autoSetGenders(m_imgInfos, m_xmlExportPureName);
	saveXml(QDir::cleanPath(m_rootPath+QDir::separator()+m_xmlExportPureName));
	// useful when construct __attributes.xml
	//PatternImageInfo::constructTypeMaps_qxml_save("__attributes.xml");
}

bool GlobalDataHolder::loadXml_tixml(QString filename, QString root, std::vector<PatternImageInfo>& imgInfos)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename.toStdString().c_str()))
		return false;
	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		PatternImageInfo info;
		if (!info.fromXml(root, doc_iter))
		{
			imgInfos.clear();
			return false;
		}
		if (info.getBaseName() != "")
			imgInfos.push_back(info);
	} // end for doc_iter
	return true;
}

bool GlobalDataHolder::saveXml_tixml(QString filename, QString root, const std::vector<PatternImageInfo>& imgInfos)
{
	TiXmlDocument doc;

	for (const auto& info : imgInfos)
	{
		TiXmlElement* ele = new TiXmlElement(info.getBaseName().toStdString().c_str());
		doc.LinkEndChild(ele);
		info.toXml(ele);
	} // end for info

	if (!doc.SaveFile(filename.toStdString().c_str()))
		return false;
	return true;
}

bool GlobalDataHolder::loadXml_qxml(QString filename, QString root, std::vector<PatternImageInfo>& imgInfos)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		std::cout << "File not exist: " << filename.toStdString() << std::endl;
		return false;
	}
	QXmlStreamReader reader(&file);
	if (reader.hasError())
	{
		std::cout << "read error [" << filename.toStdString() << "]: "
			<< reader.errorString().toStdString() << std::endl;
		return false;
	}
	reader.readNextStartElement();
	if (reader.name() != "document")
	{
		std::cout << "read error [" << filename.toStdString() << "]: root name must be <document>" << std::endl;
		return false;
	}
	while (!reader.isEndDocument())
	{
		PatternImageInfo info;
		info.fromXml(root, reader);
		if (info.getBaseName() != "")
			imgInfos.push_back(info);
		if (reader.hasError())
		{
			std::cout << "read error [" << filename.toStdString() << "]: "
				<< reader.errorString().toStdString() << std::endl;
			imgInfos.clear();
			return false;
		}
	} // end for doc_iter

	return true;
}

bool GlobalDataHolder::saveXml_qxml(QString filename, QString root, const std::vector<PatternImageInfo>& imgInfos)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		std::cout << "File not exist: " << filename.toStdString() << std::endl;
		return false;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	if (writer.hasError())
		return false;
	writer.writeStartDocument();
	writer.writeStartElement("document");
	if (!PatternImageInfo::getPatternXmlName().isEmpty())
		writer.writeTextElement("pattern-xml", PatternImageInfo::getPatternXmlName());
	for (const auto& info : imgInfos)
	{
		info.toXml(writer);
		if (writer.hasError())
			return false;
	} // end for info
	writer.writeEndElement();
	writer.writeEndDocument();
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
void GlobalDataHolder::collect_labelded_patterns(QString folder)
{
	static QString mergeName = "pattern_merged";
	std::vector<std::wstring> xmlNames;
	ldp::getAllFilesInDir(folder.toStdWString(), xmlNames, L".xml");
	auto cfolder = QDir::cleanPath(folder);
	std::vector<PatternImageInfo> imgInfoMerged;
	m_lastRun_PatternDir = folder;
	saveLastRunInfo();
	
	for (size_t iXml = 0; iXml < xmlNames.size(); iXml++)
	{
		QString xmlFullName = QString().fromStdWString(xmlNames[iXml]);
		QFileInfo xinfo(xmlFullName);
		auto xpath = xinfo.absolutePath();
		// ignore the merged xml itself
		if (xinfo.baseName().toLower() == mergeName.toLower())
			continue;
		std::vector<PatternImageInfo> imgInfo;
		if (!loadXml_qxml(xmlFullName, xpath, imgInfo))
		{
			CHECK_FILE(loadXml_tixml(xmlFullName, xpath, imgInfo), xmlFullName);
			CHECK_FILE(saveXml_tixml(xmlFullName + ".backup", xpath, imgInfo), xmlFullName);
			CHECK_FILE(saveXml_qxml(xmlFullName, xpath, imgInfo), xmlFullName);
		}
		for (const auto& info : imgInfo)
		{
			if (info.getBaseName() == "" || info.getAttributeType("cloth-types") == "other"
				|| info.getAttributeType("cloth-types") == "")
				continue;
			imgInfoMerged.push_back(info);
			int num = std::max(0, xpath.size() - cfolder.size() - 1);
			imgInfoMerged.back().setBaseName(QDir::cleanPath(xpath.right(num))
				+ QDir::separator() + imgInfoMerged.back().getBaseName());
		} // end for info
		std::cout << "xml processed: " << iXml << "/" << xmlNames.size() << std::endl;
	} // end for iXml

	QDir cdir(folder);
	auto saveName = cdir.absoluteFilePath(mergeName + ".xml");
	CHECK_FILE(saveXml_qxml(saveName, folder, imgInfoMerged), saveName);
}

void GlobalDataHolder::loadPatternXml(QString filename)
{
	m_patternInfos.clear();
	m_namePatternMap.clear();
	m_inputPatternXmlName = filename;
	QFileInfo finfo(filename);
	QFileInfo linfo(m_lastRun_PatternDir);
	if (finfo.absolutePath() != linfo.absoluteFilePath())
		m_lastRun_PatternDir = finfo.absolutePath();
	saveLastRunInfo();
	CHECK_FILE(loadXml_qxml(filename, finfo.absolutePath(), m_patternInfos), filename);
	PatternImageInfo::setPatternXmlName(filename);
	for (auto& pattern : m_patternInfos)
		m_namePatternMap.insert(pattern.getBaseName(), qMakePair(&pattern, 0));
	for (auto& info : m_imgInfos)
	{
		auto& iter = m_namePatternMap.find(info.getJdMappedPattern());
		if (iter != m_namePatternMap.end())
			iter.value().second++;
	}
	autoSetGenders(m_patternInfos, finfo.baseName());
}

void GlobalDataHolder::savePatternXml(QString filename)const
{
	QFileInfo finfo(filename);
	QFileInfo linfo(m_lastRun_PatternDir);
	if (finfo.absolutePath() != linfo.absoluteFilePath())
		m_lastRun_PatternDir = finfo.absolutePath();
	CHECK_FILE(saveXml_qxml(filename, finfo.absolutePath(), m_patternInfos), filename);
	saveLastRunInfo();
}

void GlobalDataHolder::uniquePatterns()
{
	std::cout << "before cleaning: " << m_patternInfos.size() << std::endl;
	QMap<QString, PatternImageInfo> urlPatternMap;
	for (const auto& info : m_patternInfos)
	if (urlPatternMap.find(info.getUrl()) == urlPatternMap.end())
		urlPatternMap.insert(info.getUrl(), info);

	m_patternInfos.clear();
	for (const auto& info : urlPatternMap)
		m_patternInfos.push_back(info);
	m_namePatternMap.clear();
	for (auto& pattern : m_patternInfos)
		m_namePatternMap.insert(pattern.getBaseName(), qMakePair(&pattern, 0));
	for (auto& info : m_imgInfos)
	{
		auto& iter = m_namePatternMap.find(info.getJdMappedPattern());
		if (iter != m_namePatternMap.end())
			iter.value().second++;
	}
	std::cout << "after cleaning: " << m_patternInfos.size() << std::endl;
}

void GlobalDataHolder::exportPatternTrainingData(const QStringList& labeledXmls)
{
	if (labeledXmls.isEmpty())
		return;
	QFileInfo inputFileInfo(labeledXmls[0]);
	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (inputFileInfo.absolutePath() != linfo.absoluteFilePath())
	{
		m_lastRun_RootDir = inputFileInfo.absolutePath();
		m_lastRun_imgId = 0;
	}
	saveLastRunInfo();

	// load all input xmls
	std::vector<PatternImageInfo> imgInfosAll;
	QString patternXmlName;
	QMap<QString, int> patternUseNumMap, patternUseIdxMap;
	QVector <QPair<int, QString>> patternUseNumInvMap;
	for (const auto& xmlName : labeledXmls)
	{
		std::vector<PatternImageInfo> imgInfos;
		CHECK_FILE(loadXml_qxml(xmlName, inputFileInfo.absolutePath(), imgInfos), xmlName);
		auto tmp = PatternImageInfo::getPatternXmlName();
		if (patternXmlName.isEmpty())
			patternXmlName = tmp;
		else if (patternXmlName != tmp)
			std::cout << "warning exporting training data: pattern xml not matched!" << std::endl;
		for (const auto& info : imgInfos)
		{
			if (info.getAttributeType("cloth-types") != "other")
			{
				auto pt = info.getJdMappedPattern();
				if (!pt.isEmpty())
				{
					if (patternUseNumMap.find(pt) == patternUseNumMap.end())
						patternUseNumMap.insert(pt, 1);
					else
						++patternUseNumMap[pt];
					imgInfosAll.push_back(info);
				}
			} // end if not "other"
		} // end for info
		std::cout << "parsed: " << xmlName.toStdString() << std::endl;
		std::flush(std::cout);
	} // end for xml
	PatternImageInfo::setPatternXmlName(patternXmlName);
	int totalNumUsed = 0;
	for (auto iter = patternUseNumMap.begin(); iter != patternUseNumMap.end(); ++iter)
	{
		patternUseNumInvMap.push_back(qMakePair(iter.value(), iter.key()));
		totalNumUsed += iter.value();
	}
	qSort(patternUseNumInvMap.begin(), patternUseNumInvMap.end(), qGreater<QPair<int, QString>>());

	// extract used patterns and save
	loadPatternXml(patternXmlName);
	std::vector<PatternImageInfo> patternUsed;
	for (int idx = 0; idx != patternUseNumInvMap.size(); ++idx)
	{
		QString name = patternUseNumInvMap[idx].second;
		const auto& iter = m_namePatternMap.find(name);
		if (iter == m_namePatternMap.end())
			continue;
		patternUsed.push_back(*iter.value().first);
	} // end for info
	QString patternOutName = QDir::cleanPath(m_lastRun_PatternDir + QDir::separator() + "patterns_used.xml");
	CHECK_FILE(saveXml_qxml(patternOutName, m_lastRun_PatternDir, patternUsed), patternOutName);

	// save all used patterns ordered by the num of usage.
	QString outSummaryName = QDir::cleanPath(inputFileInfo.absolutePath() + QDir::separator() + "z_labeled_pattern_idx_map.xml");
	QFile outSummaryXml(outSummaryName);
	if (!outSummaryXml.open(QIODevice::WriteOnly))
		throw std::exception(("File not exist: " + outSummaryName.toStdString()).c_str());
	QXmlStreamWriter writer(&outSummaryXml);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("document");
	writer.writeTextElement("pattern-xml", PatternImageInfo::getPatternXmlName());
	writer.writeComment(QString().sprintf("total num labeled: %d", totalNumUsed));
	for (int idx = 0; idx != patternUseNumInvMap.size(); ++idx)
	{
		QString name = patternUseNumInvMap[idx].second;
		writer.writeStartElement("pattern");
		writer.writeAttribute("idx", QString().sprintf("%d", idx));
		writer.writeAttribute("num", QString().sprintf("%d", patternUseNumMap[name]));
		writer.writeAttribute("name", name);
		writer.writeEndElement();
		patternUseIdxMap.insert(name, idx);
	} // end for info
	writer.writeEndElement();
	writer.writeEndDocument();

	// save the jd-pattern map
	QString outName = QDir::cleanPath(inputFileInfo.absolutePath() + QDir::separator() + "z_labeled_jdPatterns.txt");
	std::wofstream stm1(outName.toStdWString());
	stm1 << outSummaryName.toStdWString() << std::endl;
	for (const auto& info : imgInfosAll)
	{
		if (info.numImages() == 0)
			continue;
		QString pt = info.getJdMappedPattern();
		if (patternUseIdxMap.find(pt) == patternUseIdxMap.end())
			continue;
		QString imgName = info.getImageName(0);
		QFileInfo imgInfo(imgName);
		if (!imgInfo.exists())
			continue;
		QString out = QDir::cleanPath(info.getBaseName() + QDir::separator() + imgInfo.fileName());
		stm1 << out.toStdWString() << " " << patternUseIdxMap[pt] << std::endl;
	} // end for info
	stm1.close();

	// also save all labeled infos, mainly for debug
	QString outXmlName = QDir::cleanPath(inputFileInfo.absolutePath() + QDir::separator() + "z_labeled_jdPatterns.xml");
	CHECK_FILE(saveXml_qxml(outXmlName, inputFileInfo.absolutePath(), imgInfosAll), outXmlName);
}