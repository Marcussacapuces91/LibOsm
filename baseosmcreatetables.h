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

#ifndef BASEOSMCREATETABLES_H
#define BASEOSMCREATETABLES_H

#include "basesqlite3.h" // inheriting class's header file
//#include "xml/baseinterface.h"
//#include "osm/way.h"
//#include "osm/member.h"


/**
 * Classe assurant la création des tables (si nécessaire) *avant* la création
 * des Commandes Préparées.
 */
class BaseOsmCreateTables : public BaseSQLite3
{
    public:
        BaseOsmCreateTables(const string& aPath,
                            const bool aInitSpatialite,
                            const int aFlags) :
            BaseSQLite3(aPath, aFlags)
        {
            if (aInitSpatialite) execFile("init_spatialite-2.3.sql");

//
// Table pour stocker les users (pour tous les éléments).
//
            exec("CREATE TABLE IF NOT EXISTS user (                            \
                    id INTEGER PRIMARY KEY,                                    \
                    name TEXT)");

//
// Table pour stocker les tags (pour tous les éléments).
//
            exec("CREATE TABLE IF NOT EXISTS tag (                             \
                    id INTEGER PRIMARY KEY AUTOINCREMENT,                      \
                    key TEXT,                                                  \
                    value TEXT,                                                \
                    UNIQUE (key, value))");

//
// Tables pour gérer les changesets.
//
            exec("CREATE TABLE IF NOT EXISTS changeset (                       \
                    id INTEGER PRIMARY KEY,                                    \
                    user TEXT DEFAULT NULL,                                    \
                    uid INTEGER REFERENCES user,                               \
                    created_at TEXT NOT NULL,                                  \
                    num_changes INTEGER NOT NULL,                              \
                    closed_at TEXT DEFAULT NULL,                               \
                    open INTEGER(1) NOT NULL);                                 \
                                                                               \
                SELECT AddGeometryColumn('changeset','mbr',4326,'POLYGON',2,1);\
                                                                               \
                CREATE TABLE IF NOT EXISTS changeset_tags (                    \
                    id_changeset INTEGER REFERENCES changeset,                 \
                    id_tag INTEGER NOT NULL REFERENCES tag,                    \
                    PRIMARY KEY (id_changeset, id_tag))");

//
// Tables pour gérer les nodes.
//
            exec("CREATE TABLE IF NOT EXISTS node (                            \
                    id INTEGER PRIMARY KEY,                                    \
                    version INTEGER,                                           \
                    changeset INTEGER REFERENCES changeset,                    \
                    uid INTEGER REFERENCES user,                               \
                    visible INTEGER(1) DEFAULT 1 NOT NULL,                     \
                    timestamp TEXT NOT NULL);                                  \
                                                                               \
                SELECT AddGeometryColumn('node', 'coord', 4326, 'POINT', 2, 1);\
                                                                               \
                CREATE TABLE IF NOT EXISTS node_tags (                         \
                    id_node INTEGER NOT NULL REFERENCES node,                  \
                    id_tag INTEGER NOT NULL REFERENCES tag,                    \
                    PRIMARY KEY (id_node, id_tag))");


//		SELECT CreateSpatialIndex('node','coord');

//
// Tables pour gérer les way.
//
            exec("CREATE TABLE IF NOT EXISTS way (                             \
                    id INTEGER PRIMARY KEY,                                    \
                    version INTEGER,                                           \
                    changeset INTEGER REFERENCES changeset,                    \
                    uid INTEGER REFERENCES user,                               \
                    visible INTEGER(1) DEFAULT 1 NOT NULL,                     \
                    timestamp TEXT NOT NULL);                                  \
                                                                               \
                CREATE TABLE IF NOT EXISTS way_nodes (                         \
                    id_way INTEGER REFERENCES way,                             \
                    rang INTEGER(5),                                           \
                    id_node INTEGER REFERENCES node,                           \
                    PRIMARY KEY (id_way, rang));                               \
                                                                               \
                CREATE TABLE IF NOT EXISTS way_tags (                          \
                    id_way INTEGER NOT NULL REFERENCES way,                    \
                    id_tag INTEGER NOT NULL REFERENCES tag,                    \
                    PRIMARY KEY (id_way, id_tag))");

//
// Tables pour gérer les relations.
//
            exec("CREATE TABLE IF NOT EXISTS relation (                        \
                    id INTEGER PRIMARY KEY,                                    \
                    version INTEGER,                                           \
                    changeset INTEGER REFERENCES changeset,                    \
                    uid INTEGER REFERENCES user,                               \
                    visible INTEGER(1) DEFAULT 1 NOT NULL,                     \
                    timestamp TEXT NOT NULL);                                  \
                                                                               \
                CREATE TABLE IF NOT EXISTS relation_tags (                     \
                    id_relation INTEGER NOT NULL REFERENCES relation,          \
                    id_tag INTEGER NOT NULL REFERENCES tag,                    \
                    PRIMARY KEY (id_relation, id_tag));                        \
                                                                               \
                CREATE TABLE IF NOT EXISTS relation_members (                  \
                    id_relation INTEGER NOT NULL REFERENCES relation,          \
                    rang INTEGER(5),                                           \
                    type INTEGER(1) NOT NULL,                                  \
                    id_member INTEGER NOT NULL,                                \
                    role TEXT NULL,                                            \
                    PRIMARY KEY (id_relation, rang, type, id_member))");

        }

/**
 * \brief Destructeur virtuel de l'instance.
 *
 * Sans action.
 */
        virtual ~BaseOsmCreateTables(void) {};

    private:
        BaseOsmCreateTables(const BaseOsmCreateTables&);
        BaseOsmCreateTables& operator=(const BaseOsmCreateTables&);

};

#endif // BASEOSMCREATETABLES_H
