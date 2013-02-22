#include "loadclas.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace ESM
{

const Class::Specialization Class::sSpecializationIds[3] = {
  Class::Combat,
  Class::Magic,
  Class::Stealth
};

const char *Class::sGmstSpecializationIds[3] = {
  "sSpecializationCombat",
  "sSpecializationMagic",
  "sSpecializationStealth"
};

void Class::load(ESMReader &esm)
{
    mName = esm.getHNString("FNAM");
    esm.getHNT(mData, "CLDT", 60);
    boost::fusion::for_each(mData, SwapLEStruct());

    if (mData.mIsPlayable > 1)
        esm.fail("Unknown bool value");

    mDescription = esm.getHNOString("DESC");
}

void Class::save(ESMWriter &esm)
{
    esm.writeHNCString("FNAM", mName);

    boost::fusion::for_each(mData, SwapLEStruct());
    esm.writeHNT("CLDT", mData, 60);
    boost::fusion::for_each(mData, SwapLEStruct());

    esm.writeHNOString("DESC", mDescription);
}

}
