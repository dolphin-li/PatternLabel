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
	m_lastRun_imgId = 0;
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
	stm.close();
}

void GlobalDataHolder::saveLastRunInfo()const
{
	std::wofstream stm(L"__lastruninfo.txt");
	if (stm.fail())
		return;
	stm << m_lastRun_RootDir.toStdWString() << std::endl;
	stm << m_lastRun_imgId << std::endl;
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
			wprintf(L"warning: size of %s not matched: %d vs %d\n", 
			info.getBaseName().toStdWString().c_str(), nImgs, imgs.size());
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

	if (!loadXml_qxml(filename))
	{
		CHECK_FILE(loadXml_tixml(filename), filename);
		CHECK_FILE(saveXml_tixml(filename + ".backup"), filename);
		CHECK_FILE(saveXml_qxml(filename), filename);
	}

}

void GlobalDataHolder::saveXml(QString filename)const
{
	CHECK_FILE(saveXml_qxml(filename), filename);
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
		dir = QDir::cleanPath(root + QDir::separator() + xlslName + QDir::separator() + "images");
		info.setFile(dir, imgName + exts[iExt]);
		if (info.exists())
			return info.absoluteFilePath();
		dir = QDir::cleanPath(root + QDir::separator() + "images");
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

	// useful when construct __attributes.xml
	//PatternImageInfo::constructTypeMaps_qxml_save("__attributes.xml");
}

bool GlobalDataHolder::loadXml_tixml(QString filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename.toStdString().c_str()))
		return false;
	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		PatternImageInfo info;
		if (!info.fromXml(m_rootPath, doc_iter))
		{
			m_imgInfos.clear();
			return false;
		}
		if (info.getBaseName() != "")
			m_imgInfos.push_back(info);
	} // end for doc_iter
	return true;
}

bool GlobalDataHolder::saveXml_tixml(QString filename)const
{
	TiXmlDocument doc;

	for (const auto& info : m_imgInfos)
	{
		TiXmlElement* ele = new TiXmlElement(info.getBaseName().toStdString().c_str());
		doc.LinkEndChild(ele);
		info.toXml(ele);
	} // end for info

	if (!doc.SaveFile(filename.toStdString().c_str()))
		return false;
	return true;
}

bool GlobalDataHolder::loadXml_qxml(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		wprintf(L"File not exist: %s\n", filename.toStdWString().c_str());
		return false;
	}
	QXmlStreamReader reader(&file);
	if (reader.hasError())
	{
		wprintf(L"read error [%s]: %s\n", filename.toStdWString().c_str(), 
			reader.errorString().toStdWString().c_str());
		return false;
	}
	reader.readNextStartElement();
	if (reader.name() != "document")
	{
		wprintf(L"read error [%s]: %s\n", filename.toStdWString().c_str(),
			L"root name must be <document>");
		return false;
	}
	while (!reader.isEndDocument())
	{
		PatternImageInfo info;
		info.fromXml(m_rootPath, reader);
		if (info.getBaseName() != "")
			m_imgInfos.push_back(info);
		if (reader.hasError())
		{
			wprintf(L"read error [%s]: %s\n", filename.toStdWString().c_str(),
				reader.errorString().toStdWString().c_str());
			m_imgInfos.clear();
			return false;
		}
	} // end for doc_iter

	return true;
}

bool GlobalDataHolder::saveXml_qxml(QString filename)const
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		wprintf(L"File not exist: %s\n", filename.toStdWString().c_str());
		return false;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	if (writer.hasError())
		return false;
	writer.writeStartDocument();
	writer.writeStartElement("document");
	for (const auto& info : m_imgInfos)
	{
		info.toXml(writer);
		if (writer.hasError())
			return false;
	} // end for info
	writer.writeEndElement();
	writer.writeEndDocument();
	return true;
}

void GlobalDataHolder::collect_labelded_patterns(QString folder)
{
	std::vector<std::wstring> xmlNames;
	ldp::getAllFilesInDir(folder.toStdWString(), xmlNames, L".xml");
	auto imgInfoBackup = m_imgInfos;
	auto cfolder = QDir::cleanPath(folder);
	std::vector<PatternImageInfo> imgInfoMerged;
	
	for (size_t iXml = 0; iXml < xmlNames.size(); iXml++)
	{
		QString xmlFullName = QString().fromStdWString(xmlNames[iXml]);
		loadXml(xmlFullName);
		QFileInfo xinfo(xmlFullName);
		auto xpath = xinfo.absolutePath();
		for (const auto& info : m_imgInfos)
		{
			if (info.getBaseName() == "" || info.getAttributeType("cloth-types") == "other"
				|| info.getAttributeType("cloth-types") == "")
				continue;
			imgInfoMerged.push_back(info);
			imgInfoMerged.back().setBaseName(QDir::cleanPath(xpath.right(xpath.size()-cfolder.size()-1))
				+ QDir::separator() + imgInfoMerged.back().getBaseName());
		} // end for info
		printf("xml processed: %d/%d\n", iXml, xmlNames.size());
	} // end for iXml

	m_imgInfos = imgInfoMerged;
	QDir cdir(folder);
	saveXml(cdir.absoluteFilePath("pattern_merged.xml"));

	m_imgInfos = imgInfoBackup;
}
