#ifndef MYHEADERS_H
#define MYHEADERS_H

// 用于测试输出
#include <QDebug>
#ifndef MYDEBUG
#define qcout qDebug()
#else
#define qcout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"
#endif

#endif // MYHEADERS_H
