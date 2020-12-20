#include <QPalette>

#include "app/config.hpp"
#include "app/pages/vehicle.hpp"
#include "app/window.hpp"
#include "obd/conversions.hpp"

Gauge::Gauge(units_t units, QFont value_font, QFont unit_font, Gauge::Orientation orientation, int rate,
             std::vector<CanFrameMsgDecoder> frames, int precision, can_decoder_t decoder, QWidget *parent)
: QWidget(parent)
{
    Config *config = Config::get_instance();
    ICANBus *bus = SocketCANBus::get_instance();

    using namespace std::placeholders;
    std::function<void(uint32_t, QByteArray)> callback = std::bind(&Gauge::can_callback, this, std::placeholders::_1, std::placeholders::_2);

    bus->registerFrameHandler(frames[0].frameID, callback);
    DASH_LOG(info)<<"[Gauges] Registered frame handler for id "<<(frames[0].frameID);

    this->si = config->get_si_units();

    this->rate = rate;
    this->precision = precision;

    this->canframes = frames;
    this->decoder = decoder;

    QBoxLayout *layout;
    if (orientation == BOTTOM)
        layout = new QVBoxLayout(this);
    else
        layout = new QHBoxLayout(this);

    value_label = new QLabel(this->null_value(), this);
    value_label->setFont(value_font);
    value_label->setAlignment(Qt::AlignCenter);

    QLabel *unit_label = new QLabel(this->si ? units.second : units.first, this);
    unit_label->setFont(unit_font);
    unit_label->setAlignment(Qt::AlignCenter);

    this->timer = new QTimer(this);
    // connect(this->timer, &QTimer::timeout, [this, bus, cmds]() {
    //     for (auto cmd : cmds) {
    //         bus->writeFrame(cmd.frame);
    //     }
    // });

    connect(config, &Config::si_units_changed, [this, units, unit_label](bool si) {
        this->si = si;
        unit_label->setText(this->si ? units.second : units.first);
        this->value_label->setText(this->null_value());
    });

    layout->addStretch();
    layout->addWidget(value_label);
    layout->addWidget(unit_label);
}

void Gauge::can_callback(uint32_t id, QByteArray payload){
    for(auto frame : canframes) {
        //DASH_LOG(info)<<"Called handler for "<<(frame.description);
        value_label->setText(this->format_value(this->decoder(frame.decoder(payload), this->si)));
    }
}

QString Gauge::format_value(double value)
{
    if (this->precision == 0)
        return QString::number((int)value);
    else
        return QString::number(value, 'f', this->precision);
}

QString Gauge::null_value()
{
    QString null_str = "-";
    if (this->precision > 0)
        null_str += ".-";
    else
        null_str += '-';

    return null_str;
}

void VehiclePage::get_plugins()
{
    for (const QFileInfo &plugin : Config::plugin_dir("vehicle").entryInfoList(QDir::Files)) {
        if (QLibrary::isLibrary(plugin.absoluteFilePath()))
            this->plugins[Config::fmt_plugin(plugin.baseName())] = plugin;
    }
}

VehiclePage::VehiclePage(QWidget *parent) : QTabWidget(parent)
{
    this->tabBar()->setFont(Theme::font_14);
    this->addTab(new DataTab(this), "Car Data");

    // QPushButton *settings_button = new QPushButton(this);
    // settings_button->setFlat(true);
    // settings_button->setIconSize(Theme::icon_24);
    // settings_button->setIcon(Theme::get_instance()->make_button_icon("settings", settings_button));
    // connect(settings_button, &QPushButton::clicked, [this]() { this->dialog->open(); });
    // this->setCornerWidget(settings_button);
}

DataTab::DataTab(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QWidget *driving_data = this->driving_data_widget();
    layout->addWidget(driving_data);
    layout->addWidget(Theme::br(this, true));

    QWidget *env_data = this->temperature_widget();
    layout->addWidget(env_data);

    QSizePolicy sp_left(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp_left.setHorizontalStretch(3);
    driving_data->setSizePolicy(sp_left);
    
    QSizePolicy sp_right(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp_right.setHorizontalStretch(2);
    env_data->setSizePolicy(sp_right);

    for (auto &gauge : this->gauges)
    {
        gauge->start();
    }
}

QWidget *DataTab::driving_data_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addStretch();
    layout->addWidget(this->speedo_tach_widget());
    layout->addStretch();
    layout->addWidget(Theme::br(widget));

    return widget;
}

QWidget *DataTab::speedo_tach_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    Gauge *speed = new Gauge({"mph", "km/h"}, QFont("Titillium Web", 72), QFont("Montserrat", 16, QFont::Light, true),
                             Gauge::BOTTOM, 100, {VehicleFrames.SPEED}, 0,
                             [](double x, bool si) { return si ? x : kph_to_mph(x); }, widget);
    layout->addWidget(speed);
    this->gauges.push_back(speed);

    Gauge *rpm = new Gauge({"x100rpm", "x100rpm"}, QFont("Titillium Web", 72),
                           QFont("Montserrat", 16, QFont::Light, true), Gauge::BOTTOM, 100, {VehicleFrames.RPM}, 1,
                           [](double x, bool _) { return x; }, widget);
    layout->addWidget(rpm);
    this->gauges.push_back(rpm);

    return widget;
}

QWidget *DataTab::temperature_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    layout->addStretch();
    layout->addWidget(this->temperature_data_widget());
    layout->addStretch();
    layout->addWidget(Theme::br(widget));
    layout->addStretch();

    return widget;
}

QWidget *DataTab::temperature_data_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    Gauge *outside_temp = new Gauge(
        {"°F", "°C"}, QFont("Titillium Web", 36), QFont("Montserrat", 14, QFont::Light, true), Gauge::RIGHT, 5000,
        {VehicleFrames.OUT_TEMP}, 1, [](double x, bool si) { return si ? x : c_to_f(x); }, widget);
    layout->addWidget(outside_temp);
    
    this->gauges.push_back(outside_temp);

    QLabel *outside_temp_label = new QLabel("Outside", widget);
    outside_temp_label->setFont(QFont("Montserrat", 14, QFont::Light));
    outside_temp_label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(outside_temp_label);

    Gauge *coolant_temp = new Gauge(
        {"°F", "°C"}, QFont("Titillium Web", 36), QFont("Montserrat", 14, QFont::Light, true), Gauge::RIGHT, 5000,
        {VehicleFrames.COOLANT_TEMP}, 1, [](double x, bool si) { return si ? x : c_to_f(x); }, widget);
    layout->addWidget(coolant_temp);
    
    this->gauges.push_back(coolant_temp);

    QLabel *coolant_temp_label = new QLabel("Coolant", widget);
    coolant_temp_label->setFont(QFont("Montserrat", 14, QFont::Light));
    coolant_temp_label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(coolant_temp_label);

    return widget;
}

QWidget *DataTab::engine_data_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addStretch();
    layout->addWidget(this->coolant_temp_widget());
    layout->addStretch();
    layout->addWidget(Theme::br(widget));
    layout->addStretch();

    return widget;
}

QWidget *DataTab::coolant_temp_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    Gauge *coolant_temp = new Gauge(
        {"°F", "°C"}, QFont("Titillium Web", 36), QFont("Montserrat", 14, QFont::Light, true), Gauge::RIGHT, 5000,
        {VehicleFrames.COOLANT_TEMP}, 1, [](double x, bool si) { return si ? x : c_to_f(x); }, widget);
    layout->addWidget(coolant_temp);
    this->gauges.push_back(coolant_temp);

    QLabel *coolant_temp_label = new QLabel("Coolant", widget);
    coolant_temp_label->setFont(QFont("Montserrat", 14, QFont::Light));
    coolant_temp_label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(coolant_temp_label);

    return widget;
}

QWidget *DataTab::outside_temp_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    Gauge *outside_temp = new Gauge(
        {"°F", "°C"}, QFont("Titillium Web", 36), QFont("Montserrat", 14, QFont::Light, true), Gauge::RIGHT, 5000,
        {VehicleFrames.OUT_TEMP}, 1, [](double x, bool si) { return si ? x : c_to_f(x); }, widget);
    layout->addWidget(outside_temp);
    this->gauges.push_back(outside_temp);

    QLabel *outside_temp_label = new QLabel("Outside", widget);
    outside_temp_label->setFont(QFont("Montserrat", 14, QFont::Light));
    outside_temp_label->setAlignment(Qt::AlignHCenter);
    layout->addWidget(outside_temp_label);

    return widget;
}

