
SeqWires:
* Support other formats

SeqxTape:
* Proper plugin initialization for Seq2tape
* Add support for setting name and copyright from commandline.
* Add another text field: info.
* Test Multichannel input and output
* Consider implementing basic WAV support myself, so project works without libsnd.

SMF
* Time signature (Needs support for optional features)
* Instruments (Needs support for optional features)
* Support Standard MIDI event types
* Yamaha XF chords
* Restructure data
* If BabelWires splits up import/export then SMF could have:
  - record-based import with channel-number fields which is robust to changes
  - array-based export which is flexible
* Add unit tests for SMF handling.
