#pragma once

#include <functional>
#include <string>
#include <QCanBusFrame>
#include "obd/message.hpp"

struct Command {
    std::string description;
    uint32_t frameID;
    std::function<double(QByteArray)> decoder;
};

struct Commands {
    Command COOLANT_TEMP;
    Command RPM;
    Command SPEED;
    Command OUT_TEMP;
};

extern Commands cmds;
