# BabelWires-Music

BabelWires-Music is the Music plugin for [BabelWires](https://github.com/Malcohol/BabelWires). It provides common types for music sequence data. The BabelWires-Music repository also carries plugins for specific music sequence data formats.

The following screenshot shows an example of music data processing in BabelWires.
The source file has two tracks: a melody and a simple accompanyment.
The project repeats the melody, and adds an offset and transposed copy of it.
The result is like a simple [musical round](https://en.wikipedia.org/wiki/Round_(music)).

![Screenshot showing several music nodes wired together](https://raw.githubusercontent.com/Malcohol/BabelWires/main/Docs/screenshot.png "An example BabelWires project using music nodes")

Right now, the supported formats are:
* SMF (Standard MIDI file)
* Many more formats to come :D 

## Seq2tape

Many pre-MIDI or low-end sequencers and drum machines use cassette tapes to store their sequence data.
This is achieved by converting the device's digital data to an analog audio signal which can be recorded onto or played back from tape.
Seq2tape is a utility for converting the audio which these devices use to data files which SeqWires can use.
It depends on per-device plugins which know how the device represents data as audio.
Currently, no plugins are included in the code base, but please watch this space :)

## Status

Right now the SMF plugin only supports note on/off events.
My current priority is on getting the infrastructure right.

# License

BabelWires-Music is licensed under the GPLv3.
See the [LICENSE](LICENSE) file.
