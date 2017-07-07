#ifndef POWERSCHEME_H
#define POWERSCHEME_H
#include <QString>

#include "rpc.h"
#include "rpcdce.h"

//Object that describes a windows power plan/scheme
struct PowerScheme{
public:
    PowerScheme(QString friendlyName, GUID powerGUID);
      const QString friendlyName;
      const GUID schemeGUID;
    ~PowerScheme();
};


#endif // POWERSCHEME_H
