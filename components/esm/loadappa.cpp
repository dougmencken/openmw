#include "loadappa.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace ESM
{
void Apparatus::load(ESMReader &esm)
{
    mModel = esm.getHNString("MODL");
    mName = esm.getHNString("FNAM");

    esm.getHNT(mData, "AADT", 16);
    boost::fusion::for_each(mData, SwapLEStruct());

    mScript = esm.getHNOString("SCRI");
    mIcon = esm.getHNString("ITEX");
}
void Apparatus::save(ESMWriter &esm)
{
    esm.writeHNCString("MODL", mModel);
    esm.writeHNCString("FNAM", mName);

    boost::fusion::for_each(mData, SwapLEStruct());
    esm.writeHNT("AADT", mData, 16);
    boost::fusion::for_each(mData, SwapLEStruct());

    esm.writeHNOCString("SCRI", mScript);
    esm.writeHNCString("ITEX", mIcon);
}
}
