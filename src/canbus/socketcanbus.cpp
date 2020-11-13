#include "canbus/socketcanbus.hpp"

SocketCANBus::SocketCANBus(QString canInterface)
{
    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan"))) {
        DASH_LOG(info) << "[SocketCANBus] 'socketcan' Available";
        socketCANAvailable = true;
        QString errorString;
        bus = QCanBus::instance()->createDevice(
            QStringLiteral("socketcan"), canInterface, &errorString);
        if (!bus) {
            DASH_LOG(error) <<"[SocketCANBus] Error creating CAN device, " << errorString.toStdString();
        } else {
            DASH_LOG(info) <<"[SocketCANBus] Connecting CAN interface";
            bus->connectDevice();
            connect(bus, &QCanBusDevice::framesReceived,
                     this, &SocketCANBus::framesAvailable);
        }
    }
}

SocketCANBus::~SocketCANBus()
{
    DASH_LOG(info) << "[SocketCANBus] Disconnecting and deleting bus";
    if(bus->state() == QCanBusDevice::ConnectedState){
        bus->disconnectDevice();
    }
}

bool SocketCANBus::writeFrame(QCanBusFrame frame)
{
    return bus->writeFrame(frame);
}

SocketCANBus *SocketCANBus::get_instance()
{
    static SocketCANBus bus;
    return &bus;
}

void SocketCANBus::framesAvailable()
{
    int numFrames = bus->framesAvailable();
    if(numFrames>0){
        for(int i = 0; i<numFrames; i++){
            QCanBusFrame frame = bus->readFrame();
            if (callbacks.find(frame.frameId()) != callbacks.end())
            {
                for(auto callback : callbacks[frame.frameId()]){
                    callback(frame.frameId(), frame.payload());
                }
            }
        }
    }
    
} 


void SocketCANBus::registerFrameHandler(int id, std::function<void(uint32_t, QByteArray)> callback)
{
    callbacks[id].push_back(callback);
    QCanBusDevice::Filter filter;
    filter.frameId = id;
    filter.frameIdMask = 0xFFF;
    filter.format=QCanBusDevice::Filter::MatchBaseAndExtendedFormat;
    filter.type = QCanBusFrame::InvalidFrame;
    filterList.append(filter);
    bus->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filterList));
}
