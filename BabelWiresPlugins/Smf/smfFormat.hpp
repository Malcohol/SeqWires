/**
 * Formats which support Standard MIDI Files.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include "BabelWires/FileFormat/fileFeature.hpp"
#include "BabelWires/FileFormat/fileFormat.hpp"

namespace smf {
    /// Format for loading Standard MIDI Files..
    class SmfFormat : public babelwires::SourceFileFormat {
      public:
        SmfFormat();
        static std::string getThisIdentifier();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature>
        loadFromFile(babelwires::DataSource& dataSource, babelwires::UserLogger& userLogger) const override;
    };

    /// Format for creating Format 0 Standard MIDI Files..
    class Format0FactoryFormat : public babelwires::TargetFileFactory {
      public:
        Format0FactoryFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature() const override;
        virtual void writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const override;
    };

    /// Format for creating Format 1 Standard MIDI Files..
    // TODO Bug: Tracks don't seem to have ports at the moment.
    class Format1FactoryFormat : public babelwires::TargetFileFactory {
      public:
        Format1FactoryFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature() const override;
        virtual void writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const override;
    };

} // namespace smf
