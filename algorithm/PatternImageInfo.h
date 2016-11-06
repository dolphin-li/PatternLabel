#pragma once

#include <QVector>
#include <QString>
#include <QMap>
#include <QSet>
#include <qxml.h>
#include <qxmlstream.h>
#include "tinyxml\tinyxml.h"
class PatternImageInfo
{
public:
	PatternImageInfo();
	~PatternImageInfo();

	void clear();
	int numImages()const;
	QString getImageName(int i)const;
	void clearImages();
	void addImage(const QString& name);
	void setBaseName(const QString& name);
	QString getBaseName()const;
	void setUrl(const QString& url);
	QString getUrl()const;
	QString getAttributeType(const QString& typeName)const;
	void setAttributeType(const QString& typeName, const QString& type);
	bool toXml(QXmlStreamWriter& writer)const;
	bool fromXml(QString rootFolder, QXmlStreamReader& reader);
	bool toXml(TiXmlNode* writer)const;
	bool fromXml(QString rootFolder, TiXmlElement* reader);
public:
	static bool initialized() { return s_mapInitialized; }
	static int numAttributes() { return (int)s_typeSet.size(); }
	static QVector<QString> attributeNames();
	static const QVector<QString>& attributeTypes(const QString& name);
protected:
	static bool constructTypeMaps();
	static bool constructTypeMaps_tixml(QString filename);
	static bool constructTypeMaps_qxml(QString filename);
	static bool constructTypeMaps_qxml_save(QString filename);
	void setDefaultTypes();
private:
	QString m_baseName;
	QString m_url;
	QVector<QString> m_imgNames;
	QMap<QString, int> m_types;

	static bool s_mapInitialized;
	// E.G., cloth types, collar types, ...
	static QMap<QString, QVector<QString>> s_typeSet;
};