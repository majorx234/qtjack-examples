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
#include "mididial_mainwindow.h"

#include "ui_mididial.h"


MidiDialMainWindow::MidiDialMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Processor(_client) 
    , mididial_ui(new Ui::Mididial)
{
    mididial_ui->setupUi(this);
    slider = mididial_ui->dial;
    slider->setAttribute(Qt::WA_AcceptTouchEvents);
    setCentralWidget(slider);
    setupJackClient();
    connect(mididial_ui->dial, &QDial::valueChanged, this, &MidiDialMainWindow::on_twist);
}

MidiDialMainWindow::~MidiDialMainWindow(){
    delete _midi_out_buffer;
    _client.disconnectFromServer();
}

void MidiDialMainWindow::setupJackClient(){
    _client.connectToServer("mididial");
    _midi_out = _client.registerMidiOutPort("out_1");
    _midi_out_buffer = new QtJack::MidiBuffer(); //not used yet
    _client.setMainProcessor(this);
    _client.activate();
}

void MidiDialMainWindow::on_twist() {
    _value = mididial_ui->dial->value();
    _timestamp = _client.getJackFrameTime();
}

void MidiDialMainWindow::process(int samples) {

    jack_nframes_t last_frame_time;
    QtJack::MidiBuffer port_buffer = _midi_out.buffer(samples);
    port_buffer.clearEventBuffer();
    last_frame_time = _client.getJackTime();     
    unsigned int printvalue = _value;
    int timestamp = _timestamp;
    int t = timestamp + samples - last_frame_time;
    if((t > 0) && (t < samples) ){
        unsigned char midiData[3];
        midiData[0] = 0xb1;
        midiData[1] = 10;
        midiData[2] = 0xff & printvalue;
        port_buffer.writeEvent(t, midiData, 3);
    }    
}
