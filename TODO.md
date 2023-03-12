Also see [TODO.md in BabelWires](https://github.com/Malcohol/BabelWires/blob/main/TODO.md)

SeqWires:
* Support other formats
* Improved handling of event truncation: 
  - Use new group events to denote truncated end and start. 
  - When truncated end meets truncated start, act as though event was not truncated.

Seq2tape:
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
* Harmonize - Attempted to adapt notes to a given chord (This is supported by arranger keyboards)
* First note, last note - Can be combined with the excerpt processor (and possibly quantize) to trim a track.
* etc.
