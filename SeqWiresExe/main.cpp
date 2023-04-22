/**
 * SeqWires application main function.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <SeqWiresExe/seqWiresOptions.hpp>

#include <BabelWiresQtUi/ModelBridge/RowModels/rowModelRegistry.hpp>
#include <BabelWiresQtUi/uiMain.hpp>
#include <BabelWiresQtUi/uiProjectContext.hpp>

#include <BabelWiresLib/FileFormat/fileFeature.hpp>
#include <BabelWiresLib/FileFormat/sourceFileFormat.hpp>
#include <BabelWiresLib/FileFormat/targetFileFormat.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processorFactoryRegistry.hpp>
#include <BabelWiresLib/Project/Modifiers/modifierData.hpp>
#include <BabelWiresLib/Project/project.hpp>
#include <BabelWiresLib/Project/projectData.hpp>
#include <BabelWiresLib/Serialization/projectSerialization.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/libRegistration.hpp>

#include <Common/Audio/fileAudioDest.hpp>
#include <Common/Audio/fileAudioSource.hpp>
#include <Common/IO/fileDataSource.hpp>
#include <Common/Identifiers/identifierRegistry.hpp>
#include <Common/Log/ostreamLogListener.hpp>
#include <Common/Log/unifiedLog.hpp>
#include <Common/Serialization/deserializationRegistry.hpp>
// "plugins"
#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <SeqWiresLib/libRegistration.hpp>
#include <SeqWiresLibUi/libRegistration.hpp>

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <tinyxml2.h>

using namespace babelwires;

int main(int argc, char* argv[]) {
    try {
        ProgramOptions options(argc, argv);

        if (options.m_mode == ProgramOptions::MODE_PRINT_HELP) {
            writeHelp(argv[0], std::cout);
            return EXIT_SUCCESS;
        }

        babelwires::IdentifierRegistryScope identifierRegistry;

        babelwires::OStreamLogListener::Features features = babelwires::OStreamLogListener::Features::none;
#ifndef NDEBUG
        features = features | babelwires::OStreamLogListener::Features::logDebugMessages |
                   babelwires::OStreamLogListener::Features::timestamp;
#endif
        babelwires::UnifiedLog log;
        babelwires::DebugLogger::swapGlobalDebugLogger(&log);
        babelwires::OStreamLogListener logToCout(std::cout, log, features);

        SourceFileFormatRegistry sourceFileFormatReg;
        TargetFileFormatRegistry targetFileFormatReg;
        ProcessorFactoryRegistry processorReg;
        babelwires::AutomaticDeserializationRegistry deserializationRegistry;
        babelwires::RowModelRegistry rowModelRegistry;
        babelwires::TypeSystem typeSystem;

        const unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        babelwires::logDebug() << "The random seed was " << seed;
        std::default_random_engine randomEngine(seed);

        babelwires::UiProjectContext context{
            deserializationRegistry, sourceFileFormatReg, targetFileFormatReg, processorReg, typeSystem, randomEngine,
            rowModelRegistry};

        context.m_applicationIdentity.m_applicationTitle = "Seqwires";
        context.m_applicationIdentity.m_projectExtension = ".seqwires";

        // register factories, etc.
        babelwires::registerLib(context);
        seqwires::registerLib(context);
        seqwiresUi::registerLib(context);
        smf::registerLib(context);

        if (options.m_mode == ProgramOptions::MODE_RUN_PROJECT) {
            Project project(context, log);
            ProjectData projectData = ProjectSerialization::loadFromFile(options.m_inputFileName.c_str(), context, log);
            project.setProjectData(projectData);
            project.tryToSaveAllTargets();
            return EXIT_SUCCESS;
        } else {
            Ui ui(argc, argv, context, log);
            if (argc > 1) {
                throw babelwires::OptionError() << "Unexpected arguments";
            }
            ui.runMainLoop();
        }
    } catch (const babelwires::OptionError& e) {
        std::cerr << e.what() << std::endl;
        writeUsage(argv[0], std::cerr);
    } catch (const babelwires::BaseException& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}
