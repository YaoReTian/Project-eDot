#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <QJsonObject>


class JsonParser
{
public:
    static QJsonObject parse(QString filename);
    static QString parsePath(QString filepath);
};

#endif // JSONPARSER_H
