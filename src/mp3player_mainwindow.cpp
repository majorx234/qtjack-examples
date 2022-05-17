///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                            //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
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

// Own includes
#include "mp3player_mainwindow.hpp"
#include "ui_mp3player_mainwindow.h"

// Qt includes
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

Mp3PlayerMainWindow::Mp3PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    Processor(_client),
    ui(new Ui::Mp3PlayerMainWindow) {

    ui->setupUi(this);
    setupJackClient();
    setupMp3Decoder();

    connect(&_audioDecoder, SIGNAL(bufferReady()),
            this, SLOT(transferSamples()));

    connect(&_audioDecoder, SIGNAL(error(QAudioDecoder::Error)),
            this, SLOT(decodingError(QAudioDecoder::Error)));

    startTimer(2000);
}

Mp3PlayerMainWindow::~Mp3PlayerMainWindow() {
    delete ui;
}

void Mp3PlayerMainWindow::on_toolButtonFileChoose_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open mp3 file", QString(), "*.mp3");
    if(!fileName.isEmpty()) {
        ui->lineEditFileName->setText(fileName);
        _audioDecoder.setSourceFilename(fileName);
        _audioDecoder.start();
    }
}

void Mp3PlayerMainWindow::setupJackClient() {
    // Connect to JACK server
    _client.connectToServer("mp3_player");

    // Create a 1000 seconds buffer (at 44,1kHz)
    _ringBufferLeft  = QtJack::AudioRingBuffer(44100 * 1000);
    _ringBufferRight = QtJack::AudioRingBuffer(44100 * 1000);

    // Register two output ports
    _outLeft     = _client.registerAudioOutPort("out_1");
    _outRight    = _client.registerAudioOutPort("out_2");

    _client.setMainProcessor(this);

    // Activate client.
    _client.activate();

    // Connect automatically to system playback.
    _client.connect(_outLeft, _client.portByName("system:playback_1"));
    _client.connect(_outRight, _client.portByName("system:playback_2"));
}

void Mp3PlayerMainWindow::setupMp3Decoder() {
    QAudioFormat targetAudioFormat;
    targetAudioFormat.setSampleRate(_client.sampleRate());
    targetAudioFormat.setSampleType(QAudioFormat::SignedInt);
    targetAudioFormat.setChannelCount(2);
    targetAudioFormat.setCodec("audio/x-raw");
    _audioDecoder.setAudioFormat(targetAudioFormat);
}


void Mp3PlayerMainWindow::decodingError(QAudioDecoder::Error error) {
    switch(error) {
        case QAudioDecoder::NoError:
            break;
        case QAudioDecoder::ResourceError:
            QMessageBox::warning(this, "Error", "A media resource couldn't be resolved.");
            break;
        case QAudioDecoder::FormatError:
            QMessageBox::warning(this, "Error", "The format of a media resource isn't supported.");
            break;
        case QAudioDecoder::AccessDeniedError:
            QMessageBox::warning(this, "Error", "There are not the appropriate permissions to play a media resource.");
            break;
        case QAudioDecoder::ServiceMissingError:
            QMessageBox::warning(this, "Error", "A valid playback service was not found, playback cannot proceed.");
            break;
        default:
            QMessageBox::warning(this, "Error", "Decoding failed with an unknown error.");
            break;
    }
}

void Mp3PlayerMainWindow::transferSamples() {
    QAudioBuffer audioBuffer = _audioDecoder.read();
    if(audioBuffer.isValid()) {
        int frames = audioBuffer.frameCount();
        QtJack::AudioSample left[frames];
        QtJack::AudioSample right[frames];


        const QAudioBuffer::S16S *stereoBuffer = audioBuffer.constData<QAudioBuffer::S16S>();
        for (int i = 0; i < frames; i++) {
            left[i]     = (QtJack::AudioSample)(stereoBuffer[i].left / 65536.0);
            right[i]    = (QtJack::AudioSample)(stereoBuffer[i].right / 65536.0);
        }

        _ringBufferLeft.write(left, frames);
        _ringBufferRight.write(right, frames);

    }
}


void Mp3PlayerMainWindow::timerEvent(QTimerEvent*) {
    qDebug() << "Samples remaining in the ring buffer: "
             << _ringBufferLeft.numberOfElementsAvailableForRead()
             << " / "
             << _ringBufferRight.numberOfElementsAvailableForRead();
}

void Mp3PlayerMainWindow::process(int samples) {
    // Just shift samples from the ringbuffers to the outputs buffers.
    _outLeft.buffer(samples).pop(_ringBufferLeft);
    _outRight.buffer(samples).pop(_ringBufferRight);
}
