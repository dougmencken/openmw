#include "loadbook.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace ESM
{

void Book::load(ESMReader &esm)
{
    mModel = esm.getHNString("MODL");
    mName = esm.getHNOString("FNAM");

    esm.getHNT(mData, "BKDT", 20);
    boost::fusion::for_each(mData, SwapLEStruct());

    mScript = esm.getHNOString("SCRI");
    mIcon = esm.getHNOString("ITEX");
    mText = esm.getHNOString("TEXT");
    mEnchant = esm.getHNOString("ENAM");
}
void Book::save(ESMWriter &esm)
{
    esm.writeHNCString("MODL", mModel);
    esm.writeHNOCString("FNAM", mName);

    boost::fusion::for_each(mData, SwapLEStruct());
    esm.writeHNT("BKDT", mData, 20);
    boost::fusion::for_each(mData, SwapLEStruct());

    esm.writeHNOCString("SCRI", mScript);
    esm.writeHNOCString("ITEX", mIcon);
    esm.writeHNOString("TEXT", mText);
    esm.writeHNOCString("ENAM", mEnchant);
}

}
