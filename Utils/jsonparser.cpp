#include "jsonparser.h"

#include <QFile>
#include <QJsonDocument>

QJsonObject JsonParser::parse(QString filename)
{
    QString fileContent;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    fileContent = file.readAll();
    file.close();
    return QJsonDocument::fromJson(fileContent.toUtf8()).object();
}

QString JsonParser::parsePath(QString filepath)
{
    int pos = filepath.lastIndexOf("/");;

    if (filepath.endsWith(".tsx"))      return (":/tileset/res" + filepath.sliced(pos)).replace(".tsx",".tsj");
    if (filepath.contains("Tiles"))     return ":/image/tile/res" + filepath.sliced(pos);
    if (filepath.contains("Sprites"))   return ":/image/sprite/res" + filepath.sliced(pos);
    if (filepath.contains("Bullets"))   return ":/image/bullet/res" + filepath.sliced(pos);

    qDebug() << "ERROR: filepath not found";
    return "";
}
