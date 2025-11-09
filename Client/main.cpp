//#include "client.h"
//#include <QApplication>
//#include "index.h"

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Client::getInstance().show();
//    return a.exec();
//}

#include "client.h"
#include <QApplication>
#include "index.h"
#include "memorypool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client::getInstance().show();
    int result = a.exec();

    // 程序退出时清理内存池
    MemoryPool::getInstance().clear();
    return result;
}
