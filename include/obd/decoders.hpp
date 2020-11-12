#pragma once

#include <obd/message.hpp>

double percentage(Response resp) { return (100.0 / 255.0) * (int)resp.data.at(0); }

double temp(Response resp) { return (int)resp.data.at(0) - 40; }

double rpm(Response resp) { return ((256.0 * (int)resp.data.at(0)) + (int)resp.data.at(1)) / 4.0; }

double speed(Response resp) { return (int)resp.data.at(0); }

double flow(Response resp) { return ((256.0 * (int)resp.data.at(0)) + (int)resp.data.at(1)) / 100.0; }

/* data adapted from https://github.com/megadrifter/Astra-H/blob/master/other%20examples/AstraH_CAN_test_rpm/AstraH_CAN_test_rpm.ino */
double can_speed(QByteArray data) { return data.at(4); }

double can_rpm(QByteArray data) { return ((data.at(2) << 6U) + (data.at(3) >> 2U)); }

double can_temp(QByteArray data) { return (data.at(2) - 40.0); }

/* data from https://github.com/Trueffelwurm/Car-CAN-Message-DB/blob/master/Opel/Astra/H/MS-CAN/Body.md */
double can_tempOut(QByteArray data) {return (data.at(3) / 2.0 - 40.0); }

