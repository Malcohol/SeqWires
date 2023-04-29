/**
 * The TrackRowModel provides the UI for a Track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLibUi/ValueModels/trackValueModel.hpp>

#include <SeqWiresLib/Types/Track/track.hpp>

#include <QString>

#include <cassert>

QString seqwiresUi::TrackValueModel::getRichText() const {
    const seqwires::Track& value = m_value->is<seqwires::Track>();
    const babelwires::Rational duration = value.getDuration();
    return duration.toHtmlString().c_str();
}

/*
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
        } else {
            return QString("No events");
        }
    }
    return ret;
}
*/