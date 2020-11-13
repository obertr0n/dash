#pragma once

#include <QCanBus> 
#include <string>
#include <QVector>
#include <QByteArray>
#include <QVariant>

#include "DashLog.hpp"
#include "canbus/ICANBus.hpp"

class SocketCANBus : public QObject, public ICANBus
{
    Q_OBJECT
    public:
        SocketCANBus(QString canInterface = "can0");
        ~SocketCANBus();
        static SocketCANBus *get_instance();
        void registerFrameHandler(int id, std::function<void(uint32_t, QByteArray)> callback) override;
        bool writeFrame(QCanBusFrame frame) override;

    private:
        bool socketCANAvailable = false;
        QCanBusDevice *bus;
        std::map<int, std::vector<std::function<void(uint32_t, QByteArray)>>> callbacks;
        QList<QCanBusDevice::Filter> filterList;

    private slots:
        void framesAvailable();
};

