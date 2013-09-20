#include <QDebug>
#include <QCoreApplication>
#include <AccountHelper.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    AccountHelper ah;
    ah.createAccount ();
    ah.createCredentials ();
    ah.listAccounts ();
    return a.exec();
}
