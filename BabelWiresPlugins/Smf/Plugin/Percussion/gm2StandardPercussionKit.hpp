/**
 * A PercussionKit corresponding to the instruments of GM2 standard percussion set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionKit.hpp>

namespace smf {
    /// A PercussionKit corresponding to the instruments of GM2 standard percussion set.
    class GM2StandardPercussionKit : public PercussionKit {
      public:
        GM2StandardPercussionKit(const seqwires::BuiltInPercussionInstruments& builtInInstruments);

        static babelwires::LongIdentifier getThisIdentifier();
    };
} // namespace smf