
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/pachner-impl.h"
#include "triangulation/detail/skeleton-impl.h"
#include "triangulation/generic.h"

namespace regina { namespace detail {

template REGINA_API std::string TriangulationBase<6>::isoSigFrom(
    size_t, const Perm<7>&, Isomorphism<6>*) const;
template REGINA_API std::string TriangulationBase<6>::isoSig(
    Isomorphism<6>**) const;
template REGINA_API Triangulation<6>* TriangulationBase<6>::fromIsoSig(
    const std::string&);
template REGINA_API size_t TriangulationBase<6>::isoSigComponentSize(
    const std::string&);

template REGINA_API void TriangulationBase<6>::calculateSkeleton();
template REGINA_API void TriangulationBase<6>::clearBaseProperties();
template REGINA_API void TriangulationBase<6>::swapBaseProperties(
    TriangulationBase<6>&);

template REGINA_API bool PachnerHelper<6, 0>::pachner(
    Triangulation<6>*, Face<6, 0>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 1>::pachner(
    Triangulation<6>*, Face<6, 1>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 2>::pachner(
    Triangulation<6>*, Face<6, 2>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 3>::pachner(
    Triangulation<6>*, Face<6, 3>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 4>::pachner(
    Triangulation<6>*, Face<6, 4>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 5>::pachner(
    Triangulation<6>*, Face<6, 5>*, bool, bool);
template REGINA_API bool PachnerHelper<6, 6>::pachner(
    Triangulation<6>*, Face<6, 6>*, bool, bool);

} }
