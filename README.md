# qtjack-examples
Example repository for QJack

Example repository for QtJack: https://github.com/majorx234/qtjack
Fork from: https://github.com/jacob3141/qtjack-examples

## Info
Dependencies:

* Qt 5.4 or greater
* QJack, which itself depends on Jack2

On Ubuntu, you need to install dependencies:
```
sudo apt-get install libjack-jackd2-dev
```

On Arch Linux:
```
sudo pacman -S jack2 qt5-base
```

## Build
- build project:
```
git clone https://github.com/majorx234/qtjack-examples.git
cd qtjack-examples
mkdir build
cd build
cmake ..
make
```

## Examples
### Attenuator
- read in an audiostream and multiply amplitude with 0.5
### Mididial
- shows interaction between GUI and Jack Midi
- how to send Midi messages
### mp3player
- shows interaction of Qt objects (QAudioDecoder) and datatypes QAudioFormat and QtJack
- show usages of a ringbuffer to store audio data befor sending it through Jack
### Sinedial
- shows generating of audio data a side from jack process function
- how to do data exchange

## Questions

If you have any questions feel free to drop me a mail me at
```
majorx234@googlemail.com

```
## Thx

Thx to
```
jacob@omg-it.works
```
for the inspiring code.

Happy hacking!
