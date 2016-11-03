#include "global_data_holder.h"

#include <fstream>
#include <sstream>

#define CHECK_FILE(result, filename) \
if (!(result))\
	throw std::exception((std::string("open file error: ") + filename).c_str());

GlobalDataHolder g_dataholder;

void GlobalDataHolder::init()
{
	m_curIndex = -1;
	m_curIndex_imgIndex = -1;
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
	
	std::string tmp;
	ldp::fileparts(filename, m_rootPath, tmp, tmp);

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
		m_imgInfos.push_back(info);
		m_curIndex = 0;
		m_curIndex_imgIndex = 0;
	} while (!fstm_s.eof());
}

void GlobalDataHolder::loadXml(std::string filename)
{
	m_imgInfos.clear();
	std::string tmp;
	ldp::fileparts(filename, m_rootPath, tmp, tmp);
	TiXmlDocument doc;
	CHECK_FILE(doc.LoadFile(filename.c_str()), filename);
	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		PatternImageInfo info;
		info.fromXml(doc_iter);
		std::vector<std::string> imgs;
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath, info.getBaseName()), imgs, "jpg");
		ldp::getAllFilesInDir(ldp::fullfile(m_rootPath, info.getBaseName()), imgs, "png");
		for (auto img : imgs)
			info.addImage(img);
		m_imgInfos.push_back(info);
	} // end for doc_iter
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
}