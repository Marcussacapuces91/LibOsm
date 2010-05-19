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

#ifndef TESTS_H
#define TESTS_H

#include <expat.h>
#include <sstream>
#include <iostream>
#include "xml/parserosm.h"

using namespace std;

class Tests : public BaseInterface
{
    public:
        Tests()
        {
            std::istringstream iss("<way id=\"5090250\" visible=\"true\" timestamp=\"2009-01-19T19:07:25Z\" version=\"8\" changeset=\"816806\" user=\"Blumpsy\" uid=\"64226\"> \
                                   <nd ref=\"822403\"/>                          \
                                   <nd ref=\"21533912\"/>                        \
                                   <nd ref=\"821601\"/>                          \
                                   <nd ref=\"21533910\"/>                        \
                                   <nd ref=\"135791608\"/>                       \
                                   <nd ref=\"333725784\"/>                       \
                                   <nd ref=\"333725781\"/>                       \
                                   <nd ref=\"333725774\"/>                       \
                                   <nd ref=\"333725776\"/>                       \
                                   <nd ref=\"823771\"/>                          \
                                   <tag k=\"created_by\" v=\"Potlatch 0.10e\"/>  \
                                   <tag k=\"highway\" v=\"unclassified\"/>       \
                                   <tag k=\"name\" v=\"Clipstone Street\"/>      \
                                   <tag k=\"oneway\" v=\"yes\"/>                 \
                               </way>");

            XML_Parser xml_parser = XML_ParserCreate(0);
            ParserOsm parser(&xml_parser, *this, 0, string("osm"), vector< pair<string, string> >());
        }

        void add(const Changeset& aChangeset)
        {
        }

        void add(const Node& aNode)
        {
        }

        void add(const Way& aWay)
        {
        }

        void add(const Relation& aRelation)
        {
        }

};

#endif
