#pragma once

#include <QVector>
#include <QString>
#include <QMap>
#include <QSet>
#include <qxml.h>
#include <qxmlstream.h>
#include "tinyxml\tinyxml.h"
#include <QPixmap>
class PatternImageInfo
{
public:
	PatternImageInfo();
	~PatternImageInfo();

	void clear();
	int numImages()const;
	QString getImageName(int i)const;
	QPixmap* getImage(int i)const;
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

	void setJdId(const QString& s) { m_jdId = s; }
	QString getJdId()const { return m_jdId; }
	void setJdTitle(const QString& s) { m_jdTitle = s; }
	QString getJdTitle()const { return m_jdTitle; }
	void setJdMappedPattern(const QString& s) { m_jdMappedPatternName = s; }
	QString getJdMappedPattern()const { return m_jdMappedPatternName; }
public:
	static bool initialized() { return s_mapInitialized; }
	static int numAttributes() { return (int)s_typeSet.size(); }
	static QVector<QString> attributeNames();
	static const QVector<QString>& attributeTypes(const QString& name);
	static QVector<QString> jdAttributeNames();
	static int numJdAttributes() { return (int)s_jd2typeMap.size(); }
	static QPair<QString, QString> jdAttributeMapped(QString jdAttName, QString jdType);
	static void addJdAttributeMap(QString jdAttName, QString attName, QString jdType, QString type);
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

	/// for jd images
	QString m_jdTitle;
	QString m_jdId;
	QString m_jdMappedPatternName;
	struct JdTypeMapVal
	{
		QString mappedName;
		QMap<QString, QString> typeMap;
	};
	static QMap<QString, JdTypeMapVal> s_jd2typeMap;
};