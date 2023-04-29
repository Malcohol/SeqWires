/**
 * TrackType is the type of Tracks.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/type.hpp>

#include <SeqWiresLib/musicTypes.hpp>

namespace seqwires {
    /// TrackType is the type of Tracks.
    class TrackType : public babelwires::Type {
      public:
        TrackType(ModelDuration defaultDuration = 0);

        babelwires::NewValueHolder createValue() const override;

        bool isValidValue(const babelwires::Value& v) const override;

        std::string getKind() const override;
      private:
        ModelDuration m_defaultDuration;
    };

    /// The standard TrackType which creates empty Tracks with 0 duration.
    class DefaultTrackType : public TrackType {
      public:
        PRIMITIVE_TYPE("track", "Track", "346ec14c-25dd-43fc-a942-d24722be6802", 1);
    };
} // namespace seqwires