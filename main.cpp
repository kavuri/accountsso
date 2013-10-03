#include <QDebug>
#include <QCoreApplication>
#include <AccountHelper.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    AccountHelper ah;
    //ah.createAccount ();
    //ah.addSetting ("Remote database", "http://sync.memotoo.com/syncml");
    //ah.addSetting ("remote_service_name", "memotoo.com");
    //ah.createCredentials ();
    ah.addSetting(QString(argv[1]).toUInt(), QString(argv[2]), QString(argv[3]));
    ah.listAccounts ();
    ah.settings(QString(argv[1]).toUInt());
    return a.exec();
}
