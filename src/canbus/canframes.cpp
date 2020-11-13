#include "canbus/canframes.hpp"

FrameDecoders VehicleFrames = 
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
            "CIM Buttons", 
            0x0206,
            can_btn
        },
        {
            "EHU Buttons", 
            0x0201,
            can_btn
        },
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