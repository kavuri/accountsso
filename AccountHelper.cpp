#include "AccountHelper.h"
#include <QDebug>

AccountHelper::AccountHelper(QObject *parent) :
    QObject(parent), mMgr(new Manager()), mAcc(0), mAccService(0), mAuthService(new AuthService()),
    mIdentity(0), mIdentityInfo(0), mSession(0), mSessionData(0)
{
    connect(mAuthService, SIGNAL(methodsAvailable (const QStringList &)),
            this, SLOT(methodsAvailable(const QStringList&)));
    connect(mAuthService, SIGNAL(mechanismsAvailable (const QString&, const QStringList &)),
            this, SLOT(mechanismsAvailable(const QString&,const QStringList&)));
    
    connect(mMgr, SIGNAL(accountCreated(Accounts::AccountId)),
            this, SLOT(accountCreated(Accounts::AccountId)));
}

void
AccountHelper::methodsAvailable (const QStringList &mechs)
{
    qDebug("methodsAvailable");
    for (int i = 0; i < mechs.size(); ++i) {
        qDebug() << mechs.at(i).toLocal8Bit().constData() << endl;
        mAuthService->queryMechanisms(mechs.at(i));
    }
}

void
AccountHelper::mechanismsAvailable (const QString &method, const QStringList &mechs)
{
    qDebug("mechanismsAvailable");
    qDebug() << method;
    for (int i = 0; i < mechs.size(); ++i) {
        qDebug() << mechs.at(i).toLocal8Bit().constData() << endl;
    }
}

void
AccountHelper::accountCreated(AccountId id)
{
    qDebug() << "Account created with id:" << id;
}

void
AccountHelper::query ()
{
    mAuthService->queryMethods();
}

void
AccountHelper::createAccount ()
{
    mAcc = mMgr->createAccount("email");
    mAcc->setDisplayName("my email account");
    mAcc->setEnabled(true);
    Service ser = mMgr->service("email");
    mAcc->selectService(ser);
    mAcc->syncAndBlock();
}

void
AccountHelper::credentialsStored (const quint32 id)
{
    qDebug() << "Stored credentials with id:" << id;
    mAcc->setCredentialsId(id);
    mAcc->syncAndBlock();
    
    getCredentials ();
}

void
AccountHelper::error (const SignOn::Error &err)
{
    qDebug() << "REceived error";
}

void
AccountHelper::credentialsResponse (const SessionData& sessionData)
{
    qDebug() << "Credentials response";
    QVariantMap data;
    QStringList sdpns = sessionData.propertyNames();
    foreach (const QString &sdpn, sdpns) {
        data.insert(sdpn, sessionData.getProperty(sdpn));
        qDebug() << sdpn << sessionData.getProperty(sdpn);
    }

    QString accessToken;
    SignOn::AuthSession *session = qobject_cast<SignOn::AuthSession *>(sender());
    int accountId = static_cast<int>(session->property("accountId").toUInt());
    qDebug() << "ACC ID:" << accountId;

    if (data.contains(QLatin1String("AccessToken"))) {
        accessToken = data.value(QLatin1String("AccessToken")).toString();
        qDebug () << "ACCESS TOKEN AVAILABLE:" << accessToken;
    } else {
        qDebug () << "$$$NO ACCESS TOKEN";
    }
}

void
AccountHelper::credentialsError (const SignOn::Error &error)
{
    qDebug("session Err: %d", error.type());
    qDebug() << error.message();
}

void
AccountHelper::createCredentials ()
{
    QMap<MethodName, MechanismsList> methods;
    QStringList mechs = QStringList() << QString::fromLatin1 ("password");
    methods.insert (QLatin1String("password"), mechs);
    mIdentityInfo = new IdentityInfo(QLatin1String("jolla"),
                                     QLatin1String("secret"),
                                     methods);
    mIdentityInfo->setSecret(QLatin1String("sfsddfd"));
    mIdentityInfo->setType(IdentityInfo::Application);
    mIdentity = Identity::newIdentity(*mIdentityInfo);
    
    connect(mIdentity, SIGNAL(credentialsStored(const quint32)),
            this, SLOT(credentialsStored(const quint32)));
    connect(mIdentity, SIGNAL(error(const SignOn::Error &)),
            this, SLOT(error(const SignOn::Error &)));
    
    mIdentity->storeCredentials();
}

void
AccountHelper::getCredentials ()
{
    quint32 credId = mAcc->credentialsId();
    qDebug() << "Getting credentials for cred id:" << credId;
    Identity* ident = Identity::existingIdentity(credId);
    SessionData data;
    
    mSession = ident->createSession(QLatin1String("password"));
    connect (mSession, SIGNAL(response(const SignOn::SessionData &)),
        this, SLOT(credentialsResponse(const SessionData&)));

    connect(mSession, SIGNAL(error(const SignOn::Error &)),
        this, SLOT(credentialsError(const Error&)));

    mSession->process(data, QLatin1String("password"));
}

void
AccountHelper::listAccounts ()
{
    AccountIdList accList = mMgr->accountList();
    qDebug() << "All accounts";
    foreach (AccountId id, accList) {
        qDebug() << id;
    }
}
