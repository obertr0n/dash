#pragma once

#include <QMap>
#include <QtWidgets>
#include "openauto/Configuration/Configuration.hpp"

#include "app/bluetooth.hpp"
#include "app/config.hpp"
#include "app/shortcuts.hpp"
#include "app/theme.hpp"

class SettingsTab : public QTabWidget {
    Q_OBJECT

   public:
    SettingsTab(QWidget *parent = nullptr);
};

class GeneralSettingsSubTab : public QWidget {
    Q_OBJECT

   public:
    GeneralSettingsSubTab(QWidget *parent = nullptr);

   private:
    QWidget *settings_widget();
    QWidget *dark_mode_row_widget();
    QWidget *brightness_module_row_widget();
    QWidget *brightness_module_select_widget();
    QWidget *brightness_row_widget();
    QWidget *brightness_widget();
    QWidget *si_units_row_widget();
    QWidget *color_row_widget();
    QWidget *color_select_widget();
    QWidget *mouse_row_widget();
    QWidget *volume_row_widget();
    QWidget *volume_widget();

    Config *config;
    Theme *theme;
    Shortcuts *shortcuts;
};

class LayoutSettingsSubTab : public QWidget {
    Q_OBJECT

   public:
    LayoutSettingsSubTab(QWidget *parent = nullptr);

   private:
    QWidget *settings_widget();
    QWidget *pages_widget();
    QWidget *controls_bar_widget();
    QWidget *quick_view_row_widget();
    QWidget *quick_view_select_widget();
    QWidget *scale_row_widget();
    QWidget *scale_widget();

    Config *config;
    Theme *theme;
};

class BluetoothSettingsSubTab : public QWidget {
    Q_OBJECT

   public:
    BluetoothSettingsSubTab(QWidget *parent = nullptr);

   private:
    QWidget *controls_widget();
    QWidget *scanner_widget();
    QWidget *devices_widget();

    Bluetooth *bluetooth;
    Config *config;
    Theme *theme;
    QMap<BluezQt::DevicePtr, QPushButton *> devices;
};

class ShortcutsSettingsSubTab : public QWidget {
    Q_OBJECT

   public:
    ShortcutsSettingsSubTab(QWidget *parent = nullptr);

   private:
    QWidget *settings_widget();
    QWidget *shortcut_row_widget(QString key, QString name, Shortcut *shortcut);
    QWidget *shortcut_input_widget(QString id, Shortcut *shortcut);

    Theme *theme;
    Config *config;
    Shortcuts *shortcuts;
};
