#pragma once

#include <functional>
#include <string>
#include <QCanBusFrame>
#include "obd/message.hpp"

struct CanFrameDecoder {
    std::string description;
    uint32_t frameID;
    std::function<double(QByteArray)> decoder;
};

struct FrameDecoders {
    CanFrameDecoder COOLANT_TEMP;
    CanFrameDecoder RPM;
    CanFrameDecoder SPEED;
    CanFrameDecoder OUT_TEMP;
};

extern FrameDecoders VehicleFrames;
