#include "obd/command.hpp"
#include "obd/decoders.hpp"

Commands cmds = 
    {
        // {
        //     "DIS Main Text", 
        //     0x06C1,
        //     percentage
        // },
        // {
        //     "DIS AC Text",
        //     0x06C8,
        //     flow
        // },
        {
            "Engine Coolant Temperature", 
            0x04EC,
            can_temp
        },
        {
            "Engine Revolutions Per Minute (RPM)", 
            0x04E8,
            can_rpm
        },
        {
            "Vehicle Speed",
            0x04E8,
            can_speed
        },
        {
            "Outside Air Temperature",
            0x0683,
            can_tempOut
        }
    };
