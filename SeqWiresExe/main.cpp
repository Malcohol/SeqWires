/**
 * SeqWires application main function.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "Audio/Alsa/alsaAudioDest.hpp"
#include "Audio/Alsa/alsaAudioSource.hpp"
#include "Common/Audio/fileAudioDest.hpp"
#include "Common/Audio/fileAudioSource.hpp"

#include "BabelWires/Features/Utilities/featureXml.hpp"

#include "BabelWiresQtUi/ModelBridge/RowModels/rowModelRegistry.hpp"
#include "BabelWiresQtUi/uiProjectContext.hpp"

#include "BabelWires/Features/Path/fieldNameRegistry.hpp"
#include "BabelWires/FileFormat/fileFeature.hpp"
#include "BabelWires/FileFormat/fileFormat.hpp"
#include "BabelWires/Processors/processorFactory.hpp"
#include "BabelWires/Processors/processorFactoryRegistry.hpp"
#include "BabelWires/Project/Modifiers/modifierData.hpp"
#include "BabelWires/Project/project.hpp"
#include "BabelWires/Project/projectData.hpp"
#include "BabelWires/Serialization/projectSerialization.hpp"
#include "Common/IO/fileDataSource.hpp"
#include "SeqWiresExe/seqWiresOptions.hpp"

#include "BabelWiresQtUi/uiMain.hpp"
#include "Common/Log/ostreamLogListener.hpp"
#include "Common/Log/unifiedLog.hpp"
#include "Common/Serialization/deserializationRegistry.hpp"

// "plugins"
#include "BabelWiresPlugins/Smf/libRegistration.hpp"
#include "SeqWiresLib/libRegistration.hpp"
#include "SeqWiresLibUi/libRegistration.hpp"

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

        babelwires::FieldNameRegistryScope fieldNameRegistry;

        babelwires::OStreamLogListener::Features features;
#ifndef NDEBUG
        features = features | babelwires::OStreamLogListener::Features::logDebugMessages |
                   babelwires::OStreamLogListener::Features::timestamp;
#endif
        babelwires::UnifiedLog log;
        babelwires::DebugLogger::swapGlobalDebugLogger(&log);
        babelwires::OStreamLogListener logToCout(std::cout, log, features);

        FileFeatureFactoryRegistry factoryFormatReg;
        FileFormatRegistry fileFormatReg;
        ProcessorFactoryRegistry processorReg;
        babelwires::AutomaticDeserializationRegistry deserializationRegistry;
        babelwires::RowModelRegistry rowModelRegistry;

        const unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        babelwires::logDebug() << "The random seed was " << seed;
        std::default_random_engine randomEngine(seed);

        babelwires::UiProjectContext context{factoryFormatReg,        fileFormatReg, processorReg,
                                             deserializationRegistry, randomEngine,  rowModelRegistry};
        
        context.m_applicationIdentity.m_applicationTitle = "Seqwires";
        context.m_applicationIdentity.m_projectExtension = "seqwires";

        // register factories, etc.
        seqwires::registerLib(context);
        seqwiresUi::registerLib(context);
        smf::registerLib(context);

        if (options.m_mode == ProgramOptions::MODE_DUMP) {
            if (const FileFormat* format = context.m_fileFormatReg.getEntryByFileName(options.m_inputFileName)) {
                try {
                    babelwires::FileDataSource file(options.m_inputFileName.c_str());
                    std::shared_ptr<babelwires::FileFeature> loadedFile = format->loadFromFile(file, log);
                    babelwires::featureToXml(*loadedFile, std::cout, options.m_dumpIsFullDump);
                } catch (const babelwires::BaseException& e) {
                    std::cerr << "Failed to load source file \"" << options.m_inputFileName << "\": " << e.what()
                              << "\n";
                }
            }
            return EXIT_SUCCESS;
        } else if (options.m_mode == ProgramOptions::MODE_RUN_PROJECT) {
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
