#pragma once

#include <functional>
#include <string>
#include <QCanBusFrame>
#include "obd/message.hpp"

struct Frame {
    std::string description;
    uint32_t frameID;
    std::function<double(QByteArray)> decoder;
};

struct Frames {
    Frame COOLANT_TEMP;
    Frame RPM;
    Frame SPEED;
    Frame OUT_TEMP;
};

extern Frames VehicleFrames;
