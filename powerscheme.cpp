/*
Copyright (C) {2017} {7thfleet}

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*/
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




