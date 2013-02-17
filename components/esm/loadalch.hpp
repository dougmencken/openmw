#ifndef OPENMW_ESM_ALCH_H
#define OPENMW_ESM_ALCH_H

#include <string>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "effectlist.hpp"

namespace ESM
{

class ESMReader;
class ESMWriter;

/*
 * Alchemy item (potions)
 */

struct Potion
{
    struct ALDTstruct
    {
        float mWeight;
        int mValue;
        int mAutoCalc;
    };
    ALDTstruct mData;

    std::string mId, mName, mModel, mIcon, mScript;
    EffectList mEffects;

    void load(ESMReader &esm);
    void save(ESMWriter &esm);
};

}

BOOST_FUSION_ADAPT_STRUCT(
    ESM::Potion::ALDTstruct,
    (float, mWeight)
    (int, mValue)
    (int, mAutoCalc))

#endif
