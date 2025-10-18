#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    static OperateDB& getInstance();
    QSqlDatabase m_db;
    void connect(); //连接到数据库
    ~OperateDB();
    bool handleRegist(const char* name, const char* pwd);
    bool handleLogin(const char* name, const char* pwd);
    void handleOffline(const char* name);
    int handleFindUser(const char* name);
    QStringList handleOnlineUser();
    int handleAddFriend(const char* curName, const char* tarName);
    bool handleAddFriendAgree(const char* curName, const char* tarName);
    QStringList handleOnlineFriend(const char* curName);
    bool handleDelFriend(const char* curName, const char* tarName);
private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB(const OperateDB& instance) = delete;
    OperateDB& operator=(const OperateDB&) = delete;
signals:

};

#endif // OPERATEDB_H
