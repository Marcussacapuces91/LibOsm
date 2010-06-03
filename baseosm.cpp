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

#include "baseosm.h" // class's header file

#include <sstream>
#include "exception.h"
// #include "libspatialite/headers/spatialite.h"
#include "libspatialite/headers/spatialite/gaiageo.h"
#include "osm/way.h"
#include "osm/relation.h"
#include <cstdlib>


using namespace std;

BaseOsm::BaseOsm(const string& aPath,
                 const bool aInitSpatialite,
                 const int aFlags) :
	BaseOsmCreateTables(aPath, aInitSpatialite, aFlags),
    fInsertUser(this->fpSqlite3, "INSERT OR IGNORE INTO user (id, name) VALUES (?,?)"),
	fSelectTag(this->fpSqlite3, "SELECT id FROM tag WHERE (key=? AND value=?)"),
    fInsertTag(this->fpSqlite3, "INSERT INTO tag (key, value) VALUES (?,?)"),
    fInsertChangeset(this->fpSqlite3, "INSERT INTO changeset (id, user, uid, created_at, closed_at, open, mbr) VALUES (?,?,?,?,?,?,?)"),
    fInsertChangesetTags(this->fpSqlite3, "INSERT INTO changeset (id, user, uid, created_at, closed_at, open, mbr) VALUES (?,?,?,?,?,?,?)"),
    fInsertNode(this->fpSqlite3, "INSERT INTO node (id,version,changeset,uid,visible,timestamp,coord) VALUES (?,?,?,?,?,?,?)"),
	fInsertNodeTags(this->fpSqlite3, "INSERT INTO node_tags (id_node, id_tag) VALUES (?,?)"),
    fInsertWay(this->fpSqlite3, "INSERT INTO way (id,version,changeset,uid,visible,timestamp) VALUES (?,?,?,?,?,?)"),
    fInsertWayTags(this->fpSqlite3, "INSERT INTO way_tags (id_way, id_tag) VALUES (?,?)"),
    fInsertWayNodes(this->fpSqlite3, "INSERT INTO way_nodes (id_way, id_node, rang) VALUES (?,?,?)"),
    fInsertRelation(this->fpSqlite3, "INSERT INTO relation (id,version,changeset,uid,visible,timestamp) VALUES (?,?,?,?,?,?)"),
    fInsertRelationTags(this->fpSqlite3, "INSERT INTO relation_tags (id_relation, id_tag) VALUES (?,?)"),
    fInsertRelationMembers(this->fpSqlite3, "INSERT INTO relation_members (id_relation, type, id_member, role, rang) VALUES (?,?,?,?,?)"),
	fNbChangesets(0),
	fNbNodes(0),
    fNbWays(0),
    fNbRelations(0)
{
//	exec("PRAGMA cache_size = 1000000");    // Déjà défini dans main.
	exec("PRAGMA synchronous = OFF");
	exec("PRAGMA journal_mode = OFF");
	exec("PRAGMA temp_store =  MEMORY");

//	sqlite3_progress_handler(fpSqlite3, 1000, BaseOsm::progress, this);
}

/*
BaseOsm::~BaseOsm(void)
{
    cerr << __PRETTY_FUNCTION__ << endl;
}
*/

/*
int BaseOsm::progress(void *const apBaseOsm)
{
	BaseOsm *const pBaseOsm = static_cast<BaseOsm *const>(apBaseOsm);

	return EXIT_SUCCESS;
}
*/

void BaseOsm::createIndexes()
{
	exec("BEGIN");
// Changesets
// cerr << endl << "Indexes de Changeset : ";
//	exec("SELECT CreateMbrCache('changeset','mbr')");
// cerr << "Ok" << endl;

// Nodes
cerr << endl << "Indexes de Node : ";
//	exec("SELECT CreateSpatialIndex('node','coord');
	exec("CREATE INDEX IF NOT EXISTS node_tags_id_tag ON node_tags (id_tag)");
cerr << "Ok" << endl;

// Ways
cerr << endl << "Indexes de Way : ";
	exec("CREATE INDEX IF NOT EXISTS way_nodes_id_node ON way_nodes (id_node); \
		  CREATE INDEX IF NOT EXISTS way_tags_id_tag ON way_tags (id_tag)");
cerr << "Ok" << endl;

// Relations
cerr << endl << "Indexes de Relation : ";
    exec("CREATE INDEX IF NOT EXISTS relation_members_type_id_member        \
            ON relation_members (type, id_member);                          \
          CREATE INDEX IF NOT EXISTS relation_tags_id_tag                   \
            ON relation_tags (id_tag)");
cerr << "Ok" << endl;

	exec("COMMIT");

cerr << endl << "Analyse : ";
	exec("ANALYZE");
cerr << "Ok" << endl;
}

void BaseOsm::bindElement(Commande& aCommande, const Element& aElement)
{
    insertUser(aElement);
    check(sqlite3_bind_int64(aCommande, 1, aElement.id()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_bind_int64(aCommande, 2, aElement.version()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_bind_int64(aCommande, 3, aElement.changeset()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_bind_int64(aCommande, 4, aElement.uid()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_bind_int(aCommande, 5, (aElement.visible() ? 1 : 0)),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    const string timestamp = aElement.timestamp().iso8601();
    check(sqlite3_bind_text(aCommande, 6, timestamp.c_str(), timestamp.size(), SQLITE_STATIC),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseOsm::insertUser(const Top& aTop)
{
    check(sqlite3_bind_int64(fInsertUser, 1, aTop.uid()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    const string& nom = aTop.user();
    if (nom.empty()) {
        check(sqlite3_bind_null(fInsertUser, 2),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    } else {
        check(sqlite3_bind_text(fInsertUser, 2, nom.c_str(), nom.size(), SQLITE_STATIC),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    const int err = sqlite3_step(fInsertUser);
    if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_reset(fInsertUser),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

unsigned long BaseOsm::getIdTag(const string& aKey, const string& aValue)
{
//    assert(!aKey.empty());
//    assert(!aValue.empty());
    check(sqlite3_bind_text(fSelectTag, 1, aKey.c_str(), aKey.size(), SQLITE_STATIC),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_bind_text(fSelectTag, 2, aValue.c_str(), aValue.size(), SQLITE_STATIC),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    unsigned long idTag = 0;
    const int err = sqlite3_step(fSelectTag);
    switch (err) {
        case SQLITE_ROW :   // Connu
            idTag = sqlite3_column_int64(fSelectTag, 0);
            break;
        case SQLITE_DONE : {    // Inconnu
            check(sqlite3_bind_text(fInsertTag, 1, aKey.c_str(), aKey.size(), SQLITE_STATIC),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_bind_text(fInsertTag, 2, aValue.c_str(), aValue.size(), SQLITE_STATIC),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertTag);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            idTag = sqlite3_last_insert_rowid(fpSqlite3);
            check(sqlite3_reset(fInsertTag),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            break;
        }
        default :
            check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    check(sqlite3_reset(fSelectTag), __FILE__, __LINE__, __PRETTY_FUNCTION__);
    return idTag;
}

void BaseOsm::insertChangeset(const Changeset& aChangeset)
{
    check(sqlite3_bind_int64(fInsertChangeset, 1, aChangeset.id()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    insertUser(aChangeset);
    check(sqlite3_bind_int64(fInsertChangeset, 3, aChangeset.uid()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    const string createdAt = aChangeset.createdAt().iso8601();
    check(sqlite3_bind_text(fInsertChangeset, 4, createdAt.c_str(), createdAt.size(), SQLITE_STATIC),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    if (aChangeset.open()) {
        check(sqlite3_bind_null(fInsertChangeset, 5),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        check(sqlite3_bind_int(fInsertChangeset, 6, 1),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    } else {
        const string closedAt = aChangeset.closedAt().iso8601();
        check(sqlite3_bind_text(fInsertChangeset, 5, closedAt.c_str(), closedAt.size(), SQLITE_STATIC),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        check(sqlite3_bind_int(fInsertChangeset, 6, 0),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    unsigned char *pBlob = 0;
    int blobSize = 0;
    gaiaBuildMbr(aChangeset.minLat(), aChangeset.minLon(), aChangeset.maxLat(), aChangeset.maxLon(), 4326, &pBlob, &blobSize);
    assert(pBlob);
    assert(blobSize);
    check(sqlite3_bind_blob(fInsertChangeset, 7, pBlob, blobSize, free), __FILE__, __LINE__, __PRETTY_FUNCTION__);

    const int err = sqlite3_step(fInsertChangeset);
    if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_reset(fInsertChangeset), __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseOsm::insertChangesetTags(const Top& aTop,
                                  const Top::ListeTags& aTags)
{
    sqlite3_bind_int64(fInsertChangesetTags, 1, aTop.id());
    for (Top::ListeTags::const_iterator it = aTags.begin(); it != aTags.end(); ++it) {
        sqlite3_bind_int64(fInsertChangesetTags, 2, getIdTag(it->first, it->second));
        sqlite3_step(fInsertChangesetTags);
        sqlite3_reset(fInsertChangesetTags);
    }
}

void BaseOsm::add(const Changeset& aChangeset)
{
    insertChangeset(aChangeset);
    insertChangesetTags(aChangeset, aChangeset.tags());
    ++fNbChangesets;
}

void BaseOsm::insertNode(const Node& aNode)
{
    bindElement(fInsertNode, aNode);
    unsigned char *pBlob = 0;
    int blobSize = 0;
 // X == longitude, Y == latitude
    gaiaMakePoint(aNode.longitude(), aNode.latitude(), 4326, &pBlob, &blobSize);
    assert(pBlob);
    assert(blobSize);
    check(sqlite3_bind_blob(fInsertNode, 7, pBlob, blobSize, free),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);

    const int err = sqlite3_step(fInsertNode);
    if (err != SQLITE_DONE) {
       cerr << err << " " << sqlite3_errmsg(fpSqlite3) << endl;
       check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    check(sqlite3_reset(fInsertNode), __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseOsm::insertNodeTags(const Top& aTop,
                             const Top::ListeTags& aTags)
{
    if (!aTags.empty()) {
        check(sqlite3_bind_int64(fInsertNodeTags, 1, aTop.id()),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        for (Node::ListeTags::const_iterator it = aTags.begin(); it != aTags.end(); ++it) {
            check(sqlite3_bind_int64(fInsertNodeTags, 2,
                                     getIdTag(it->first, it->second)),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertNodeTags);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_reset(fInsertNodeTags),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::add(const Node& aNode)
{
    insertNode(aNode);
    insertNodeTags(aNode, aNode.tags());
    ++fNbNodes;
}

void BaseOsm::insertWay(const Way& aWay)
{
    bindElement(fInsertWay, aWay);
    const int err = sqlite3_step(fInsertWay);
    if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_reset(fInsertWay), __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseOsm::insertWayTags(const Top& aTop,
                            const Top::ListeTags& aTags)
{
    if (!aTags.empty()) {
        check(sqlite3_bind_int64(fInsertWayTags, 1, aTop.id()),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        for (Top::ListeTags::const_iterator it = aTags.begin(); it != aTags.end(); ++it) {
            check(sqlite3_bind_int64(fInsertWayTags, 2,
                                     getIdTag(it->first, it->second)),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertWayTags);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_reset(fInsertWayTags),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::insertWayNodes(const Top& aTop,
                             const Way::ListeNodes& aNodes)
{
    if (!aNodes.empty()) {
        check(sqlite3_bind_int64(fInsertWayNodes, 1, aTop.id()),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        unsigned rang = 0;  // commence à 1 car pré-incrémenté.
        for (Way::ListeNodes::const_iterator it = aNodes.begin(); it != aNodes.end(); ++it) {
            check(sqlite3_bind_int64(fInsertWayNodes, 2, *it),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_bind_int(fInsertWayNodes, 3, ++rang),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertWayNodes);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_reset(fInsertWayNodes),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::add(const Way& aWay)
{
    insertWay(aWay);
    insertWayTags(aWay, aWay.tags());
    insertWayNodes(aWay, aWay.nodes());
    ++fNbWays;
}

void BaseOsm::insertRelation(const Relation& aRelation)
{
    bindElement(fInsertRelation, aRelation);
    const int err = sqlite3_step(fInsertRelation);
    if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    check(sqlite3_reset(fInsertRelation),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void BaseOsm::insertRelationTags(const Top& aTop,
                                 const Top::ListeTags& aTags)
{
    if (!aTags.empty()) {
        check(sqlite3_bind_int64(fInsertRelationTags, 1, aTop.id()),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        for (Top::ListeTags::const_iterator it = aTags.begin(); it != aTags.end(); ++it) {
            check(sqlite3_bind_int64(fInsertRelationTags, 2,
                                     getIdTag(it->first, it->second)),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertRelationTags);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_reset(fInsertRelationTags),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::insertRelationMembers(const Top& aTop,
                                    const list<Member>& aMembers)
{
    if (!aMembers.empty()) {
        check(sqlite3_bind_int64(fInsertRelationMembers, 1, aTop.id()),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        unsigned rang = 0;  // commence à 1 car pré-incrémenté.
        for (list<Member>::const_iterator it = aMembers.begin(); it != aMembers.end(); ++it) {
            check(sqlite3_bind_int(fInsertRelationMembers, 2, it->getType()),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_bind_int64(fInsertRelationMembers, 3, it->getId()),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            if (it->getRole().empty()) {
                check(sqlite3_bind_null(fInsertRelationMembers, 4),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            } else {
                const string s = it->getRole();
                check(sqlite3_bind_text(fInsertRelationMembers, 4, s.c_str(), s.size(), SQLITE_STATIC),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            check(sqlite3_bind_int(fInsertRelationMembers, 5, ++rang),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            const int err = sqlite3_step(fInsertRelationMembers);
            if (err != SQLITE_DONE) check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            check(sqlite3_reset(fInsertRelationMembers),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::add(const Relation& aRelation)
{
    insertRelation(aRelation);
    insertRelationTags(aRelation, aRelation.tags());
    insertRelationMembers(aRelation, aRelation.getMembers());
    ++fNbRelations;
}
