#include "SeqwiresLibUi/libRegistration.hpp"

#include "BabelWiresQtUi/ModelBridge/RowModels/rowModelRegistry.hpp"
#include "BabelWiresQtUi/uiProjectContext.hpp"

#include "SeqwiresLibUi/RowModels/trackRowModel.hpp"

#include "SeqwiresLib/Features/trackFeature.hpp"

void seqwiresUi::registerLib(babelwires::UiProjectContext& context) {
    context.m_rowModelReg.registryHandler<seqwires::TrackFeature, TrackRowModel>();
}
