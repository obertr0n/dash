#include "obd/command.hpp"
#include "obd/decoders.hpp"

Commands cmds = {
    {
        "DIS Module Text", 
        0x06C1, 
        percentage
    },

    {
        "Engine Coolant Temperature", 
        0x04EC,
        temp
    },

    {
        "Engine Revolutions Per Minute (RPM)",
        0x04E8,
        rpm
    },

    {
        "Vehicle Speed",
        0x04E8,
        speed
    },

    {
        "Outside Air Temperature", 
        0x0683, 
        temp
    },

    {
        "DIS AC Field Text", 
        0x06C8, 
        flow
    }
};

