
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file engine.h
 *  \brief Provides global routines and classes for interfacing with the
 *  Regina calculation engine.
 */

#ifndef __ENGINE_H
#ifndef __DOXYGEN
#define __ENGINE_H
#endif

/**
 * Contains the entire Regina calculation engine.
 *
 * \todo \bugurgent Hunt for default copy constructors that are being called.
 * \todo \featurelong Enhance the test suite for the calculation engine.
 */
namespace regina {

class NAbelianGroup;
class NAngleStructureList;
class NAugTriSolidTorus;
class NContainer;
class NFile;
class NFileInfo;
class NGroupExpression;
class NGroupPresentation;
class NLayeredChain;
class NLayeredLensSpace;
class NLayeredLoop;
class NLayeredSolidTorus;
class NMatrixInt;
class NNormalSurfaceList;
class NPlugTriSolidTorus;
class NProgressManager;
class NScript;
class NSignature;
class NSnappedBall;
class NSnappedTwoSphere;
class NSpiralSolidTorus;
class NSurfaceFilter;
class NSurfaceFilterCombination;
class NSurfaceFilterProperties;
class NSurfaceSubset;
class NTetrahedron;
class NText;
class NTriSolidTorus;
class NTriangulation;

/**
 * \addtogroup engine Engine Structure
 * Overall structure of the calculation engine.
 * @{
 */

/**
 * Returns the full version number of this calculation engine.
 * For instance, version 2.3.1 would have full version <tt>"2.3.1"</tt>.
 *
 * @return the version of this calculation engine.
 */
const char* getVersionString();

/**
 * Returns the major version number of this calculation engine.
 * For instance, version 2.3.1 would have major version 2.
 *
 * @return the major version number of this calculation engine.
 */
int getVersionMajor();

/**
 * Returns the minor version number of this calculation engine.
 * For instance, version 2.3.1 would have minor version 3.
 *
 * @return the minor version number of this calculation engine.
 */
int getVersionMinor();

/**
 * Tests to see if an interface can successfully communicate with the
 * underlying C++ calculation engine.
 *
 * This routine simply uses the engine to return the same value that is
 * passed to it; it can be used to test whether communications between
 * the interface and the C++ engine are working properly.
 *
 * @param value any integer; this same integer will be returned.
 * @return the same integer that was passed as \a value.
 */
int testEngine(int value);

#ifdef __DOXYGEN

/**
 * Represents a link from an external interface to the calculation
 * engine.  Objects in the underlying engine can be created through an
 * instance of this class.  Global engine routines and variables are also
 * accessible.
 *
 * Notes for routines in this class can be found with the
 * corresponding constructor / global routine / global variable
 * descriptions and not here in the \a Engine class notes.
 *
 * \ifacescpp Not present.
 */
class Engine {
    public:
        /**
         * \name Class Constructors
         */
        /*@{*/

        NAbelianGroup* newNAbelianGroup();
            /**< Calls the corresponding constructor. */
        NAbelianGroup* newNAbelianGroup(const NAbelianGroup& cloneMe);
            /**< Calls the corresponding constructor. */
        NAngleStructureList* newNAngleStructureList(NTriangulation* owner);
            /**< Calls the corresponding constructor. */
        NContainer* newNContainer();
            /**< Calls the corresponding constructor. */
        NFile* newNFile();
            /**< Calls the corresponding constructor. */
        NGroupExpression* newNGroupExpression();
            /**< Calls the corresponding constructor. */
        NGroupExpression* newNGroupExpression(const NGroupExpression& cloneMe);
            /**< Calls the corresponding constructor. */
        NGroupPresentation* newNGroupPresentation();
            /**< Calls the corresponding constructor. */
        NGroupPresentation* newNGroupPresentation(
                const NGroupPresentation& cloneMe);
            /**< Calls the corresponding constructor. */
        NLayeredChain* newNLayeredChain(NTetrahedron* tet,
                NPerm vertexRoles);
            /**< Calls the corresponding constructor. */
        NLayeredChain* newNLayeredChain(NLayeredChain* cloneMe);
            /**< Calls the corresponding constructor. */
        NLensSpace* newNLensSpace(unsigned long p, unsigned long q);
            /**< Calls the corresponding constructor. */
        NLensSpace* newNLensSpace(const NLensSpace& cloneMe);
            /**< Calls the corresponding constructor. */
        NMatrixInt* newNMatrixInt(int rows, int columns);
            /**< Calls the corresponding constructor. */
        NMatrixInt* newNMatrixInt(const NMatrixInt& cloneMe);
            /**< Calls the corresponding constructor. */
        NNormalSurfaceList* newNNormalSurfaceList(NTriangulation* owner,
                int flavour, bool isEmbeddedOnly = true);
            /**< Calls the corresponding constructor. */
        NProgressManager* newNProgressManager();
            /**< Calls the corresponding constructor. */
        NSFS* newNSFS();
            /**< Calls the corresponding constructor. */
        NSFS* newNSFS(unsigned long newOrbitGenus, bool newOrbitOrientable,
                unsigned long newOrbitPunctures);
            /**< Calls the corresponding constructor. */
        NSFS* newNSFS(const NSFS& cloneMe);
            /**< Calls the corresponding constructor. */
        NScript* newNScript();
            /**< Calls the corresponding constructor. */
        NSignature* newNSignature(const NSignature& cloneMe);
            /**< Calls the corresponding constructor. */
        NSurfaceFilter* newNSurfaceFilter();
            /**< Calls the corresponding constructor. */
        NSurfaceFilter* newNSurfaceFilter(const NSurfaceFilter& cloneMe);
            /**< Calls the corresponding constructor. */
        NSurfaceFilterCombination* newNSurfaceFilterCombination();
            /**< Calls the corresponding constructor. */
        NSurfaceFilterCombination* newNSurfaceFilterCombination(
                const NSurfaceFilterCombination& cloneMe);
            /**< Calls the corresponding constructor. */
        NSurfaceFilterProperties* newNSurfaceFilterProperties();
            /**< Calls the corresponding constructor. */
        NSurfaceFilterProperties* newNSurfaceFilterProperties(
                const NSurfaceFilterProperties& cloneMe);
            /**< Calls the corresponding constructor. */
        NSurfaceSubset* newNSurfaceSubset(const NSurfaceSet& set,
                const NSurfaceFilter& filter);
            /**< Calls the corresponding constructor. */
        NTetrahedron* newNTetrahedron();
            /**< Calls the corresponding constructor. */
        NTetrahedron* newNTetrahedron(const char* desc);
            /**< Calls the corresponding constructor. */
        NText* newNText();
            /**< Calls the corresponding constructor. */
        NText* newNText(const char* text);
            /**< Calls the corresponding constructor. */
        NTriangulation* newNTriangulation();
            /**< Calls the corresponding constructor. */
        NTriangulation* newNTriangulation(const NTriangulation& cloneMe);
            /**< Calls the corresponding constructor. */

        /*@}*/
        /**
         * (end: Class Constructors)
         */

        /**
         * \name Global and Static Routines
         */
        /*@{*/

        NTriangulation* enterTextTriangulation();
            /**< Calls NTriangulation::enterTextTriangulation(). */
        long formCensus(NPacket* parent, int nTetrahedra,
                NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
                int nBdryFaces, NProgressManager* manager = 0);
            /**< Calls ::formCensus(). */
        NSnappedTwoSphere* formsSnappedTwoSphere(NSnappedBall* ball1,
                NSnappedBall* ball2);
            /**< Calls NSnappedTwoSphere::formsSnappedTwoSphere(). */
        NSnappedTwoSphere* formsSnappedTwoSphere(NTetrahedron* tet1,
                NTetrahedron* tet2);
            /**< Calls NSnappedTwoSphere::formsSnappedTwoSphere(). */
        int getVersionMajor();
            /**< Calls ::getVersionMajor(). */
        int getVersionMinor();
            /**< Calls ::getVersionMinor(). */
        std::string getVersionString();
            /**< Calls ::getVersionString(). */
        NFileInfo* identifyFileInfo(const std::string& idPathname);
            /**< Calls NFileInfo::identify(). */
        NAugTriSolidTorus* isAugTriSolidTorus(const NComponent* comp);
            /**< Calls NAugTriSolidTorus::isAugTriSolidTorus(). */
        NSFS* isKnownSFS(NTriangulation* tri);
            /**< Calls ::isKnownSFS(). */
        NLayeredChainPair* isLayeredChainPair(const NComponent* comp);
            /**< Calls NLayeredChainPair::isLayeredChainPair(). */
        NLayeredLensSpace* isLayeredLensSpace(const NComponent* comp);
            /**< Calls NLayeredLensSpace::isLayeredLensSpace(). */
        NLayeredLoop* isLayeredLoop(const NComponent* comp);
            /**< Calls NLayeredLoop::isLayeredLoop(). */
        NLayeredSolidTorus* isLayeredSolidTorusBase(NTetrahedron* tet);
            /**< Calls NLayeredSolidTorus::isLayeredSolidTorusBase(). */
        NPlugTriSolidTorus* isPlugTriSolidTorus(NTriangulation* tri);
            /**< Calls NPlugTriSolidTorus::isPlugTriSolidTorus(). */
        NSnappedBall* isSnappedBall(NTetrahedron* tet);
            /**< Calls NSnappedBall::isSnappedBall(). */
        NSpiralSolidTorus* isSpiralSolidTorus(NTetrahedron* tet, NPerm roles);
            /**< Calls NSpiralSolidTorus::isSpiralSolidTorus(). */
        NTriSolidTorus* isTriSolidTorus(NTetrahedron* tet, NPerm vertexRoles);
            /**< Calls NTriSolidTorus::isTriSolidTorus(). */
        NMatrixInt* makeMatchingEquations(NTriangulation* triangulation,
                int flavour);
            /**< Calls ::makeMatchingEquations(). */
        NSignature* parseSignature(const std::string& sig);
            /**< Calls NSignature::parse(). */
        NPacket* readFileMagic(const std::string& fileName);
            /**< Calls ::readFileMagic(). */
        NPacket* readFromFile(const char* fileName);
            /**< Calls ::readFromFile(). */
        NPacket* readXMLFile(const char* fileName);
            /**< Calls ::readXMLFile(). */
        NTriangulation* readSnapPea(const char* fileName);
            /**< Calls ::readSnapPea(). */
        void smithNormalForm(NMatrixInt* matrix);
            /**< Calls ::smithNormalForm(). */
        int testEngine(int value);
            /**< Calls ::testEngine(). */
        bool writeToFile(const char* fileName, NPacket* packet);
            /**< Calls ::writeToFile(). */
        bool writeXMLFile(const char* fileName, NPacket* subtree,
                bool compressed = true);
            /**< Calls ::writeXMLFile(). */

        /*@}*/
        /**
         * (end: Global and Static Routines)
         */
};

#endif // doxygen-only declarations

/*@}*/

} // namespace regina

#endif
