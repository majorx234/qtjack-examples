#include <QCoreApplication>

#include <qtjack/Client>
#include <qtjack/Processor>
#include <qtjack/RingBuffer>

class MyProcessor : public QtJack::Processor {
public:
    MyProcessor(QtJack::Client& client)
        : Processor(client)  {
        in = client.registerAudioInPort("in");
        out = client.registerAudioOutPort("out");
        ringBuffer = QtJack::AudioRingBuffer();
    }

    void process(int samples) {
        in.buffer(samples).push(ringBuffer);
        out.buffer(samples).pop(ringBuffer);
        out.buffer(samples).multiply(0.5);
    }

private:
    QtJack::AudioPort in;
    QtJack::AudioPort out;
    QtJack::AudioRingBuffer ringBuffer;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QtJack::Client client;
    client.connectToServer("attenuator_demo");

    MyProcessor processor(client);
    client.activate();

    return a.exec();
}