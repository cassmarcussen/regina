
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file packet/nscript.h
 *  \brief Contains a packet representing a script.
 */

#ifndef __NSCRIPT_H
#ifndef __DOXYGEN
#define __NSCRIPT_H
#endif

#include <map>
#include <utility>
#include <vector>
#include "regina-core.h"
#include "packet/npacket.h"

namespace regina {

class NXMLPacketReader;
class NScript;

/**
 * \weakgroup packet
 * @{
 */

/**
 * Stores information about the script packet.
 * See the general PacketInfo template notes for further details.
 *
 * \ifacespython Not present.
 */
template <>
struct PacketInfo<PACKET_SCRIPT> {
    typedef NScript Class;
    inline static const char* name() {
        return "Script";
    }
};

/**
 * A packet representing a Python script that can be run.
 * Accessor methods for a script work a line at a time.
 *
 * As of Regina 4.95, variables are now stored as pointers to packets, not
 * packet labels.  This affects how variables react to changes in the
 * packets that they point to.  In particular, if a variable \a V points to
 * some packet \a P, then as of Regina 4.95:
 *
 * - if \a P is renamed then \a V will still point to it and the script will
 *   \e not notify listeners of any changes (though of course \a P will
 *   still notify its own listeners);
 * - if \a P is deleted then \a V will take the value \c None, and the script
 *   \e will notify listeners of the change.
 */
class REGINA_API NScript : public NPacket, public NPacketListener {
    REGINA_PACKET(NScript, PACKET_SCRIPT)

    private:
        std::vector<std::string> lines;
            /**< An array storing the lines of this script; none of
             *   these strings should contain newlines. */
        std::map<std::string, NPacket*> variables;
            /**< A map storing the variables with which this script
             *   is to be run.  Variable names are mapped to their
             *   corresponding values. */

    public:
        /**
         * Initialises to a script with no lines and no variables.
         */
        NScript();

        /**
         * Returns the number of lines in this script.
         *
         * @return the number of lines.
         */
        unsigned long getNumberOfLines() const;
        /**
         * Returns the requested line of this script.
         *
         * @param index the index of the requested line; this must be
         * between 0 and getNumberOfLines()-1 inclusive.
         * @return the requested line.
         */
        const std::string& getLine(unsigned long index) const;

        /**
         * Adds the given line to the beginning of this script.
         *
         * @param line the line to insert.
         */
        void addFirst(const std::string& line);
        /**
         * Adds the given line to the end of this script.
         *
         * @param line the line to add.
         */
        void addLast(const std::string& line);
        /**
         * Inserts the given line into the given position in this script.
         * All subsequent lines will be shifted down to make room.
         *
         * @param line the line to insert.
         * @param index the index at which the new line will be placed;
         * this must be between 0 and getNumberOfLines() inclusive.
         */
        void insertAtPosition(const std::string& line, unsigned long index);
        /**
         * Replaces a line of this script with the given line.
         *
         * @param line the line to replace the currently stored line.
         * @param index the index of the line to replace; this must be
         * between 0 and getNumberOfLines()-1 inclusive.
         */
        void replaceAtPosition(const std::string& line, unsigned long index);
        /**
         * Removes the requested line from this script.
         *
         * @param index the index of the requested line; this must be
         * between 0 and getNumberOfLines()-1 inclusive.
         */
        void removeLineAt(unsigned long index);
        /**
         * Removes all lines from this script.
         */
        void removeAllLines();

        /**
         * Returns the number of variables associated with this script.
         *
         * @return the number of variables.
         */
        unsigned long getNumberOfVariables() const;
        /**
         * Returns the name of the requested variable associated with
         * this script.
         *
         * @param index the index of the requested variable; this must
         * be between 0 and getNumberOfVariables()-1 inclusive.
         * @return the name of the requested variable.
         */
        const std::string& getVariableName(unsigned long index) const;
        /**
         * Returns the value of the requested variable associated with
         * this script.  Variables may take the value \c null.
         *
         * @param index the index of the requested variable; this must
         * be between 0 and getNumberOfVariables()-1 inclusive.
         * @return the value of the requested variable.
         */
        NPacket* getVariableValue(unsigned long index) const;
        /**
         * Returns the value of the variable stored with the given
         * name.  Variables may take the value \c null.
         *
         * If no variable is stored with the given name, then \c null
         * will likewise be returned.
         *
         * @param name the name of the requested variable; note that
         * names are case sensitive.
         * @return the value of the requested variable.
         */
        NPacket* getVariableValue(const std::string& name) const;

        /**
         * Adds a new variable to be associated with this script.
         * If a variable with the given name is already stored, this
         * routine will do nothing.
         *
         * @param name the name of the new variable.
         * @param value the value of the new variable; this is allowed
         * to be \c null.
         * @return \c true if the variable was successfully added, or
         * \c false if a variable with the given name was already stored.
         */
        bool addVariable(const std::string& name, NPacket* value);
        /**
         * Removes the variable stored with the given name.
         * Note that the indices of other variables may change as a
         * result of this action.
         *
         * If no variable is stored with the given name, this routine
         * will do nothing (but waste time!).
         *
         * @param name the name of the variable to remove; note that
         * names are case sensitive.
         */
        void removeVariable(const std::string& name);
        /**
         * Removes all variables associated with this script.
         */
        void removeAllVariables();

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent,
            NXMLTreeResolver& resolver);
        virtual bool dependsOnParent() const;

        virtual void packetToBeDestroyed(NPacket* packet);

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NScript

inline NScript::NScript() {
}

inline unsigned long NScript::getNumberOfLines() const {
    return lines.size();
}
inline const std::string& NScript::getLine(unsigned long index) const {
    return lines[index];
}

inline void NScript::addFirst(const std::string& line) {
    ChangeEventSpan span(this);
    lines.insert(lines.begin(), line);
}
inline void NScript::addLast(const std::string& line) {
    ChangeEventSpan span(this);
    lines.push_back(line);
}
inline void NScript::insertAtPosition(const std::string& line,
        unsigned long index) {
    ChangeEventSpan span(this);
    lines.insert(lines.begin() + index, line);
}
inline void NScript::replaceAtPosition(const std::string& line,
        unsigned long index) {
    ChangeEventSpan span(this);
    lines[index] = line;
}
inline void NScript::removeLineAt(unsigned long index) {
    ChangeEventSpan span(this);
    lines.erase(lines.begin() + index);
}
inline void NScript::removeAllLines() {
    ChangeEventSpan span(this);
    lines.clear();
}

inline unsigned long NScript::getNumberOfVariables() const {
    return variables.size();
}
inline bool NScript::addVariable(const std::string& name, NPacket* value) {
    ChangeEventSpan span(this);
    bool ans = variables.insert(std::make_pair(name, value)).second;
    if (value)
        value->listen(this);
    return ans;
}
inline void NScript::removeAllVariables() {
    unregisterFromAllPackets();

    ChangeEventSpan span(this);
    variables.clear();
}

inline void NScript::writeTextShort(std::ostream& o) const {
    o << "Script with " << lines.size() << " line";
    if (lines.size() != 1)
        o << 's';
}

inline bool NScript::dependsOnParent() const {
    return false;
}

} // namespace regina

#endif

