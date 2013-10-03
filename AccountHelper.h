#ifndef ACCOUNTHELPER_H
#define ACCOUNTHELPER_H

#include <QObject>

#include <Accounts/Account>
#include <Accounts/Service>
#include <Accounts/Manager>
#include <Accounts/AccountService>
#include <SessionData>
#include <IdentityInfo>
#include <Identity>
#include <AuthSession>
#include <AuthService>
using namespace Accounts;
using namespace SignOn;

class AccountHelper : public QObject
{
    Q_OBJECT
public:
    explicit AccountHelper(QObject *parent = 0);
    
    void createAccount();
    void createCredentials(); 
    void getCredentials();
    void query();
    void listAccounts();
    void addSetting(const QString key, const QString value);
    void addSetting(Accounts::AccountId id, const QString key, const QString value);
    void settings(Accounts::AccountId id);
    
public slots:
    void accountCreated(Accounts::AccountId id);
    void methodsAvailable(const QStringList &mechs);
    void mechanismsAvailable(const QString &method, const QStringList &mechs);
    void credentialsStored(const quint32 id);
    void error(const SignOn::Error &err);
    void credentialsResponse(const SessionData& data);
    void credentialsError(const SignOn::Error& err);
    
private:
    Manager* mMgr;
    Account* mAcc;
    AccountService* mAccService;
    
    AuthService* mAuthService;
    Identity* mIdentity;
    IdentityInfo* mIdentityInfo;
    AuthSession* mSession;
    SessionData* mSessionData;    
};

#endif // ACCOUNTHELPER_H
