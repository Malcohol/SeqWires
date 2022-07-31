Also see [TODO.md in BabelWires](https://github.com/Malcohol/BabelWires/blob/main/TODO.md)

SeqWires:
* Support other formats
* Improved handling of event truncation: 
  - Use new group events to denote truncated end and start. 
  - When truncated end meets truncated start, act as though event was not truncated.
* Chords:
  - Have a "sustain until CancelChord" as an optional policy in the chordsFromNotes processor.


Seq2tape:
* Proper plugin initialization for Seq2tape
* Add support for setting name and copyright from commandline.
* Add another text field: info.
* Test Multichannel input and output
* Consider implementing basic WAV support myself, so project works without libsnd.

SMF
* Time signature
* Instruments
* Support Standard MIDI event types
* Yamaha XF chords
* Support Format 2 files.

Processors:
* PolyToChord
* ChordToPoly
* Harmonize
* etc.
