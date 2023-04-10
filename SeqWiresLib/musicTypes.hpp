/**
 * Some common type definitions for musical data.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/pitch.hpp>

#include <Common/Math/rational.hpp>
#include <Common/types.hpp>

#include <cstdint>
#include <string>

namespace seqwires {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Velocity;

} // namespace seqwires
