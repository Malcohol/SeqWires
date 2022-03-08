/**
 * Formats which support Standard MIDI Files.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWiresLib/FileFormat/fileFeature.hpp"
#include "BabelWiresLib/FileFormat/sourceFileFormat.hpp"
#include "BabelWiresLib/FileFormat/targetFileFormat.hpp"

namespace smf {
    /// Format for loading Standard MIDI Files..
    class SmfSourceFormat : public babelwires::SourceFileFormat {
      public:
        SmfSourceFormat();
        static babelwires::LongIdentifier getThisIdentifier();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature>
        loadFromFile(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger) const override;
    };

    /// Format for creating Format 0 Standard MIDI Files..
    class SmfFormat0TargetFormat : public babelwires::TargetFileFormat {
      public:
        SmfFormat0TargetFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature(const babelwires::ProjectContext& projectContext) const override;
        virtual void writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const override;
    };

    /// Format for creating Format 1 Standard MIDI Files..
    class SmfFormat1TargetFormat : public babelwires::TargetFileFormat {
      public:
        SmfFormat1TargetFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature(const babelwires::ProjectContext& projectContext) const override;
        virtual void writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const override;
    };

} // namespace smf
