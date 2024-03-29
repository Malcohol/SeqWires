# SeqWires

SeqWires is an application for converting between music sequencer formats.

As well as getting track data out of an input format and putting it in an output format, some support for transforming that data is provided. 
SeqWires attempts to support many common music transformation scenarios with a convenient and intuitive user interface.
It does not attempt to support every possible transformation of music data and nor does it try to be a visual programming language.

The following screenshot shows an example of music data processing in SeqWires.
The source file has two tracks: a melody and a simple accompanyment.
The project repeats the melody, and adds an offset and transposed copy of it.
The result is like a simple [musical round](https://en.wikipedia.org/wiki/Round_(music)).

![Screenshot showing several nodes wired together](https://raw.githubusercontent.com/Malcohol/BabelWires/main/Docs/screenshot.png "An example SeqWires project")

Here's a screenshot of the MapEditor defining a map between percussion instruments:

![Screenshot showing the MapEditor](https://raw.githubusercontent.com/Malcohol/BabelWires/main/Docs/mapEditor.png "Screenshot of the MapEditor defining a map between percussion instruments")

Right now, the supported formats are:
* SMF (standard MIDI file)
* Many more formats to come :D 

See also, the [FAQ](Docs/FAQ.md).

## Seq2tape

Many pre-MIDI or low-end sequencers and drum machines use cassette tapes to store their sequence data.
This is achieved by converting the device's digital data to an analog audio signal which can be recorded onto or played back from tape.
Seq2tape is a utility for converting the audio which these devices use to data files which SeqWires can use.
It depends on per-device plugins which know how the device represents data as audio.
Currently, no plugins are included in the code base, but please watch this space :)

Ultimately, I do intend to include functionality for interacting with audio storage formats within SeqWires, which is why the Seq2tape code is part of this project.
However, even when I do so, there will be still be a use-case for the stand-alone seq2tape application.

## Status

[![SeqWires build and test pipeline](https://github.com/Malcohol/SeqWires/actions/workflows/ci.yml/badge.svg)](https://github.com/Malcohol/SeqWires/actions/workflows/ci.yml)

SeqWires is intended to be cross platform. It should build on GNU/Linux, Windows and Mac OS but quality is inconsistent: see the table at [BabelWires status](https://github.com/Malcohol/BabelWires/blob/main/README.md#Status) for more details.

Right now the SMF plugin only supports note on/off events.
My current priority is on getting the infrastructure right.

## Downloading and Building

Clone recursively to ensure submodules are populated:

```
git clone --recurse-submodules https://github.com/Malcohol/SeqWires.git
```

SeqWires is implemented using the [BabelWires](https://github.com/Malcohol/BabelWires) framework (which in turn makes heavy use of the [QT Node Editor](https://github.com/paceholder/nodeeditor) project). 

# License

SeqWires is licensed under the GPLv3.
See the [LICENSE](LICENSE) file.
