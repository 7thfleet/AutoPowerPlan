#include "powerscheme.h"

#include <QString>
#include <QDataStream>
#include <QSettings>

#include "rpc.h"
#include "rpcdce.h"
PowerScheme::PowerScheme(QString friendlyName, GUID powerGUID) :
    friendlyName(friendlyName),
    schemeGUID(powerGUID)
{}

PowerScheme::~PowerScheme(){}




