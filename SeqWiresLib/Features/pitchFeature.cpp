/**
 * The PitchFeature is an IntFeature holding a single MIDI note value.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include "SeqWiresLib/Features/pitchFeature.hpp"

#include "SeqWiresLib/musicTypes.hpp"

#include "BabelWires/ValueNames/valueNames.hpp"

namespace {
    struct NoteValueNames : babelwires::ValueNames {
        bool doGetValueForName(const std::string& name, int& valueOut) const override {
            try {
                valueOut = seqwires::stringToPitch(name);
                return true;
            }
            catch(babelwires::ParseException& e) {
                return false;
            }
        }
        bool doGetNameForValue(int value, std::string& nameOut) const override {
            assert(value >= 0);
            assert(value <= 127);
            nameOut = seqwires::pitchToString(value);
            return true;
        }
        virtual int getFirstValue() const override {
            return 0;
        }
        virtual bool getNextValueWithName(int& value) const  {
            if (value < 127 ) {
                ++value;
                return true;
            }
            return false;
        }
    } s_noteValueNames;
} // namespace

const babelwires::ValueNames* seqwires::PitchFeature::getValueNames() const {
    return &s_noteValueNames;
}
