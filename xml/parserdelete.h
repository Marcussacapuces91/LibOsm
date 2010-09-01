/*
    Copyright © 2010 par Marc Sibert

    This file is part of LIBOSM

    LIBOSM is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LIBOSM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LIBOSM.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 * \author Marc Sibert
 */

#ifndef PARSERDELETE_H
#define PARSERDELETE_H

// #include "parser.h"     // header de la classe héritée.
#include "parserconteneurelement.h"    // header de l'interface héritée.

#include "baseinterface.h"

/**
 *
 */
class ParserDelete : public ParserConteneurElement
{
    public:
/**
 * Default constructor
 */
        ParserDelete(XML_Parser *const apXML_Parser,
                     BaseInterface& aBase,
                     Parser *const apParent,
                     const string& aName,
                     const vector< pair<string, string> >& aAtts);

 /**
  * Default destructor
  */
        virtual ~ParserDelete() {}

        void traiter(const Node& aNode);
        void traiter(const Way& aWay);
        void traiter(const Relation& aRelation);
        void traiter(const Changeset&) { throw -1; }

    protected:
        virtual void startElement(const string& aName,
                                  const vector< pair<string, string> >& aAtts);

        virtual void endElement(const string& aName);

    private:
        BaseInterface& fBase;

        Element* fpElement;

/** Copy constructor
 *  \param other Object to copy from
 */
        ParserDelete(const ParserDelete& other);

/** Assignment operator
 *  \param other Object to assign from
 *  \return A reference to this
 */
        ParserDelete& operator=(const ParserDelete& other);

};

#endif // PARSERDELETE_H
