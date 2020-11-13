#pragma once

#include <functional>
#include <string>
#include <QByteArray>
#include <QObject>

enum AhBtnKey
{
    EHU_BC          = 0x01,
    EHU_OK          = 0x6F,
    EHU_SETTINGS    = 0xFF,
    EHU_LEFT        = 0x6D,
    EHU_RIGHT       = 0x6C,
    EHU_AMFM        = 0xE0,
    //    EHU_CDMP3        = 0xE0,
    EHU_1           = 0x31,
    EHU_2           = 0x32,
    EHU_3           = 0x33,
    EHU_4           = 0x34,
    EHU_5           = 0x35,
    EHU_6           = 0x36,
    EHU_7           = 0x37,
    EHU_8           = 0x38,
    EHU_9           = 0x39,
    CIM_LEFT_UP     = 0x81, /* next track */
    CIM_LEFT_DOWN   = 0x82, /* prev track */
    CIM_KNOB_LEFT   = 0x83, /* volume */
    CIM_RIGHT_UP    = 0x91, /* answer call */
    CIM_RIGHT_DOWN  = 0x92, /* reject call */
    CIM_KNOB_RIGHT  = 0x93, /* ..... */
    INVALID_CODE    = 0xA1
};

struct CanFrameMsgDecoder {
    std::string description;
    uint32_t frameID;
    std::function<double (QByteArray)> decoder;
};

struct CanFrameBtnDecoder {
    std::string description;
    uint32_t frameID;
    std::function<AhBtnKey (QByteArray)> decoder;
};

struct FrameDecoders {
    CanFrameBtnDecoder CIM_BTN;
    CanFrameBtnDecoder EHU_BTN;
    CanFrameMsgDecoder COOLANT_TEMP;
    CanFrameMsgDecoder RPM;
    CanFrameMsgDecoder SPEED;
    CanFrameMsgDecoder OUT_TEMP;
};

extern FrameDecoders VehicleFrames;

class AstraHDecoder : public QObject
{
    Q_OBJECT
private:
public:
    AstraHDecoder(){};
    ~AstraHDecoder(){};
    static void btn_press(QByteArray data);

signals:
    void buttonPressed(AhBtnKey key);
};
