//
// Copyright 2015 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// BitSetIterator:
//   A helper class to quickly bitscan bitsets for set bits.
//

#ifndef COMMON_BITSET64_H_
#define COMMON_BITSET64_H_

#include <stdint.h>

#include <bitset>

#include "common/angleutils.h"
#include "common/debug.h"
#include "common/mathutil.h"
#include "common/platform.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace angle
{
class BitSet64 final
{
  public:
    explicit BitSet64(uint64_t bits):mBits(bits) {}

    class Iterator final
    {
      public:
        explicit Iterator(uint64_t bits):mBits(bits) {}
				Iterator &operator++()
				{
					mBits &= mBits - 1;
					return *this;
				}
        bool operator==(const Iterator &other) const { return mBits == other.mBits; }
        bool operator!=(const Iterator &other) const { return mBits != other.mBits; }
				unsigned long operator*() const
				{
#if defined(ANGLE_PLATFORM_WINDOWS)
					unsigned long firstBitIndex = 0ul;
					_BitScanForward64(&firstBitIndex, mBits);
					return firstBitIndex;
#elif defined(ANGLE_PLATFORM_POSIX)
					return static_cast<unsigned long>(__builtin_ctzl(mBits));
#else
#error Please implement bit-scan-forward for your platform!
#endif
				}

      private:
        uint64_t mBits;
    };

    Iterator begin() const { return Iterator(mBits); }
    Iterator end() const { return Iterator(0); }

  private:
    uint64_t mBits;
};

// Helper to avoid needing to specify the template parameter size
template<size_t N>
inline BitSet64 IterateBitSet64(const std::bitset<N> &bitset)
{
    return BitSet64(bitset.to_ullong());
}

}  // angle

#endif  // COMMON_BITSET64_H_
