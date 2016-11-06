#include "global_data_holder.h"

#include <fstream>
#include <sstream>
#include "libxl.h"
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

	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteDir() != linfo.absoluteDir())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
}

void GlobalDataHolder::loadXml(QString filename)
{
	m_imgInfos.clear();
	QFileInfo finfo(filename);
	m_xmlExportPureName = finfo.baseName() + ".xml";
	m_rootPath = finfo.absolutePath();
	if (!loadXml_qxml(filename))
	{
		CHECK_FILE(loadXml_tixml(filename), filename);
		CHECK_FILE(saveXml_tixml(filename + ".backup"), filename);
		CHECK_FILE(saveXml_qxml(filename), filename);
	}

	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteDir() != linfo.absoluteDir())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
}

void GlobalDataHolder::saveXml(QString filename)const
{
	CHECK_FILE(saveXml_qxml(filename), filename);
	saveLastRunInfo();
}

void GlobalDataHolder::loadJdImageList(QString filename)
{
	m_imgInfos.clear();	
	QFileInfo finfo(filename);
	m_xmlExportPureName = finfo.baseName() + ".xml";
	m_rootPath = finfo.absolutePath();
	QFileInfo rinfo(m_rootPath), linfo(m_lastRun_RootDir);
	if (rinfo.absoluteDir() != linfo.absoluteDir())
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
	
	// load xlsx
	libxl::IBookT<TCHAR>* book = xlCreateXMLBookW();
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
