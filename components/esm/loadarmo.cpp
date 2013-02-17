#include "loadarmo.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace ESM
{

void PartReferenceList::load(ESMReader &esm)
{
    while (esm.isNextSub("INDX"))
    {
        PartReference pr;
        esm.getHT(pr.mPart); // The INDX byte
        pr.mMale = esm.getHNOString("BNAM");
        pr.mFemale = esm.getHNOString("CNAM");
        mParts.push_back(pr);
    }
}

void PartReferenceList::save(ESMWriter &esm)
{
    for (std::vector<PartReference>::iterator it = mParts.begin(); it != mParts.end(); ++it)
    {
        esm.writeHNT("INDX", it->mPart);
        esm.writeHNOString("BNAM", it->mMale);
        esm.writeHNOString("CNAM", it->mFemale);
    }
}

void Armor::load(ESMReader &esm)
{
    mModel = esm.getHNString("MODL");
    mName = esm.getHNString("FNAM");
    mScript = esm.getHNOString("SCRI");

    esm.getHNT(mData, "AODT", 24);
    boost::fusion::for_each(mData, SwapLEStruct());

    mIcon = esm.getHNOString("ITEX");
    mParts.load(esm);
    mEnchant = esm.getHNOString("ENAM");
}

void Armor::save(ESMWriter &esm)
{
    esm.writeHNCString("MODL", mModel);
    esm.writeHNCString("FNAM", mName);
    esm.writeHNOCString("SCRI", mScript);

    boost::fusion::for_each(mData, SwapLEStruct());
    esm.writeHNT("AODT", mData, 24);
    boost::fusion::for_each(mData, SwapLEStruct());

    esm.writeHNOCString("ITEX", mIcon);
    mParts.save(esm);
    esm.writeHNOCString("ENAM", mEnchant);
}

}
