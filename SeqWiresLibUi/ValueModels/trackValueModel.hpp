/**
 * The TrackRowModel provides the UI for a Track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresQtUi/ModelBridge/RowModels/rowModelWithRichText.hpp>
#include <BabelWiresQtUi/ValueModels/valueModelWithRichText.hpp>

namespace seqwires {
    class TrackFeature;
}

namespace seqwiresUi {

    class TrackValueModel : public babelwires::ValueModelWithRichText {
      public:
        QString getRichText() const override;
        QString getTooltip() const override;
    };

} // namespace seqwiresUi
