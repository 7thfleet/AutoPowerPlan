/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
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
