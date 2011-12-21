#include "qjdreadxmlindex.h"

QJDReadXMLIndex::QJDReadXMLIndex(QObject *parent) :
    QObject(parent)
{
    allModuleName.clear();
    allModuleGroup.clear();
    allModulePath.clear();
    allModuleType.clear();
}



void QJDReadXMLIndex::nodeClear()
{
    name.clear();
    group.clear();
    path.clear();
    desc.clear();
    type.clear();
}

bool QJDReadXMLIndex::read(QIODevice *device)
{
//    qDebug()<<"read";
    QString errorStr;
    int errorLine;
    int errorColumn;

    // 错误处理
    if (!domDocument.setContent(device, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "index")   // tag 限定
    {
        qDebug()<<tr("The file is not an index  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an index version 1.0 file.");
        return false;
    }

    /// 这里要改变了,因为为各个模块进行分组了
//    QDomElement modulegroup = root.firstChildElement("modulegroup");  // 不需要这一行了
    QDomElement child=root.firstChildElement("module");
    /// 只处理property,当中有其他的可不行，例如隐藏之类的
    while (!child.isNull())
    {
        nodeClear();  //清理节点包含信息
        name = child.attribute("name");
        group = child.attribute("group");
        parsePropertyElement(child);

        setModule(name,group,path,desc,type);
//        qDebug()<<"readIndexXML::"<<name<<group<<path<<desc<<type;
        child = child.nextSiblingElement("module");
    }
    return true;
}


void QJDReadXMLIndex::parsePropertyElement(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        if (child.tagName() == "module")
        {
            parsePropertyElement(child);  //同根递归
        }
        else if (child.tagName() == "path")
        {
            path=child.text();
        }
        else if (child.tagName() == "desc")
        {
            desc=child.text();
        }
        else if (child.tagName() == "type")
        {
            type=child.text();
        }
        child = child.nextSiblingElement(); //循环
    }
}

void QJDReadXMLIndex::setModule(QString moduleName, QString moduleGroup,
                                 QString modulePath, QString moduleDesc, QString moduleType)
{
    allModuleName.append(moduleName);
    allModuleGroup.append(moduleGroup);
    allModulePath.append(modulePath);
    allModuleDesc.append(moduleDesc);
    allModuleType.append(moduleType);
}

QStringList QJDReadXMLIndex::getModuleName()
{
    return allModuleName;
}

QStringList QJDReadXMLIndex::getModuleGroup()
{
    return allModuleGroup;
}

QStringList QJDReadXMLIndex::getModulePath()
{
    return allModulePath;
}

QStringList QJDReadXMLIndex::getModuleDesc()
{
    return allModuleDesc;
}

QStringList QJDReadXMLIndex::getModuleType()
{
    return allModuleType;
}
