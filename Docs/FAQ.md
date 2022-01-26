# Seqwires FAQ

### Q: Could the SeqWires codebase be used as an import/export library for another application?

A: Not easily.
SeqWires aims for flexibility and, by design, does not attempt to impose a single general model of music sequence. 

The following weak statement is a close as you could get to such a model: 
"A supported file format will usually correspond to a CompoundFeature carrying TrackFeatures, and many of the tracks are likely to contain recognisable events such as notes."

An application, such as a software sequencer, could rely on this weak model to present imported data as a flattened list of tracks. 
That might offer some value, even though the structure and semantics of the tracks would be missing, and lots of other important data would be lost.

The situation for exporting is more severe: it is extremely unlikely that an application could correctly populate the Features in an arbitrary format.

The same use-cases are better supported if SeqWires has a plugin for the application's own native format (or at least some format it can import/export). 


