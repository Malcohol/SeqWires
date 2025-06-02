/**
 * Register factories etc. for SeqWires into BabelWires.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

namespace babelwires {
    struct ProjectContext;
}

namespace bw_music {
    /// This is not a true plugin model, because everything is statically linked.
    void registerLib(babelwires::ProjectContext& context);
} // namespace bw_music
