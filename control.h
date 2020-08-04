#ifndef CONTROL_H
#define CONTROL_H

#include <QString>

const QString API = "/api/instances/";
const QString SCHEME = "http";
const QString RUNNING = "RUNNING";
const QString OFFLINE = "OFFLINE";
const QString RESTART = "RESTART";

struct Instance {
    QString name;
    QString status;
};

struct Credential {
    QString host;
    int port;
    QString user;
    QString passwd;
};

#endif // CONTROL_H
