#pragma once

#include <functional>
#include <string>
#include <QCanBusFrame>
#include "obd/message.hpp"

struct Command {
    std::string description;
    uint32_t frame;
    std::function<double(Response)> decoder;
};

struct Commands {
    Command LOAD;
    Command COOLANT_TEMP;
    Command RPM;
    Command SPEED;
    Command INTAKE_TEMP;
    Command MAF;
};

extern Commands cmds;
