/**
 * Options for the seq2Tape program.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Common/Audio/fileAudioDest.hpp>
#include <Seq2tapeExe/seq2tapeOptions.hpp>
#include <getopt.h>

namespace {
    const char s_helpString[] = "help";
    const char s_convertString[] = "convert";
    const char s_playString[] = "play";
    const char s_captureString[] = "capture";
    const char s_formatsString[] = "formats";
    const char s_audioString[] = "audio";
} // namespace

ProgramOptions::ProgramOptions(int argc, char* argv[])
    : m_mode(MODE_PRINT_HELP) {
    int option = 0;

    if (argc < 2) {
        throw babelwires::OptionError() << "No mode specified";
    }

    const std::string modeArg = argv[1];

    if ((modeArg == s_helpString) || (modeArg == "-h") || (modeArg == "--help")) {
        m_mode = MODE_PRINT_HELP;
    } else if (modeArg == s_captureString) {
        m_mode = MODE_CAPTURE;
        m_captureOptions = std::make_unique<CaptureOptions>();
        m_captureOptions->m_inputCaptureSource = "default";
        m_captureOptions->m_numDataFiles = 1;

        int i = 2;
        while (i < argc - 1) {
            std::string nextArg = argv[i];
            if (nextArg == "-s") {
                m_captureOptions->m_inputCaptureSource = argv[i + 1];
                i += 2;
            } else if (nextArg == "-f") {
                m_captureOptions->m_numDataFiles = atoi(argv[i + 1]);
                i += 2;
            } else if (nextArg == "-n") {
                m_captureOptions->m_sequenceName = argv[i + 1];
                i += 2;
            } else if (nextArg == "-c") {
                m_captureOptions->m_copyright = argv[i + 1];
                i += 2;
            } else {
                throw babelwires::OptionError() << "Unexpected arguments provided to capture mode";
            }
        }
        if (i == argc) {
            throw babelwires::OptionError() << "Capture mode requires a filename argument";
        }
        m_captureOptions->m_outputFileName = argv[argc - 1];
    } else if (modeArg == s_playString) {
        m_mode = MODE_PLAYBACK;
        m_playbackOptions = std::make_unique<PlaybackOptions>();
        m_playbackOptions->m_outputPlaybackDest = "default";
        int i = 2;
        while (i < argc - 1) {
            std::string nextArg = argv[i];
            if (nextArg == "-d") {
                m_playbackOptions->m_outputPlaybackDest = argv[i + 1];
                i += 2;
            } else {
                throw babelwires::OptionError() << "Unexpected arguments to playback mode";
            }
        }
        if (i == argc) {
            throw babelwires::OptionError() << "Playback mode requires a filename argument";
        }
        m_playbackOptions->m_inputFileName = argv[argc - 1];
    } else if (modeArg == s_convertString) {
        m_mode = MODE_CONVERT;
        m_convertOptions = std::make_unique<ConvertOptions>();
        int i = 2;
        while (i < argc - 2) {
            std::string nextArg = argv[i];
            if (nextArg == "-n") {
                m_convertOptions->m_sequenceName = argv[i + 1];
                i += 2;
            } else if (nextArg == "-c") {
                m_convertOptions->m_copyright = argv[i + 1];
                i += 2;
            } else {
                throw babelwires::OptionError() << "Unexpected arguments provided to convert mode";
            }
        }
        if (i == argc - 1) {
            throw babelwires::OptionError() << "Convert mode expects two filename arguments";
        }
        m_convertOptions->m_inputFileName = argv[i];
        m_convertOptions->m_outputFileName = argv[i + 1];
    } else if (modeArg == s_formatsString) {
        m_mode = MODE_LIST_FORMATS;
    } else if (modeArg == s_audioString) {
        m_mode = MODE_LIST_AUDIO;
    } else {
        throw babelwires::OptionError() << "Unrecognized mode \"" << modeArg << "\" provided";
    }
}

void writeUsage(const std::string& programName, std::ostream& stream) {
    stream << "Usage:" << std::endl;
    stream << programName << " " << s_playString << " [-d <audio destination>] <input file>" << std::endl;
    stream << programName << " " << s_captureString
           << " [-s <audio source>] [-f <num data files>] [-n name] [-c copyright] <output file>" << std::endl;
    stream << programName << " " << s_convertString << " [-n name] [-c copyright] <input file> <output file>"
           << std::endl;
    stream << programName << " " << s_formatsString << " " << std::endl;
    stream << programName << " " << s_audioString << " " << std::endl;
    stream << programName << " " << s_helpString << " " << std::endl;
}

void writeHelp(const std::string& programName, std::ostream& stream) {
    stream << programName
           << " - Converts music sequencer data between file formats suitable for computer storage and audio data "
              "format suitable for cassette tapes."
           << std::endl;
    writeUsage(programName, stream);
}
