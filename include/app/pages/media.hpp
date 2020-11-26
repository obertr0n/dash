#pragma once

#include <QMediaPlayer>
#include <QtWidgets>

#include "app/bluetooth.hpp"
#include "app/config.hpp"
#include "app/theme.hpp"
#include "app/widgets/tuner.hpp"
#include "canbus/canframes.hpp"
#include "app/window.hpp"

class MediaPage : public QTabWidget {
    Q_OBJECT
   public:
    MediaPage(DashWindow *parent, std::vector<CanFrameBtnDecoder> decoders);
    void can_callback(uint32_t id, QByteArray payload);
   private:
    DashWindow* parent_;
    Bluetooth* bluetooth_;
    QMediaPlayer* mediaPlayer_;
    std::vector<CanFrameBtnDecoder> decoders_;
};

class BluetoothPlayerTab : public QWidget {
    Q_OBJECT

   public:
    BluetoothPlayerTab(QWidget *parent, Bluetooth *bluetooth);
   private:
    QWidget *track_widget();
    QWidget *controls_widget();

    Bluetooth *bluetooth;
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
    LocalPlayerTab(QWidget *parent, QMediaPlayer* player);
    static QString durationFmt(int total_ms);

   private:
    QWidget *playlist_widget();
    QWidget *seek_widget();
    QWidget *controls_widget();
    void populate_dirs(QString path, QListWidget *dirs_widget);
    void populate_tracks(QString path, QListWidget *tracks_widget);

    Config *config;
    QMediaPlayer *player;
    QLabel *path_label;
};
