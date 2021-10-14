#include <gtest/gtest.h>

#include <BabelWiresLib/Identifiers/identifierRegistry.hpp>

int main(int argc, char** argv) {
    // Since these tests load "real" types which use the FIELD_NAME macros, they have to work within the same registry singleton.
    babelwires::FieldNameRegistryScope fieldNameRegistry;
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
