/**
 * An enum which defines the available MIDI specifications.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(GM_SPEC_VALUES);

babelwires::LongIdentifier smf::GMSpecType::getThisIdentifier() {
    return REGISTERED_LONGID("GMSpec", "GM Specification", "4dc2566d-1be8-468b-9aa0-2f4d63344a13");
}

smf::GMSpecType::GMSpecType()
    : Enum(getThisIdentifier(), 1, ENUM_IDENTIFIER_VECTOR(GM_SPEC_VALUES), 1) {}
