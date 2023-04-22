/**
 * Handle the command-line options for the SeqWires application.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresExe/seqWiresOptions.hpp>

#include <Common/Audio/fileAudioDest.hpp>

namespace {
    const char s_helpString[] = "help";
    const char s_runString[] = "run";
    const char s_uiString[] = "ui";
} // namespace

ProgramOptions::ProgramOptions(int argc, char* argv[]) {
    if (argc == 1) {
        // UI mode.
        return;
    }

    const std::string modeArg = argv[1];

    if ((modeArg == s_helpString) || (modeArg == "-h") || (modeArg == "--help")) {
        m_mode = MODE_PRINT_HELP;
    } else if (modeArg == s_runString) {
        if (argc != 3) {
            throw babelwires::OptionError() << "Wrong number of arguments for " << s_runString << " mode";
        }
        m_mode = MODE_RUN_PROJECT;
        m_inputFileName = argv[2];
    } else if (modeArg != s_uiString) {
        throw babelwires::OptionError() << "Unrecognized mode \"" << modeArg << "\" provided";
    }
}

void writeUsage(const std::string& programName, std::ostream& stream) {
    stream << "Usage:" << std::endl;
    stream << programName << std::endl;
    stream << programName << " " << s_runString << " projectFile" << std::endl;
    stream << programName << " " << s_helpString << std::endl;
}

void writeHelp(const std::string& programName, std::ostream& stream) {
    stream << programName << " - A program to transform music sequence data between various file formats." << std::endl;
    writeUsage(programName, stream);
}
