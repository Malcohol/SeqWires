/**
 * A PercussionKit corresponding to the instruments of GM2 Power percussion set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionKit.hpp>

namespace smf {
    /// A PercussionKit corresponding to the instruments of GM2 Power Percussion set.
    class GM2PowerPercussionKit : public PercussionKit {
      public:
        GM2PowerPercussionKit(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf