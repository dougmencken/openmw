#ifndef OPENMW_ENDIANESS_H
#define OPENMW_ENDIANESS_H

#include <libs/platform/stdint.h>
#include <endian.h>
#include <OgrePlatform.h>
#include <boost/array.hpp>

#define OPENMW_FOURCHAR(a,b,c,d) ((uint32_t)(((a)<<24) | ((b)<<16) | ((c)<<8) | (d)))

float reverseFloat(const float inFloat);

#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
#  define letoh_float(f) reverseFloat(f)
#  define htole_float(f) reverseFloat(f)
#  define betoh_float(f) (f)
#  define htobe_float(f) (f)
#elif OGRE_ENDIAN == OGRE_ENDIAN_LITTLE
#  define letoh_float(f) (f)
#  define htole_float(f) (f)
#  define betoh_float(f) reverseFloat(f)
#  define htobe_float(f) reverseFloat(f)
#endif

template<class T>
inline void endianSwap(T& t)
{
    union
    {
        T t;
        unsigned char u8[sizeof(T)];
    } in, out;

    switch (sizeof(T))
    {
        case 4:
            in.t = t;
            out.u8[0] = in.u8[3];
            out.u8[1] = in.u8[2];
            out.u8[2] = in.u8[1];
            out.u8[3] = in.u8[0];
            t = out.t;
            break;
        case 2:
            in.t = t;
            out.u8[0] = in.u8[1];
            out.u8[1] = in.u8[0];
            t = out.t;
            break;
        case 1:
            // nothing to be swapped
            break;
        default:
            in.t = t;
            for (size_t k = 0; k < sizeof(T); k++)
                out.u8[k] = in.u8[sizeof(T)-1 - k];
            t = out.t;
            break;
    }
}

namespace openmw
{
  struct false_type { enum { value = false }; };
  struct true_type { enum { value = true }; };
}

template<typename T>
struct is_array : openmw::false_type {};

template<typename T, size_t N>
struct is_array< boost::array<T,N> > : openmw::true_type {};

template<typename T>
void do_swap(T& t, openmw::false_type) {
    // swap non-array types
    endianSwap<T>(t);
}

template<typename T>
void do_swap(T& t, openmw::true_type) {
    // swap boost::array
    size_t size = t.size();
    for (size_t i = 0; i < size; i++)
        endianSwap<typename T::value_type>(t[i]);
}

struct SwapLEStruct
{
    template<typename T>
    void operator()(T& t) const
    {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
        do_swap(t, is_array<T>());
#elif OGRE_ENDIAN == OGRE_ENDIAN_LITTLE
        // do nothing
#endif
    }
};

struct SwapBEStruct
{
    template<typename T>
    void operator()(T& t) const
    {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
        // do nothing
#elif OGRE_ENDIAN == OGRE_ENDIAN_LITTLE
        do_swap(t, is_array<T>());
#endif
    }
};

#endif
