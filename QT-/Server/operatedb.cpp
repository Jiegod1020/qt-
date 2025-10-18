#include "operatedb.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");      //主机名称
    m_db.setPort(3306);                 //端口号
    m_db.setUserName("root");           //用户名
    m_db.setPassword("7766");           //密码
    m_db.setDatabaseName("mydb2404");   //数据库名
    if (m_db.open()) {
        qDebug() << "数据库连接成功";
    } else {
        qDebug() << "数据连接失败" << m_db.lastError().text();
    }
}

OperateDB::~OperateDB()
{
    m_db.close();
}

bool OperateDB::handleRegist(const char *name, const char *pwd)
{
    if (name == NULL || pwd == NULL) {
        return false;
    }
    //要添加的用户是否已存在
    QString sql = QString("select * from user_info where name='%1'").arg(name);
    qDebug() << "handleRegist 查找用户sql" << sql;
    QSqlQuery q;    //用于执行sql语句的对象
    bool ret = q.exec(sql); //执行sql语句，返回值代表执行是否成功
    if (!ret) {
        return false;
    }
    if (q.next()) { //取结果集中的结果，每next一次取一个，第一次取的就是第一个结果，如果存在代表要添加的用户已存在
        return false;
    }

    sql = QString("insert into user_info(name, pwd) values('%1', '%2')").arg(name).arg(pwd);
    qDebug() << "handleRegist 插入一个用户sql" << sql;
    return q.exec(sql);
}

bool OperateDB::handleLogin(const char *name, const char *pwd)
{
    if (name == NULL || pwd == NULL) {
        return false;
    }
    QString sql = QString("select * from user_info where name='%1' and pwd='%2'").arg(name).arg(pwd);
    QSqlQuery q;
    if (!q.exec(sql)) {
        return false;
    }
    if (!q.next()) {
        return false;
    }
    sql = QString("update user_info set online=1 where name='%1' and pwd='%2'").arg(name).arg(pwd);
    return q.exec(sql);
}

void OperateDB::handleOffline(const char *name)
{
    if (name == NULL) {
        return;
    }
    QString sql = QString("update user_info set online=0 where name='%1'").arg(name);
    QSqlQuery q;
    q.exec(sql);
}

int OperateDB::handleFindUser(const char *name)
{
    if (name == NULL) {
        return -1;
    }
    QString sql = QString("select online from user_info where name='%1'").arg(name);
    QSqlQuery q;
    if (!q.exec(sql)) {
        return -1;
    }
    if (q.next()) {
        return q.value(0).toInt(); //返回0不在线，返回1在线
    }
    return 2;   //用户不存在
}

QStringList OperateDB::handleOnlineUser()
{
    QString sql = "select name from user_info where online=1";
    QSqlQuery q;
    q.exec(sql);
    QStringList result;
    while (q.next()) {
        result.append(q.value(0).toString());
    }
    return result;
}

int OperateDB::handleAddFriend(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return -1;
    }
    QString data = QString(R"(
           select * from friend where
             (
               user_id=(select id from user_info where name='%1')
               and
               friend_id=(select id from user_info where name='%2'))
             or
             (
               user_id=(select id from user_info where name='%2')
               and
               friend_id=(select id from user_info where name='%1')
             )
           )").arg(curName).arg(tarName);
    qDebug() << "handleAddFriend" << data;
    QSqlQuery q;
    q.exec(data);
    if (q.next()) {
        return -2;//已经是好友
    }
    data = QString("select online from user_info where name='%1'").arg(tarName);
    q.exec(data);
    if (q.next()) {
        return q.value(0).toInt(); //1在线，0不在线
    }
    return -1;
}

bool OperateDB::handleAddFriendAgree(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return -1;
    }
    QString data = QString(R"(
       insert into friend(user_id, friend_id)
         select u1.id, u2.id from user_info u1, user_info u2
         where u1.name ='%1' and u2.name='%2'
       )").arg(curName).arg(tarName);
    QSqlQuery q;
    return q.exec(data);
}

QStringList OperateDB::handleOnlineFriend(const char *curName)
{
    QStringList result;
    if (curName == NULL) {
        return result;
    }
    QString data = QString(R"(
           select name from user_info where id in
           (
             select user_id from friend where friend_id=(select id from user_info where name='%1')
             union
             select friend_id from friend where user_id=(select id from user_info where name='%1')
           ) and online=1
           )").arg(curName);
    qDebug() << "handleOnlineFriend query onlien friend" << data;
    QSqlQuery q;
    q.exec(data);
    while (q.next()) {
        result.append(q.value(0).toString());
    }
    return result;
}

bool OperateDB::handleDelFriend(const char *curName, const char *tarName)
{
    if (curName == NULL || tarName == NULL) {
        return false;
    }
    QString data = QString(R"(
           select * from friend where
             (
               user_id=(select id from user_info where name='%1')
               and
               friend_id=(select id from user_info where name='%2'))
             or
             (
               user_id=(select id from user_info where name='%2')
               and
               friend_id=(select id from user_info where name='%1')
             )
           )").arg(curName).arg(tarName);
    qDebug() << "handleDelFriend query friend" << data;
    QSqlQuery q;
    q.exec(data);
    if (!q.next()) {
        return false;
    }
    data = QString(R"(
            delete from friend where
            (
            user_id=(select id from user_info where name='%1')
            and
            friend_id=(select id from user_info where name='%2'))
            or
            (
            user_id=(select id from user_info where name='%2')
            and
            friend_id=(select id from user_info where name='%1')
            )
            )").arg(curName).arg(tarName);
    qDebug() << "handleDelFriend delete friend" << data;
    return q.exec(data);
}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}
