#include "bodydata.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qauthenticatorrealm.h>

Responder::Responder(QHttpRequest *req, QHttpResponse *resp)
    : QObject(0)
    , m_req(req)
    , m_resp(resp)
{
    QRegExp exp("^/user/([a-z]+$)");
    if( exp.indexIn(req->path()) != -1 )
    {
        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);
        QString name = exp.capturedTexts()[1];

        QString reply = tr("<html><head><title>BodyData App</title></head><body><h1>Hello  %1!</h1><p>").arg(name);
        resp->write(reply);
        resp->close(QString("</p></body></html>").toLatin1());
    }
    else
    {
        resp->writeHead(403);
        resp->close("You aren't allowed here!");
        // TODO: there should be a way to tell request to stop streaming data
        return;
    }
    connect(m_req, SIGNAL(data(const QByteArray&)), this, SLOT(accumulate(const QByteArray&)));
    connect(req, SIGNAL(end()), this, SLOT(reply()));
    connect(resp, SIGNAL(done()), this, SLOT(deleteLater()));
}

Responder::~Responder()
{
    qDebug() << "DELETING" << m_req;
    delete m_req;
    m_req = 0;
}

void Responder::accumulate(const QByteArray &data)
{
    m_resp->write(data);
}

void Responder::reply()
{
    m_resp->end(QString("</p></body></html>").toLatin1());
}

BodyData::BodyData()
{
    QAuthenticatorRealm* realm = new QAuthenticatorRealm("my-realm");
    realm->addCredential("user", "name");

    QHttpServer *server = new QHttpServer;
    QString certificateFile(":/test.crt");
    QString keyFile(":/test.key");
    server->enableSsl(certificateFile, keyFile);

    server->addAuthenticatorRealm(realm);
    server->listen(QHostAddress::Any, 5000);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handle(QHttpRequest*, QHttpResponse*)));
}

void BodyData::handle(QHttpRequest *req, QHttpResponse *resp)
{
    Responder *r = new Responder(req, resp);
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    BodyData hello;
    
    app.exec();
}
