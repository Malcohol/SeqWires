/**
 * The TrackRowModel provides the UI for a Track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresQtUi/ModelBridge/RowModels/rowModelWithRichText.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace seqwiresUi {

    class TrackRowModel : public babelwires::RowModelWithRichText {
      public:
        virtual QVariant getValueDisplayData() const override;
        virtual QVariant getTooltip() const override;

      public:
        const seqwires::TrackFeature& getTrackFeature() const;
    };

} // namespace seqwiresUi
