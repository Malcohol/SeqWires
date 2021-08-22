/**
 * The TrackRowModel provides the UI for a Track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLibUi/RowModels/trackRowModel.hpp"

#include "SeqWiresLib/Features/trackFeature.hpp"

#include <QString>

#include <cassert>

const seqwires::TrackFeature& seqwiresUi::TrackRowModel::getTrackFeature() const {
    // Favour output here, since tracks cannot be directly edited.
    assert(getOutputThenInputFeature()->as<const seqwires::TrackFeature>() && "Wrong type of feature stored");
    return *static_cast<const seqwires::TrackFeature*>(getOutputThenInputFeature());
}

QVariant seqwiresUi::TrackRowModel::getValueDisplayData() const {
    const seqwires::TrackFeature& trackFeature = getTrackFeature();
    const babelwires::Rational duration = trackFeature.get().getDuration();
    if (isFeatureModified()) {
        return QString("<b>") + QString(duration.toHtmlString().c_str()) + QString("</b>");
    } else {
        return QString(duration.toHtmlString().c_str());
    }
}

QVariant seqwiresUi::TrackRowModel::getTooltip() const {
    const seqwires::TrackFeature& trackFeature = getTrackFeature();
    QVariant ret = RowModel::getTooltip();
    if (ret.isNull()) {
        const seqwires::Track& track = getTrackFeature().get();
        const auto& numEventGroupsByCategory = track.getNumEventGroupsByCategory();
        if (!numEventGroupsByCategory.empty()) {
            QString summary;
            const char* delim = "";
            for (auto p : numEventGroupsByCategory) {
                summary += delim;
                summary += QString("%1: %2").arg(p.first, QString::number(p.second));
                delim = "\n";
            }
            ret = summary;
        }
    }
    return ret;
}
