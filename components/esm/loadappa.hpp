#ifndef OPENMW_ESM_APPA_H
#define OPENMW_ESM_APPA_H

#include <string>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace ESM
{

class ESMReader;
class ESMWriter;

/*
 * Alchemist apparatus
 */

struct Apparatus
{
    enum AppaType
    {
        MortarPestle = 0,
        Albemic = 1,
        Calcinator = 2,
        Retort = 3
    };

    struct AADTstruct
    {
        int mType;
        float mQuality;
        float mWeight;
        int mValue;
    };

    AADTstruct mData;
    std::string mId, mModel, mIcon, mScript, mName;

    void load(ESMReader &esm);
    void save(ESMWriter &esm);
};

}

BOOST_FUSION_ADAPT_STRUCT(
    ESM::Apparatus::AADTstruct,
    (int, mType)
    (float, mQuality)
    (float, mWeight)
    (int, mValue))

#endif
