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
    const seqwires::Track& value = getValue()->is<seqwires::Track>();
    const babelwires::Rational duration = value.getDuration();
    return duration.toHtmlString().c_str();
}

QString seqwiresUi::TrackValueModel::getTooltip() const {
    const seqwires::Track& track = getValue()->is<seqwires::Track>();
    const auto& numEventGroupsByCategory = track.getNumEventGroupsByCategory();
    if (!numEventGroupsByCategory.empty()) {
        QString summary;
        const char* delim = "";
        for (auto p : numEventGroupsByCategory) {
            summary += delim;
            summary += QString("%1: %2").arg(p.first, QString::number(p.second));
            delim = "\n";
        }
        return summary;
    } else {
        return QString("No events");
    }
}
