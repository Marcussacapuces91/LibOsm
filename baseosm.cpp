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
#include <spatialite/gaiageo.h>
#include "osm/way.h"
#include "osm/relation.h"
#include <cstdlib>
#include "basesqlite3.h"


using namespace std;

BaseOsm::BaseOsm(const string& aPath,
                 const bool aInitSpatialite,
                 const int aFlags) :
	BaseOsmCreateTables(aPath, aInitSpatialite, aFlags),
    fInsertUser(this->fpSqlite3, "INSERT OR IGNORE INTO user (id, name) VALUES (?,?)"),
	fSelectTag(this->fpSqlite3, "SELECT id FROM tag WHERE (key=? AND value=?)"),
    fInsertTag(this->fpSqlite3, "INSERT INTO tag (key, value) VALUES (?,?)"),
    fInsertChangeset(this->fpSqlite3, "INSERT INTO changeset (id, user, uid, created_at, num_changes, closed_at, open, mbr) VALUES (?,?,?,?,?,?,?,?)"),
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
	exec("PRAGMA locking_mode = EXCLUSIVE");

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
// cerr << aKey << "=" << aValue << "(" << fSelectTag << ")" << endl;
    if (aKey.empty()) {
        check(sqlite3_bind_null(fSelectTag, 1),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    } else {
        check(sqlite3_bind_text(fSelectTag, 1, aKey.c_str(), aKey.size(), SQLITE_STATIC),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    if (aValue.empty()) {
        check(sqlite3_bind_null(fSelectTag, 2),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    } else {
        check(sqlite3_bind_text(fSelectTag, 2, aValue.c_str(), aValue.size(), SQLITE_STATIC),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    const int err = sqlite3_step(fSelectTag);
    switch (err) {
        case SQLITE_ROW : {     // Connu
            const unsigned long idTag = sqlite3_column_int64(fSelectTag, 0);
            check(sqlite3_reset(fSelectTag),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            return idTag;
            break;
        }
        case SQLITE_DONE : {    // Inconnu
            check(sqlite3_reset(fSelectTag),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            if (aKey.empty()) {
                check(sqlite3_bind_null(fInsertTag, 1),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            } else {
                check(sqlite3_bind_text(fInsertTag, 1, aKey.c_str(), aKey.size(), SQLITE_STATIC),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            if (aValue.empty()) {
                check(sqlite3_bind_null(fInsertTag, 2),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            } else {
                check(sqlite3_bind_text(fInsertTag, 2, aValue.c_str(), aValue.size(), SQLITE_STATIC),
                      __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            const int err = sqlite3_step(fInsertTag);
            if (err != SQLITE_DONE) {
                sqlite3_reset(fInsertTag);
                check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            const unsigned long idTag = sqlite3_last_insert_rowid(fpSqlite3);
            check(sqlite3_reset(fInsertTag),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
            return idTag;
            break;
        }
        default :
            sqlite3_reset(fSelectTag);
            check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            assert(0);  // arrivée ici impossible !?
    }
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
    check(sqlite3_bind_int(fInsertChangeset, 5, aChangeset.numChanges()),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    if (aChangeset.open()) {
        check(sqlite3_bind_null(fInsertChangeset, 6),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        check(sqlite3_bind_int(fInsertChangeset, 7, 1),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    } else {
        const string closedAt = aChangeset.closedAt().iso8601();
        check(sqlite3_bind_text(fInsertChangeset, 6, closedAt.c_str(), closedAt.size(), SQLITE_STATIC),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
        check(sqlite3_bind_int(fInsertChangeset, 7, 0),
              __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    unsigned char *pBlob = 0;
    int blobSize = 0;
    gaiaBuildMbr(aChangeset.minLat(), aChangeset.minLon(), aChangeset.maxLat(), aChangeset.maxLon(), 4326, &pBlob, &blobSize);
    assert(pBlob);
    assert(blobSize);
    check(sqlite3_bind_blob(fInsertChangeset, 8, pBlob, blobSize, free), __FILE__, __LINE__, __PRETTY_FUNCTION__);

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
        sqlite3_reset(fInsertNode);
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
            if (err != SQLITE_DONE) {
                sqlite3_reset(fInsertNodeTags);
                check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            check(sqlite3_reset(fInsertNodeTags),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::add(const Node& aNode)
{
    insertNode(aNode);
    insertNodeTags(aNode);
    ++fNbNodes;
}

void BaseOsm::insertWay(const Way& aWay)
{
    bindElement(fInsertWay, aWay);
    const int err = sqlite3_step(fInsertWay);
    if (err != SQLITE_DONE) {
        sqlite3_reset(fInsertWay);
        check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
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
            if (err != SQLITE_DONE) {
                sqlite3_reset(fInsertWayTags);
                check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
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
            if (err != SQLITE_DONE) {
                sqlite3_reset(fInsertWayNodes);
                check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            check(sqlite3_reset(fInsertWayNodes),
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);
        }
    }
}

void BaseOsm::add(const Way& aWay)
{
    insertWay(aWay);
    insertWayTags(aWay);
    insertWayNodes(aWay);
    ++fNbWays;
}

void BaseOsm::insertRelation(const Relation& aRelation)
{
    bindElement(fInsertRelation, aRelation);
    const int err = sqlite3_step(fInsertRelation);
    if (err != SQLITE_DONE) {
        sqlite3_reset(fInsertRelation);
        check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
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
            if (err != SQLITE_DONE) {
                sqlite3_reset(fInsertRelationTags);
                check(err, __FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
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
    insertRelationTags(aRelation);
    insertRelationMembers(aRelation);
    ++fNbRelations;
}

void BaseOsm::modify(const Node& aNode)
{
    static Commande n(fpSqlite3,
                      "UPDATE node SET version=?2,changeset=?3,uid=?4,visible=?5,timestamp=?6,coord=?7 WHERE id=?1");
    bindElement(n, aNode);
    unsigned char *pBlob = 0;
    int blobSize = 0;
 // X == longitude, Y == latitude
    gaiaMakePoint(aNode.longitude(), aNode.latitude(), 4326, &pBlob, &blobSize);
    assert(pBlob);
    assert(blobSize);
    check(sqlite3_bind_blob(n, 7, pBlob, blobSize, free),
          __FILE__, __LINE__, __PRETTY_FUNCTION__);
    sqlite3_step(n);
    sqlite3_reset(n);

    static Commande nt(fpSqlite3, "DELETE FROM node_tags WHERE id_node=?");
    sqlite3_bind_int64(nt, 1, aNode.id());
    sqlite3_step(nt);
    sqlite3_reset(nt);
    insertNodeTags(aNode);
//    cerr << aNode;
}

void BaseOsm::modify(const Way& aWay)
{
    static Commande w(fpSqlite3,
                      "UPDATE way SET version=?2,changeset=?3,uid=?4,visible=?5,timestamp=?6 WHERE id=?1");
    bindElement(w, aWay);
    sqlite3_step(w);
    sqlite3_reset(w);

    static Commande wt(fpSqlite3, "DELETE FROM way_tags WHERE id_way=?");
    sqlite3_bind_int64(wt, 1, aWay.id());
    sqlite3_step(wt);
    sqlite3_reset(wt);
    insertWayTags(aWay);

    static Commande wn(fpSqlite3, "DELETE FROM way_nodes WHERE id_way=?");
    sqlite3_bind_int64(wn, 1, aWay.id());
    sqlite3_step(wn);
    sqlite3_reset(wn);
    insertWayNodes(aWay, aWay.nodes());
}

void BaseOsm::modify(const Relation& aRelation)
{
    static Commande r(fpSqlite3,
                      "UPDATE relation SET version=?2,changeset=?3,uid=?4,visible=?5,timestamp=?6 WHERE id=?1");
    bindElement(r, aRelation);
    sqlite3_step(r);
    sqlite3_reset(r);

    static Commande rt(fpSqlite3, "DELETE FROM relation_tags WHERE id_relation=?");
    sqlite3_bind_int64(rt, 1, aRelation.id());
    sqlite3_step(rt);
    sqlite3_reset(rt);
    insertRelationTags(aRelation);

    static Commande rm(fpSqlite3, "DELETE FROM relation_members WHERE id_relation=?");
    sqlite3_bind_int64(rm, 1, aRelation.id());
    sqlite3_step(rm);
    sqlite3_reset(rm);
    insertRelationMembers(aRelation, aRelation.getMembers());
}

void BaseOsm::suppress(const Node& aNode)
{
    static Commande nt(fpSqlite3, "DELETE FROM node_tags WHERE id_node=?");
    sqlite3_bind_int64(nt, 1, aNode.id());
    sqlite3_step(nt);
    sqlite3_reset(nt);

    static Commande n(fpSqlite3, "DELETE FROM node WHERE id=? LIMIT 1");
    sqlite3_bind_int64(n, 1, aNode.id());
    sqlite3_step(n);
    sqlite3_reset(n);

    --fNbNodes;
}

void BaseOsm::suppress(const Way& aWay)
{
    static Commande wt(fpSqlite3, "DELETE FROM way_tags WHERE id_way=?");
    sqlite3_bind_int64(wt, 1, aWay.id());
    sqlite3_step(wt);
    sqlite3_reset(wt);

    static Commande wn(fpSqlite3, "DELETE FROM way_nodes WHERE id_way=?");
    sqlite3_bind_int64(wn, 1, aWay.id());
    sqlite3_step(wn);
    sqlite3_reset(wn);

    static Commande w(fpSqlite3, "DELETE FROM way WHERE id=? LIMIT 1");
    sqlite3_bind_int64(w, 1, aWay.id());
    sqlite3_step(w);
    sqlite3_reset(w);

    --fNbWays;
}

void BaseOsm::suppress(const Relation& aRelation)
{
    static Commande rm(fpSqlite3, "DELETE FROM relation_members WHERE id_relation=?");
    sqlite3_bind_int64(rm, 1, aRelation.id());
    sqlite3_step(rm);
    sqlite3_reset(rm);

    static Commande rt(fpSqlite3, "DELETE FROM relation_tags WHERE id_relation=?");
    sqlite3_bind_int64(rt, 1, aRelation.id());
    sqlite3_step(rt);
    sqlite3_reset(rt);

    static Commande r(fpSqlite3, "DELETE FROM relation WHERE id=? LIMIT 1");
    sqlite3_bind_int64(r, 1, aRelation.id());
    sqlite3_step(r);
    sqlite3_reset(r);

    --fNbRelations;
}
