# SeqWires

SeqWires is an application for converting music sequence data between various music sequencer formats.

As well as getting track data out of an input format and putting it in an output format, some support for processing that data is provided. 
SeqWires attempts to support many common processing use-cases with a convenient and intuitive user interface.
However, it does not attempt to support every possible transformation of music data and nor does it try to be a visual programming language.

Right now, the supported formats are:
* SMF (standard MIDI file)
* Many more formats to come :D 

## Status

SeqWires is under active development.
Interested developers can take a look around but please note that some refactoring is planned.

Please do not expect development to be rapid.
I have very limited time to devote to this.

Right now the SMF plugin only supports note on/off events.
Extending it will be very easy, but my current priority is on getting the infrastructure right.

## Downloading and Building

Clone recursively to ensure submodules are populated:

```
git clone --recurse-submodules https://github.com/Malcohol/SeqWires.git
```

SeqWires is implemented using the [BabelWires](https://github.com/Malcohol/BabelWires) framework. 
SeqWires is intended to be cross-platform, but in its current state it is unlikely to build out-of-the-box.
See notes in the BabelWires readme.

# License

SeqWires is licensed under the GPLv3.
See the [LICENSE](LICENSE) file.
