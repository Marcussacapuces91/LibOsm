/*
    Copyright � 2010 par Marc Sibert

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

// Class automatically generated by Dev-C++ New Class wizard

#include "top.h" // class's header file

#include <sstream>
#include <iostream>
#include "osm_exception.h"

using namespace std;

string Top::xmlEntities(const string& aXml)
{
    string result = "";
    for (string::const_iterator i = aXml.begin(); i !=aXml.end(); ++i)
        switch (*i) {
            case '"' : result += "&quot;"; break;
            case '&' : result += "&amp;"; break;
            case 0x27 : result += "&apos;"; break;
            case '<' : result += "&lt;"; break;
            case '>' : result += "&gt;"; break;
            default : result += *i;
        }
    return result;
}

void Top::setAttribut(const string& aKey, const string& aValue)
{
    istringstream iss(aValue);
    if (aKey == "id") iss >> fId;
    else if (aKey == "user") fUser = aValue;
    else if (aKey == "uid") iss >> fUid;
    else if (aKey == "osmxapi:users") {
#ifdef __DEBUG__
        cerr << "osmxapi:users = " << aValue << " ignore." << endl;
#endif
    }
    else
        throw OsmException(string("Attribut " + aKey + " inconnu"),
                           __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

const string& Top::getTag(const string& aKey) const
{
    const ListeTags::const_iterator i = fTags.find(aKey);
    if (i == fTags.end())
        throw OsmException(string("Tag ") + aKey + " introuvable",
                           __FILE__, __LINE__, __PRETTY_FUNCTION__);
    return i->second;
}

void Top::setTag(const string& aKey, const string& aValue)
{
    fTags[aKey] = aValue;
}

ostream& Top::afficher(ostream& aStream) const
{
    if (fId != 0)
        aStream << "id=\"" << fId << "\" ";
    if (!fUser.empty())
        aStream << "user=\"" << xmlEntities(fUser) << "\" ";
    if (!fUid != 0)
        aStream << "uid=\"" << fUid << "\" ";
    return aStream;
}

ostream& Top::afficherTags(ostream& aStream) const
{
    for (ListeTags::const_iterator i = fTags.begin(); i != fTags.end(); ++i) {
        aStream << "\t\t<tag k=\"" << xmlEntities(i->first)
                << "\" v=\"" << xmlEntities(i->second) << "\" />" << endl;
    }
    return aStream;
}

bool Top::operator==(const Top& aTop) const
{
// M�me adresses ?
    if (this == &aTop) return true;
// Attributs identiques ?
    if ((fId != aTop.fId) ||
        (fUser != aTop.fUser) ||
        (fUid != aTop.fUid)) return false;
// Listes des Tags identiques ?
    if (fTags.size() != aTop.fTags.size()) return false;

    ListeTags::const_iterator a = fTags.begin();
    ListeTags::const_iterator b = aTop.fTags.begin();
    for (unsigned i = 0; i < fTags.size(); ++i) {
        if (*a++ != *b++) return false;
    }
// Sinon ce sont les m�mes !
    return true;
}
