#pragma once

#include <QMediaPlayer>
#include <QtWidgets>

#include "app/bluetooth.hpp"
#include "app/config.hpp"
#include "app/theme.hpp"
#include "app/widgets/tuner.hpp"
#include "canbus/canframes.hpp"

class MediaPage : public QTabWidget {
    Q_OBJECT

   public:
    MediaPage(QWidget *parent = nullptr);
};

class BluetoothPlayerTab : public QWidget {
    Q_OBJECT

   public:
    BluetoothPlayerTab(QWidget *parent, std::vector<CanFrameBtnDecoder> decs);
    void can_callback(uint32_t id, QByteArray payload);
   private:
    QWidget *track_widget();
    QWidget *controls_widget();

    Bluetooth *bluetooth;
    std::vector<CanFrameBtnDecoder> framedecs;
};

class RadioPlayerTab : public QWidget {
    Q_OBJECT

   public:
    RadioPlayerTab(QWidget *parent = nullptr);
    void can_callback(QByteArray payload);
   private:
    QWidget *tuner_widget();
    QWidget *controls_widget();

    Config *config;
    Theme *theme;
    Tuner *tuner;
};

class LocalPlayerTab : public QWidget {
    Q_OBJECT

   public:
    LocalPlayerTab(QWidget *parent, std::vector<CanFrameBtnDecoder> decs);

    static QString durationFmt(int total_ms);
    void can_callback(uint32_t id, QByteArray payload);
   private:
    QWidget *playlist_widget();
    QWidget *seek_widget();
    QWidget *controls_widget();
    void populate_dirs(QString path, QListWidget *dirs_widget);
    void populate_tracks(QString path, QListWidget *tracks_widget);

    Config *config;
    QMediaPlayer *player;
    QLabel *path_label;
    std::vector<CanFrameBtnDecoder> framedecs;
};
