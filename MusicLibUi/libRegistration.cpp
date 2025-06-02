/**
 * Register factories etc. for the SeqWiresUi into BabelWiresUi.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLibUi/libRegistration.hpp>

#include <BabelWiresQtUi/ValueModels/valueModelRegistry.hpp>
#include <BabelWiresQtUi/uiProjectContext.hpp>

#include <MusicLib/Types/Track/trackType.hpp>

#include <MusicLibUi/ValueModels/trackValueModel.hpp>

void seqwiresUi::registerLib(babelwires::UiProjectContext& context) {
    context.m_valueModelReg.registryHandler<seqwires::TrackType, TrackValueModel>();
}
