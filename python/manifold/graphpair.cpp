
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "../pybind11/pybind11.h"
#include "manifold/graphpair.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using regina::GraphPair;
using regina::Matrix2;
using regina::SFSpace;

void addGraphPair(pybind11::module& m) {
    pybind11::class_<GraphPair, regina::Manifold>(m, "GraphPair")
        .def(pybind11::init([](const SFSpace& s1, const SFSpace& s2,
                long a, long b, long c, long d) {
            // Clone the given SFSs to avoid claiming ownership of them.
            return new GraphPair(new SFSpace(s1), new SFSpace(s2), a, b, c, d);
        }))
        .def(pybind11::init([](const SFSpace& s1, const SFSpace& s2,
                const Matrix2& m) {
            // Clone the given SFSs to avoid claiming ownership of them.
            return new GraphPair(new SFSpace(s1), new SFSpace(s2), m);
        }))
        .def(pybind11::init<const GraphPair&>())
        .def("sfs", &GraphPair::sfs,
            pybind11::return_value_policy::reference_internal)
        .def("matchingReln", &GraphPair::matchingReln,
            pybind11::return_value_policy::reference_internal)
    ;

    m.attr("NGraphPair") = m.attr("GraphPair");
}

