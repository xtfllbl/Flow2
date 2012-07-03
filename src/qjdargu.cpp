#include "qjdargu.h"
#include <stdlib.h>


/// 保存一些时常需要改动的参数，集合起来方便修改
QJDArgu::QJDArgu(QObject *parent) :
    QObject(parent)
{
    // 这个需要改一下,要用环境变量来替代
    JD_FlowDataDir=getenv("JDFLOWDATA");  // /home/xtf/Data
    JD_FlowDir=getenv("JDFLOWDIR");  // /home/xtf/Data
    JD_FileType=getenv("FILETYPE");
}
