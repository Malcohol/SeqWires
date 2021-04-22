#pragma once

#include "BabelWires/FileFormat/fileFeature.hpp"
#include "BabelWires/FileFormat/fileFormat.hpp"

namespace smf {

    class SmfFormat : public babelwires::FileFormat {
      public:
        SmfFormat();
        static std::string getThisIdentifier();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature>
        loadFromFile(babelwires::DataSource& dataSource, babelwires::UserLogger& userLogger) const override;
        virtual void writeToFile(const babelwires::FileFeature& sequence, std::ostream& os,
                                 babelwires::UserLogger& userLogger) const override;
    };

    class Format0FactoryFormat : public babelwires::FileFeatureFactory {
      public:
        Format0FactoryFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature() const override;
    };

    class Format1FactoryFormat : public babelwires::FileFeatureFactory {
      public:
        Format1FactoryFormat();

        virtual std::string getManufacturerName() const override;
        virtual std::string getProductName() const override;
        virtual std::unique_ptr<babelwires::FileFeature> createNewFeature() const override;
    };

} // namespace smf
