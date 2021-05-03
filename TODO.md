Also see [TODO.md in BabelWires](https://github.com/Malcohol/BabelWires/blob/main/TODO.md)

SeqWires:
* Support other formats

Seq2tape:
* Proper plugin initialization for Seq2tape
* Add support for setting name and copyright from commandline.
* Add another text field: info.
* Test Multichannel input and output
* Consider implementing basic WAV support myself, so project works without libsnd.

SMF
* Handle track 0 for Format 1 files specially. Don't allow event data to be added.
* Time signature (Needs support for optional features)
* Instruments (Needs support for optional features)
* Support Standard MIDI event types
* Yamaha XF chords
* Add unit tests for SMF handling. Put UnitTests beside plugin.

Processors:
* Concatenate
* Transpose
* PolyToChord
* ChordToPoly
* Harmonize
* PolySplitter
* TrackMerger
* etc.
