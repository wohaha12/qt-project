//#include "operatedb.h"
//#include "server.h"

//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Server::getInstance();
//    OperateDB::getInstance().connect();
//    return a.exec();
//}


#include "operatedb.h"
#include "server.h"
#include "databaseconnectionpool.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化数据库连接池，传入数据库连接参数
    DatabaseConnectionPool::getInstance().init("QMYSQL", "localhost", 3307, "mydbqt", "root", "123456");

    Server::getInstance();
    OperateDB::getInstance().connect();

    return a.exec();
}
