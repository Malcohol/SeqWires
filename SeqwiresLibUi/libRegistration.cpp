/**
 * Register factories etc. for the SeqWiresUi into BabelWiresUi.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqwiresLibUi/libRegistration.hpp"

#include "BabelWiresQtUi/ModelBridge/RowModels/rowModelRegistry.hpp"
#include "BabelWiresQtUi/uiProjectContext.hpp"

#include "SeqwiresLibUi/RowModels/trackRowModel.hpp"

#include "SeqwiresLib/Features/trackFeature.hpp"

void seqwiresUi::registerLib(babelwires::UiProjectContext& context) {
    context.m_rowModelReg.registryHandler<seqwires::TrackFeature, TrackRowModel>();
}
