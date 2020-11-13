#pragma once

#include <QByteArray>
#include "canbus/canframes.hpp"

/* data adapted from https://github.com/megadrifter/Astra-H/blob/master/other%20examples/AstraH_CAN_test_rpm/AstraH_CAN_test_rpm.ino */
double can_speed(QByteArray data) { return data.at(4); }

double can_rpm(QByteArray data) { return ((data.at(2) << 6U) + (data.at(3) >> 2U)); }

double can_temp(QByteArray data) { return (data.at(2) - 40.0); }

/* data from https://github.com/Trueffelwurm/Car-CAN-Message-DB/blob/master/Opel/Astra/H/MS-CAN/Body.md */
double can_tempOut(QByteArray data) {return (data.at(3) / 2.0 - 40.0); }

AhBtnKey can_btn(QByteArray data) 
{
    /* short press */
    if(data.at(0) == 0)
    {
        return static_cast<AhBtnKey>(data.at(1));
    }
    return INVALID_CODE; 
}

