#pragma once

#include "BabelWiresQtUi/ModelBridge/RowModels/rowModelWithRichText.hpp"

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
