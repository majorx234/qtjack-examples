///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                            //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                  2022 <majorx234@googlemail.com>                          //
//    QtJack is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QtJack is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtJack. If not, see <http://www.gnu.org/licenses/>.          //
//                                                                           //
//    It is possible to obtain a closed-source license of QtJack.             //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <QtWidgets/QMainWindow>
#include <Client>
#include <QAbstractSlider>
#include "sine_wave.hpp"
#include "sinedial_mainwindow.hpp"

SineDialMainWindow::SineDialMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Processor(_client)
    , sinedial_ui(new Ui::Sinedial)
    , sample_rate(48000)
    , generated_samples_per_tick(1024)
    , samples_offset(0)
{
    left = (QtJack::AudioSample*)malloc(generated_samples_per_tick* sizeof(QtJack::AudioSample));
    right = (QtJack::AudioSample*)malloc(generated_samples_per_tick* sizeof(QtJack::AudioSample));
    sinedial_ui->setupUi(this);
    slider_base_freq = sinedial_ui->dial_base_freq;
    slider_mod_freq = sinedial_ui->dial_mod_freq;
    slider_base_freq->setAttribute(Qt::WA_AcceptTouchEvents);
    slider_mod_freq->setAttribute(Qt::WA_AcceptTouchEvents);

    setupJackClient();
    connect(slider_base_freq, &QDial::valueChanged, this, &SineDialMainWindow::on_twist);
    connect(slider_mod_freq, &QDial::valueChanged, this, &SineDialMainWindow::on_twist);
}

SineDialMainWindow::~SineDialMainWindow() {
    delete left;
    delete right;
    _client.disconnectFromServer();
}

void SineDialMainWindow::setupJackClient() {
    _client.connectToServer("sinedial");
    _audio_out[0] = _client.registerAudioOutPort("out_1");
    _audio_out[1] = _client.registerAudioOutPort("out_2");
    _audio_ring_buffer[0] = new QtJack::AudioRingBuffer();
    _audio_ring_buffer[1] = new QtJack::AudioRingBuffer();
    _client.setMainProcessor(this);
    _client.activate();
}

void SineDialMainWindow::on_twist() {
    _value_base_freq = sinedial_ui->dial_base_freq->value();
    _value_mod_freq = sinedial_ui->dial_mod_freq->value();
}

void SineDialMainWindow::process(int samples) {
    QtJack::AudioBuffer port_buffer_0 = _audio_out[0].buffer(samples);
    QtJack::AudioBuffer port_buffer_1 = _audio_out[1].buffer(samples);
}

void SineDialMainWindow::sine_wave_generate_function() {
  sine_wave(left,
            _value_base_freq,
            _value_mod_freq,
            generated_samples_per_tick,
            samples_offset,
            sample_rate);
  sine_wave(right,
            _value_base_freq,
            _value_mod_freq,
            generated_samples_per_tick,
            samples_offset,
            sample_rate);
}
