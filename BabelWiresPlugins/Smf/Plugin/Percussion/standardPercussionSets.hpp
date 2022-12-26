/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresPlugins/Smf/Plugin/Percussion/percussionSet.hpp>
#include <BabelWiresPlugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/builtInPercussionInstruments.hpp>

#include <unordered_set>

namespace babelwires {
    struct ProjectContext;
}

namespace smf {
    class StandardPercussionSets {
      public:
        StandardPercussionSets(const babelwires::ProjectContext& projectContext);

        /// Get the default set for each channel in the given spec.
        const PercussionSet* getDefaultPercussionSet(GMSpecType::Value gmSpec, int channelNumber);

        // TODO This isn't percussion specific.
        struct ChannelSetupInfo {
            babelwires::Byte m_bankMSB = 0;
            babelwires::Byte m_bankLSB = 0;
            babelwires::Byte m_program = 0;
            /// This is the part corresponding to this channel, irrespective of the part mapping.
            babelwires::Byte m_gsPartMode = 0;
        };

        /// Get the percussion set specified by the given parameters, or nullptr if a percussion set is not specified.
        const PercussionSet* getPercussionSetFromChannelSetupInfo(GMSpecType::Value gmSpec, ChannelSetupInfo channelSetupInfo);

        /// Find the percussion set which contains the most instruments from the given set, and is suitable for use in
        /// the given channel.
        const PercussionSet* getBestPercussionSet(GMSpecType::Value gmSpec, int channelNumber,
                                                  const std::unordered_set<babelwires::Identifier>& instrumentsInUse,
                                                  std::unordered_set<babelwires::Identifier>& excludedInstrumentsOut);

        /// Get setup information for the given percussionSet if any is required. 
        /// A null argument is allowed, in which case an empty optional is returned.
        std::optional<ChannelSetupInfo> getChannelSetupInfoFromPercussionSet(const PercussionSet* percussionSet);

      private:
        enum KnownPercussionSets {
            GM_PERCUSSION_SET,

            GM2_SETS_START,

            GM2_ANALOG_PERCUSSION_SET = GM2_SETS_START,
            GM2_BRUSH_PERCUSSION_SET,
            GM2_ELECTRONIC_PERCUSSION_SET,
            GM2_JAZZ_PERCUSSION_SET,
            GM2_ORCHESTRA_PERCUSSION_SET,
            GM2_POWER_PERCUSSION_SET,
            GM2_ROOM_PERCUSSION_SET,
            GM2_SFX_PERCUSSION_SET,
            GM2_STANDARD_PERCUSSION_SET,

            GM2_SETS_END = GM2_STANDARD_PERCUSSION_SET,

            NUM_KNOWN_PERCUSSION_SETS,
            NOT_PERCUSSION = NUM_KNOWN_PERCUSSION_SETS
        };

        /// Ensure the m_instrumentSets array is populated.
        void ensureInstrumentSets();

        /// From the range of known percussion, select the best percussion set.
        const PercussionSet* getBestPercussionSetInRange(int startIndex, int endIndex,
                                                  const std::unordered_set<babelwires::Identifier>& instrumentsInUse,
                                                  std::unordered_set<babelwires::Identifier>& excludedInstrumentsOut);

        /// Find the percussion set in the array of known percussion sets. Asserts if not found.
        KnownPercussionSets getKnownPercussionSetFromPercussionSet(const PercussionSet* percussionSet);


        std::optional<ChannelSetupInfo> getChannelSetupInfoFromKnownPercussionSet(KnownPercussionSets percussionSet);

      private:
        std::array<const smf::PercussionSet*, NUM_KNOWN_PERCUSSION_SETS> m_knownSets;

        /// This is populated on demand.
        std::array<std::unordered_set<babelwires::Identifier>, NUM_KNOWN_PERCUSSION_SETS> m_instrumentSets;
    };

} // namespace smf
