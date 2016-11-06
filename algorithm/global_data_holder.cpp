#include "global_data_holder.h"

#include <fstream>
#include <sstream>
#include "libxl.h"

#define CHECK_FILE(result, filename) \
if (!(result))\
	throw std::exception((std::string("open file error: ") + filename).c_str());

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
	std::ifstream stm("__lastruninfo.txt");
	if (stm.fail())
		return;
	std::getline(stm, m_lastRun_RootDir);
	stm >> m_lastRun_imgId;
	if (m_lastRun_imgId < 0)
		m_lastRun_imgId = 0;
	stm.close();
}

void GlobalDataHolder::saveLastRunInfo()const
{
	std::ofstream stm("__lastruninfo.txt");
	if (stm.fail())
		return;
	stm << m_lastRun_RootDir << std::endl;
	stm << m_lastRun_imgId << std::endl;
	stm.close();
}

void GlobalDataHolder::loadImageList(std::string filename)
{
	m_imgInfos.clear();

	std::ifstream fstm_f(filename);
	CHECK_FILE(!fstm_f.fail(), filename);
	std::stringstream fstm_s;
	std::copy(std::istreambuf_iterator<char>(fstm_f),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(fstm_s));
	fstm_f.close();
	
	std::string txtname, tmp;
	ldp::fileparts(filename, m_rootPath, txtname, tmp);
	m_xmlExportPureName = txtname + ".xml";

	std::string lineBuffer;
	do
	{
		PatternImageInfo info;
		std::getline(fstm_s, lineBuffer);
		info.setBaseName(lineBuffer);
		std::getline(fstm_s, lineBuffer);
		info.setUrl(lineBuffer);
		std::getline(fstm_s, lineBuffer);
		int nImgs = 0;
		std::stringstream stm(lineBuffer);
		stm >> nImgs;
		std::vector<std::string> imgs;
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath, info.getBaseName()), imgs, "jpg");
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath, info.getBaseName()), imgs, "png");
		if (nImgs != imgs.size())
			printf("warning: size of %s not matched: %d vs %d\n", info.getBaseName().c_str(), nImgs, imgs.size());
		for (int i = 0; i < nImgs; i++)
			std::getline(fstm_s, lineBuffer);
		for (auto img : imgs)
			info.addImage(img);
		if (info.getBaseName() != "")
		{
			m_imgInfos.push_back(info);
		}
		m_curIndex = 0;
		m_curIndex_imgIndex = 0;
	} while (!fstm_s.eof());

	if (ldp::validWindowsPath(m_rootPath) != ldp::validWindowsPath(m_lastRun_RootDir))
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
}

void GlobalDataHolder::loadXml(std::string filename)
{
	m_imgInfos.clear();
	std::string xmlname, ext;
	ldp::fileparts(filename, m_rootPath, xmlname, ext);
	m_xmlExportPureName = xmlname + ext;
	TiXmlDocument doc;
	CHECK_FILE(doc.LoadFile(filename.c_str()), filename);
	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		PatternImageInfo info;
		info.fromXml(m_rootPath, doc_iter);
		if (info.getBaseName() != "")
			m_imgInfos.push_back(info);
	} // end for doc_iter

	if (ldp::validWindowsPath(m_rootPath) != ldp::validWindowsPath(m_lastRun_RootDir))
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
}

void GlobalDataHolder::saveXml(std::string filename)const
{
	TiXmlDocument doc;

	for (const auto& info : m_imgInfos)
	{
		TiXmlElement* ele = new TiXmlElement(info.getBaseName().c_str());
		doc.LinkEndChild(ele);
		info.toXml(ele);
	} // end for info

	CHECK_FILE(doc.SaveFile(filename.c_str()), filename);

	saveLastRunInfo();
}

void GlobalDataHolder::loadJdImageList(std::string filename)
{
	m_imgInfos.clear();
	std::string txtname, tmp;
	ldp::fileparts(filename, m_rootPath, txtname, tmp);
	m_xmlExportPureName = txtname + ".xml";
	if (ldp::validWindowsPath(m_rootPath) != ldp::validWindowsPath(m_lastRun_RootDir))
	{
		m_lastRun_RootDir = m_rootPath;
		m_lastRun_imgId = 0;
	}
}