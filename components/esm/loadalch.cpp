#include "loadalch.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace ESM
{
void Potion::load(ESMReader &esm)
{
    mModel = esm.getHNString("MODL");
    mIcon = esm.getHNOString("TEXT"); // not ITEX here for some reason
    mScript = esm.getHNOString("SCRI");
    mName = esm.getHNOString("FNAM");

    esm.getHNT(mData, "ALDT", 12);
    boost::fusion::for_each(mData, SwapLEStruct());

    mEffects.load(esm);
}

void Potion::save(ESMWriter &esm)
{
    esm.writeHNCString("MODL", mModel);
    esm.writeHNOCString("TEXT", mIcon);
    esm.writeHNOCString("SCRI", mScript);
    esm.writeHNOCString("FNAM", mName);

    boost::fusion::for_each(mData, SwapLEStruct());
    esm.writeHNT("ALDT", mData, 12);
    boost::fusion::for_each(mData, SwapLEStruct());

    mEffects.save(esm);
}
}
