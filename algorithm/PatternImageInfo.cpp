#include "PatternImageInfo.h"
#include "util.h"
#include <QFileinfo>
#include <QDir>
PatternImageInfo::PatternImageInfo()
{
	setDefaultTypes();
}

PatternImageInfo::~PatternImageInfo()
{

}

void PatternImageInfo::clear()
{
	m_imgNames.clear();
	setDefaultTypes();
}

int PatternImageInfo::numImages()const
{
	return (int)m_imgNames.size();
}

QString PatternImageInfo::getImageName(int i)const
{
	return m_imgNames.at(i);
}

void PatternImageInfo::clearImages()
{
	m_imgNames.clear();
}

void PatternImageInfo::addImage(const QString& name)
{
	m_imgNames.push_back(name);
}

void PatternImageInfo::setBaseName(const QString& name)
{
	m_baseName = name;
}

QString PatternImageInfo::getBaseName()const
{
	return m_baseName;
}

void PatternImageInfo::setUrl(const QString& url)
{
	m_url = url;
}

QString PatternImageInfo::getUrl()const
{
	return m_url;
}

QString PatternImageInfo::getAttributeType(const QString& typeName)const
{
	int id = m_types[typeName];
	return s_typeSet[typeName].at(id);
}

void PatternImageInfo::setAttributeType(const QString& typeName, const QString& type)
{
	const auto& types = s_typeSet[typeName];
	auto iter = std::find(types.begin(), types.end(), type);
	if (iter == types.end())
		throw std::exception(("non-defined type: " + type.toStdString()).c_str());
	m_types[typeName] = int(iter - types.begin());
}

void PatternImageInfo::setDefaultTypes()
{
	m_types.clear();
	for (auto iter = s_typeSet.begin(); iter != s_typeSet.end(); ++iter)
	{
		m_types.insert(iter.key(), 0);
	}
}

bool PatternImageInfo::toXml(QXmlStreamWriter& writer)const
{
	writer.writeStartElement("pattern");
	if (!getBaseName().isEmpty())
		writer.writeAttribute("name", getBaseName());
	if (!m_jdId.isEmpty())
		writer.writeAttribute("jdId", getJdId());
	if (!m_jdTitle.isEmpty())
		writer.writeTextElement("jdTitle", getJdTitle());
	writer.writeTextElement("url", getUrl());
	for (auto name : m_imgNames)
	{
		QFileInfo finfo(name);
		writer.writeTextElement("image", finfo.fileName());
	}
	for (auto iter = m_types.begin(); iter != m_types.end(); ++iter)
		writer.writeTextElement(iter.key(), getAttributeType(iter.key()));
	writer.writeEndElement();
	if (writer.hasError())
		return false;
	return true;
}

bool PatternImageInfo::fromXml(QString rootFolder, QXmlStreamReader& reader)
{
	bool eleEnd = false;
	while (!reader.isEndDocument())
	{
		reader.readNext();
		if (reader.hasError())
			return false;
		auto n = reader.name().toString().trimmed();
		if (reader.isEndElement())
		{
			if (n == "pattern")
			{
				eleEnd = true;
				break;
			}
		} // end if end element
		else if (reader.isStartElement())
		{
			if (n == "pattern")
			{
				auto att = reader.attributes();
				m_baseName = "";
				for (auto v : reader.attributes())
				{
					if (v.name() == "name")
						m_baseName = v.value().toString();
					else if (v.name() == "jdId")
						m_jdId = v.value().toString();
				} // end for v
			} // end if n == pattern
			else if (n == "image")
			{
				addImage(reader.readElementText());
			}
			else if (n == "url")
			{
				m_url = reader.readElementText();
			}
			else if (n == "jdId")
			{
				m_jdId = reader.readElementText();
			}
			else if (n == "jdTitle")
			{
				m_jdTitle = reader.readElementText();
			}
			else
			{
				if (m_types.find(n) != m_types.end())
					setAttributeType(n, reader.readElementText());
			}
		} // end if start element
	} // n
	
	if (!eleEnd)
		return false;

	for (auto& img : m_imgNames)
	{
		QDir dir = QDir::cleanPath(rootFolder + QDir::separator() + m_baseName);
		img = dir.absoluteFilePath(img);
	}

	return true;
}

bool PatternImageInfo::toXml(TiXmlNode* parent)const
{
	auto baseName = getBaseName();
	for (int i = 0; i < baseName.size(); i++)
	{
		if (baseName[i] == ' ' || baseName[i] == '(' || baseName[i] == ')'
			|| baseName[i] == '\'' || baseName[i] == '\"' || baseName[i] == '&'
			|| baseName[i] == '#' || baseName[i] == '%' || baseName[i] == '@'
			|| baseName[i] == '!' || baseName[i] == '*' || baseName[i] == '$'
			|| baseName[i] == '^' || baseName[i] == '+')
			baseName[i] = '-';
	}
	parent->SetValue(baseName.toStdString().c_str());
	TiXmlElement* base = new TiXmlElement("baseName");
	parent->LinkEndChild(base);
	base->SetAttribute("value", m_baseName.toStdString().c_str());
	TiXmlElement* url = new TiXmlElement("url");
	parent->LinkEndChild(url);
	url->SetAttribute("value", m_url.toStdString().c_str());
	for (auto name : m_imgNames)
	{
		std::string p, n, e;
		ldp::fileparts(name.toStdString(), p, n, e);
		TiXmlElement* img = new TiXmlElement("image");
		parent->LinkEndChild(img);
		img->SetAttribute("value", (n + e).c_str());
	}
	for (auto iter = m_types.begin(); iter != m_types.end(); ++iter)
	{
		TiXmlElement* ele = new TiXmlElement(iter.key().toStdString().c_str());
		parent->LinkEndChild(ele);
		ele->SetAttribute("value", getAttributeType(iter.key()).toStdString().c_str());
	} // m_types
	return true;
}

bool PatternImageInfo::fromXml(QString rootFolder, TiXmlElement* parent)
{
	clear();
	for (auto p_iter = parent->FirstChildElement(); p_iter; p_iter = p_iter->NextSiblingElement())
	{
		std::string name = p_iter->Value();
		std::string att = p_iter->Attribute("value");
		if (name == "url")
			m_url = QString().fromStdString(att);
		else if (name == "baseName")
			m_baseName = QString().fromStdString(att);
		else if (name == "image")
			addImage(att.c_str());
		else for (auto& iter = m_types.begin(); iter != m_types.end(); ++iter)
		{
			if (name == iter.key().toStdString())
			{
				auto& types = attributeTypes(iter.key());
				auto t_iter = qFind(types.begin(), types.end(), att.c_str());
				if (t_iter == types.end())
					printf("warning: invalid type %s\n", att.c_str());
				else
					iter.value() = int(t_iter - types.begin());
			} // end if name
		} // end for type
	} // end for p_iter

	for (auto& img : m_imgNames)
	{
		img = QString().fromStdWString(ldp::fullfile(ldp::fullfile(rootFolder.toStdWString(), 
			m_baseName.toStdWString()), img.toStdWString()));
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
QMap<QString, QVector<QString>> PatternImageInfo::s_typeSet;
bool PatternImageInfo::s_mapInitialized = PatternImageInfo::constructTypeMaps();

bool PatternImageInfo::constructTypeMaps()
{
	QString filename = "__attributes.xml";
	bool r = constructTypeMaps_qxml(filename);
	if (!r)
	{
		r = constructTypeMaps_tixml(filename);
		if (r)
			r = constructTypeMaps_qxml_save(filename);
	}
	return r;
}

bool PatternImageInfo::constructTypeMaps_tixml(QString filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename.toStdString().c_str()))
	{
		wprintf(L"File not exist: %s\n", filename.toStdWString().c_str());
		return false;
	}

	for (auto doc_iter = doc.FirstChildElement(); doc_iter; doc_iter = doc_iter->NextSiblingElement())
	{
		s_typeSet.insert(QString(doc_iter->Value()), QVector<QString>());
		auto& tSet = s_typeSet[doc_iter->Value()];
		for (auto type_iter = doc_iter->FirstChildElement(); type_iter; type_iter = type_iter->NextSiblingElement())
		{
			if (type_iter->Value() == std::string("type"))
			{
				tSet.push_back(type_iter->GetText());
			}
		} // end for type_iter
	} // end for doc_iter

	return true;
}

bool PatternImageInfo::constructTypeMaps_qxml(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		wprintf(L"File not exist: %s\n", filename.toStdWString().c_str());
		return false;
	}
	QXmlStreamReader reader(&file);
	QString currTypes = "";
	while (!reader.isEndDocument() && !reader.hasError())
	{
		reader.readNext();
		if (!reader.isStartElement())
			continue;
		auto n = reader.name().toString();
		if (n == "types")
		{
			for (auto v : reader.attributes())
			{
				if (v.name() == "value")
				{
					s_typeSet.insert(v.value().toString(), QVector<QString>());
					currTypes = v.value().toString();
					break;
				}
			} // end if v
		} // end if n
		else if (n == "type" && currTypes != "")
		{
			auto& types = s_typeSet[currTypes];
			QString t = reader.readElementText();
			types.push_back(t);
		} // end if n
	} // end while
	if (s_typeSet.size() == 0)
	{
		wprintf(L"Error load [%s]: %s\n", filename.toStdWString().c_str(), L"illegal xml");
		return false;
	}
	if (reader.hasError())
	{
		wprintf(L"Error load [%s]: %s\n", filename.toStdWString().c_str(), reader.errorString());
		return false;
	}
	return true;
}


bool PatternImageInfo::constructTypeMaps_qxml_save(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		wprintf(L"File not exist: %s\n", filename.toStdWString().c_str());
		return false;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	writer.writeStartElement("document");
	for (auto iter = s_typeSet.begin(); iter != s_typeSet.end(); ++iter)
	{
		writer.writeStartElement("types");
		writer.writeAttribute("value", iter.key());
		for (auto type : iter.value())
		{
			writer.writeTextElement("type", type);
		}
		writer.writeEndElement();
	}
	writer.writeEndDocument();

	writer.writeEndDocument();
	file.close();
	if (writer.hasError())
	{
		wprintf(L"Error write [%s]\n", filename.toStdWString().c_str());
		return false;
	}
	return true;
}

QVector<QString> PatternImageInfo::attributeNames()
{
	QVector<QString> names;
	for (auto iter = s_typeSet.begin(); iter != s_typeSet.end(); ++iter)
		names.push_back(iter.key());
	return names;
}

const QVector<QString>& PatternImageInfo::attributeTypes(const QString& name)
{
	return s_typeSet[name];
}
