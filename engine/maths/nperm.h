
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file maths/nperm.h
 *  \brief Deals with permutations of {0,1,...,<i>n</i>-1}.
 */

#ifndef __NPERM_H
#ifndef __DOXYGEN
#define __NPERM_H
#endif

#include <string>
#include "regina-core.h"
#include "utilities/intutils.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a permutation of {0,1,...,<i>n</i>-1}.
 * Amongst other things, such permutations are used to describe
 * simplex gluings in (<i>n</i>-1)-manifold triangulations.
 *
 * NPerm objects are small enough to pass about by value instead of by
 * reference.  The trade-off is that, for this to be possible, the NPerm
 * class can only work with \a n &le; 16.
 *
 * Each permutation has an internal code, which is a single native
 * integer that is sufficient to reconstruct the entire permutation.
 * Thus the internal code may be a useful means for passing permutation
 * objects to and from the engine.  These codes are constructed as follows:
 *
 * - For 5 &le; \a n &le; 16, the code is essentially a packed array
 *   that holds the images of 0,...,<i>n</i>-1 in a single native integer type.
 *   More precisely, the internal code is an unsigned integer of type \a Code,
 *   whose lowest \a imageBits bits represent the image of 0, whose next
 *   lowest \a imageBits bits represent the image of 1, and so on.
 *
 * - For \a n &le; 4, the code is an index into a hard-coded list of
 *   all possible permutations (i.e., an index into the symmetric group
 *   <i>S<sub>n</sub></i>).  For details, see the documentation for
 *   the specialisations NPerm<3> and NPerm<4> respectively.
 *
 * For \a n = 3, 4 and 5, this class offers some additional functionality,
 * and is made available under the typedefs NPerm3, NPerm4 and NPerm5
 * respectively.  These specialised classes play a central role in describing
 * and manipulating 2-, 3- and 4-manifold triangulations.
 *
 * \ifacespython The various instantiations of this template class are
 * available in Python under the hard-coded names NPerm3, NPerm4, ..., NPerm16.
 *
 * @tparam n the number of objects being permuted.
 * This must be between 3 and 16 inclusive.
 */
template <int n>
class REGINA_API NPerm {
    static_assert(n >= 5 && n <= 16,
        "The generic NPerm<n> template is only available for 5 <= n <= 16.");
    public:
        /**
         * Indicates the number of bits used by the permutation code
         * to store the image of a single integer.
         *
         * This constant refers to the "image packing" codes that are
         * used for \a n &ge; 5, as described in the NPerm class notes.
         * For \a n &le; 4 the permutation codes are constructed in a
         * different way, and so this constant is not present.
         *
         * The full permutation code packs \a n such images together,
         * and so uses \a n * \a imageBits bits in total.
         */
        static const int imageBits;

        /**
         * Denotes a native signed integer type large enough to count all
         * permutations on \a n elements.  In other words, this is a
         * native signed integer type large enough to hold <i>n</i>!.
         */
        typedef typename IntOfMinSize<(imageBits * n + 7) / 8>::type Index;

        /**
         * Indicates the native unsigned integer type used to store the
         * internal permutation code.
         *
         * This typedef is present for all values of \a n, though its
         * precise size depends on how the permutation code is
         * constructed.  In particular, this type is defined differently
         * for \a n &le; 4 than for \a n &ge; 5.
         */
        typedef typename IntOfMinSize<(imageBits * n + 7) / 8>::utype Code;

        /**
         * The total number of permutations on \a n elements.
         * This is the size of the symmetric group <i>S<sub>n</sub></i>.
         */
        static const Index nPerms;

        /**
         * The total number of permutations on <i>n</i>-1 elements.  This is
         * the size of the symmetric group <i>S</i><sub><i>n</i>-1</sub>.
         */
        static const Index nPerms_1;

    private:
        Code code_;
            /**< The internal code representing this permutation. */

        static const Code idCode_;
            /**< The internal code for the identity permutation. */

        static const Code imageMask_;
            /**< A bitmask whose lowest \a imageBits are 1, and whose
                 remaining higher order bits are all 0. */

    public:
        /**
         * Creates the identity permutation.
         */
        NPerm();

        /**
         * Creates the transposition of \a a and \a b.
         * Note that \a a and \a b need not be distinct.
         *
         * \pre 0 &le; \a a,\a b < \a n.
         *
         * @param a the element to switch with \a b.
         * @param b the element to switch with \a a.
         */
        NPerm(int a, int b);

        /**
         * Creates a permutation mapping \a i to \a image[\a i] for each
         * 0 &le; \a i < \a n.
         *
         * \pre The array \a image contains \a n elements, which are
         * 0,...,<i>n</i>-1 in some order.
         *
         * \ifacespython Not present.
         *
         * @param image the array of images.
         */
        NPerm(const int* image);

        /**
         * Creates a permutation mapping
         * (\a a[0], ..., \a a[<i>n</i>-1]) to
         * (\a b[0], ..., \a b[<i>n</i>-1]) respectively.
         *
         * \pre Both arrays \a a and \a b contain \a n elements, which
         * are 0,...,<i>n</i>-1 in some order.
         *
         * \ifacespython Not present.
         *
         * @param a the array of preimages; this must have length \a n.
         * @param b the corresponding array of images; this must also have
         * length \a n.
         */
        NPerm(const int* a, const int* b);

        /**
         * Creates a permutation that is a clone of the given
         * permutation.
         *
         * @param cloneMe the permutation to clone.
         */
        NPerm(const NPerm& cloneMe);

        /**
         * Returns the internal code representing this permutation.
         * Note that the internal code is sufficient to reproduce the
         * entire permutation.
         *
         * The code returned will be a valid permutation code as
         * determined by isPermCode().
         *
         * @return the internal code.
         */
        Code getPermCode() const;

        /**
         * Sets this permutation to that represented by the given
         * internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code that will determine the
         * new value of this permutation.
         */
        void setPermCode(Code code);

        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code for the new permutation.
         * @return the permutation reprsented by the given internal
         * code.
         */
        static NPerm fromPermCode(Code code);

        /**
         * Determines whether the given integer is a valid internal
         * permutation code.  Valid permutation codes can be passed to
         * setPermCode() or fromPermCode(), and are returned by
         * getPermCode().
         *
         * @return \c true if and only if the given code is a valid
         * internal permutation code.
         */
        static bool isPermCode(Code newCode);

        /**
         * Sets this permutation to be equal to the given permutation.
         *
         * @param cloneMe the permutation whose value will be assigned
         * to this permutation.
         * @return a reference to this permutation.
         */
        NPerm& operator = (const NPerm& cloneMe);

        /**
         * Returns the composition of this permutation with the given
         * permutation.  If this permutation is <i>p</i>, the
         * resulting permutation will satisfy <tt>(p*q)[x] == p[q[x]]</tt>.
         *
         * @param q the permutation to compose this with.
         * @return the composition of both permutations.
         */
        NPerm operator * (const NPerm& q) const;

        /**
         * Finds the inverse of this permutation.
         *
         * @return the inverse of this permutation.
         */
        NPerm inverse() const;

        /**
         * Determines the sign of this permutation.
         *
         * @return 1 if this permutation is even, or -1 if this
         * permutation is odd.
         */
        int sign() const;

        /**
         * Determines the image of the given integer under this
         * permutation.
         *
         * @param source the integer whose image we wish to find.  This
         * should be between 0 and <i>n</i>-1 inclusive.
         * @return the image of \a source.
         */
        int operator[](int source) const;

        /**
         * Determines the preimage of the given integer under this
         * permutation.
         *
         * @param image the integer whose preimage we wish to find.  This
         * should be between 0 and <i>n</i>-1 inclusive.
         * @return the preimage of \a image.
         */
        int preImageOf(int image) const;

        /**
         * Determines if this is equal to the given permutation.
         * This is true if and only if both permutations have the same
         * images for all 0 &le; \a i < \a n.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * are equal.
         */
        bool operator == (const NPerm& other) const;

        /**
         * Determines if this differs from the given permutation.
         * This is true if and only if the two permutations have
         * different images for some 0 &le; \a i < \a n.
         *
         * @param other the permutation with which to compare this.
         * @return \c true if and only if this and the given permutation
         * differ.
         */
        bool operator != (const NPerm& other) const;

        /**
         * Lexicographically compares the images of (0,1,...,\a n-1) under this
         * and the given permutation.
         *
         * @param other the permutation with which to compare this.
         * @return -1 if this permutation produces a smaller image, 0 if
         * the permutations are equal, and 1 if this permutation produces
         * a greater image.
         */
        int compareWith(const NPerm& other) const;

        /**
         * Determines if this is the identity permutation.
         * This is true if and only if every integer
         * 0 &le; \a i < \a n is mapped to itself.
         *
         * @return \c true if and only if this is the identity
         * permutation.
         */
        bool isIdentity() const;

        /**
         * Returns the <i>i</i>th permutation on \a n elements, where
         * permutations are numbered lexicographically beginning at 0.
         *
         * Lexicographical ordering treats each permutation \a p as the
         * <i>n</i>-tuple (\a p[0], \a p[1], ..., \a p[<i>n</i>-1]).
         *
         * @param i the lexicographical index of the permutation; this
         * must be between 0 and <i>n</i>!-1 inclusive.
         * @return the <i>i</i>th permutation.
         */
        static NPerm atIndex(Index i);

        /**
         * Returns a string representation of this permutation.
         * The representation will consist of \a n adjacent digits
         * representing the images of 0,...,<i>n</i>-1 respectively.
         * If \a n > 10, then lower-case hexadecimal digits will be used.
         *
         * An example of a string representation for \a n = 5 is <tt>30421</tt>.
         *
         * @return a string representation of this permutation.
         */
        std::string str() const;

        /**
         * Returns a prefix of the string representation of this permutation,
         * containing only the images of the first \a len integers.
         *
         * @param len the length of the prefix required; this must be
         * between 0 and \a n inclusive.
         * @return the corresponding prefix of the string representation
         * of this permutation.
         */
        std::string trunc(unsigned len) const;

    private:
        /**
         * Creates a permutation from the given internal code.
         *
         * \pre the given code is a valid permutation code; see
         * isPermCode() for details.
         *
         * @param code the internal code from which the new permutation
         * will be created.
         */
        NPerm(Code code);
};

/**
 * Writes a string representation of the given permutation to the given
 * output stream.  The format will be the same as is used by
 * NPerm::str().
 *
 * @param out the output stream to which to write.
 * @param p the permutation to write.
 * @return a reference to \a out.
 *
 * @tparam n the number of objects being permuted.
 * This must be between 3 and 16 inclusive.
 */
template <int n>
inline REGINA_API std::ostream& operator << (std::ostream& out,
        const NPerm<n>& p) {
    return (out << p.str());
}

// Note that some of our permutation classes are specialised elsewhere.
// Do not explicitly drag in the headers nperm3.h, ..., nperm5.h for now.
template <> class NPerm<3>;
template <> class NPerm<4>;
template <> class NPerm<5>;

/*@}*/

// Static constants for NPerm

namespace {
    constexpr int64_t factorial(int n) {
        return (n <= 1 ? 1 : factorial(n - 1) * n);
    }
}

template <int n>
const typename NPerm<n>::Index NPerm<n>::nPerms = factorial(n);

template <int n>
const typename NPerm<n>::Index NPerm<n>::nPerms_1 = factorial(n-1);

namespace {
    constexpr int permImageBits(int n) {
        return (n <= 1 ? 0 : 1 + permImageBits((n + 1) / 2));
    }
}

template <int n>
const int NPerm<n>::imageBits = permImageBits(n);

namespace {
    template <int n>
    inline constexpr typename NPerm<n>::Code idCodePartial(int k) {
        return (k == 0 ? 0 :
            static_cast<typename NPerm<n>::Code>(k) <<
                (NPerm<n>::imageBits * k) | idCodePartial<n>(k - 1));
    }
}

template <int n>
const typename NPerm<n>::Code NPerm<n>::idCode_ = idCodePartial<n>(n);

template <int n>
const typename NPerm<n>::Code NPerm<n>::imageMask_ =
    (static_cast<typename NPerm<n>::Code>(1) << NPerm<n>::imageBits) - 1;

// Inline functions for NPerm

template <int n>
inline NPerm<n>::NPerm() : code_(idCode_) {
}

template <int n>
inline NPerm<n>::NPerm(int a, int b) : code_(idCode_) {
    code_ &= ~(imageMask_ << (imageBits * a));
    code_ &= ~(imageMask_ << (imageBits * b));
    code_ |= (static_cast<Code>(a) << (imageBits * b));
    code_ |= (static_cast<Code>(b) << (imageBits * a));
}

template <int n>
inline NPerm<n>::NPerm(const int* image) {
    code_ = 0;
    for (int i = 0; i < n; ++i)
        code_ |= (static_cast<Code>(image[i]) << (imageBits * i));
}

template <int n>
inline NPerm<n>::NPerm(const int* a, const int* b) {
    code_ = 0;
    for (int i = 0; i < n; ++i)
        code_ |= (static_cast<Code>(b[i]) << (imageBits * a[i]));
}

template <int n>
inline NPerm<n>::NPerm(const NPerm<n>& cloneMe) : code_(cloneMe.code_) {
}

template <int n>
inline NPerm<n>::NPerm(Code code) : code_(code) {
}

template <int n>
inline typename NPerm<n>::Code NPerm<n>::getPermCode() const {
    return code_;
}

template <int n>
inline void NPerm<n>::setPermCode(Code code) {
    code_ = code;
}

template <int n>
inline NPerm<n> NPerm<n>::fromPermCode(Code code) {
    return NPerm<n>(code);
}

template <int n>
bool NPerm<n>::isPermCode(Code code) {
    unsigned mask = 0;
    for (int i = 0; i < n; ++i)
        mask |= (1 << ((code >> (imageBits * i)) & imageMask_));
    return (mask + 1 == (1 << n));
}

template <int n>
inline NPerm<n>& NPerm<n>::operator = (const NPerm& cloneMe) {
    code_ = cloneMe.code_;
    return *this;
}

template <int n>
inline NPerm<n> NPerm<n>::operator * (const NPerm& q) const {
    Code c = 0;
    for (int i = 0; i < n; ++i)
        c |= (static_cast<Code>((*this)[q[i]]) << (imageBits * i));
    return NPerm<n>(c);
}

template <int n>
inline NPerm<n> NPerm<n>::inverse() const {
    Code c = 0;
    for (int i = 0; i < n; ++i)
        c |= (static_cast<Code>(i) << (imageBits * (*this)[i]));
    return NPerm<n>(c);
}

template <int n>
int NPerm<n>::sign() const {
    // This algorithm is quadratic in n.  Surely we can do better?
    bool even = true;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if ((*this)[i] > (*this)[j])
                even = ! even;
    return (even ? 1 : -1);
}

template <int n>
inline int NPerm<n>::operator[](int source) const {
    return (code_ >> (imageBits * source)) & imageMask_;
}

template <int n>
inline int NPerm<n>::preImageOf(int image) const {
    for (int i = 0; i < n; ++i)
        if (((code_ >> (imageBits * i)) & imageMask_) == image)
            return i;
    // We should never reach this point.
    return -1;
}

template <int n>
inline bool NPerm<n>::operator == (const NPerm& other) const {
    return (code_ == other.code_);
}

template <int n>
inline bool NPerm<n>::operator != (const NPerm& other) const {
    return (code_ != other.code_);
}

template <int n>
int NPerm<n>::compareWith(const NPerm& other) const {
    for (int i = 0; i < n; ++i) {
        if ((*this)[i] < other[i])
            return -1;
        if ((*this)[i] > other[i])
            return 1;
    }
    return 0;
}

template <int n>
inline bool NPerm<n>::isIdentity() const {
    return (code_ == idCode_);
}

template <int n>
NPerm<n> NPerm<n>::atIndex(Index i) {
    Code c = idCode_;
    int image[n];
    int p, q;
    for (p = 0; p < n; ++p) {
        image[n - p - 1] = i % (p + 1);
        i /= (p + 1);
    }
    for (p = n - 1; p >= 0; --p)
        for (q = p + 1; q < n; ++q)
            if (image[q] >= image[p])
                ++image[q];
    return NPerm<n>(image);
}

template <int n>
std::string NPerm<n>::str() const {
    char ans[n + 1];
    int image;
    for (int i = 0; i < n; ++i) {
        image = (code_ >> (imageBits * i)) & imageMask_;
        ans[i] = (image < 10 ? '0' + image : 'a' - 10 + image);
    }
    ans[n] = 0;

    return ans;
}

template <int n>
std::string NPerm<n>::trunc(unsigned len) const {
    char ans[n + 1];
    int image;
    for (int i = 0; i < len; ++i) {
        image = (code_ >> (imageBits * i)) & imageMask_;
        ans[i] = (image < 10 ? '0' + image : 'a' - 10 + image);
    }
    ans[len] = 0;

    return ans;
}

} // namespace regina

#endif

