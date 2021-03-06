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

#ifndef BASEOSM_H
#define BASEOSM_H

#include "baseosmcreatetables.h"     // inheriting class's header file
#include "xml/baseinterface.h"       // inheriting class's header file

#include "osm/element.h"
#include "osm/changeset.h"
#include "osm/node.h"
#include "osm/way.h"
#include "osm/relation.h"
#include <set>

using namespace std;

/**
 * Classe permettant d'instancier une base de données contenant les différents
 * éléments d'OSM.
 * Elle hérite de BaseSQLite3 pour ses primitives, et de l'interface
 * BaseInterface pour être utilisable par le parseur XML qui va y insérer les
 * éléments.
 */
class BaseOsm : public BaseOsmCreateTables, public BaseInterface {
private:
     /**
      * Constructeur de copie rendu non visible.
      */
     BaseOsm(const BaseOsm&);

     /**
      * Opérateur de recopie rendu non visible.
      */
     BaseOsm& operator=(const BaseOsm&);

/// Ensemble des UID connus (pour éviter les tentatives d'insertion).
     set<unsigned long> fUsers;

/// Commande précompilée pour lire un user dans la table user.
     Commande fSelectUser;

/// Commande précompilée pour ajouter un enregistrement dans la table user.
     Commande fInsertUser;

/// Commande précompilée pour lire un enregistrement depuis la table tag.
     Commande fSelectTag;

/// Commande précompilée pour ajouter un enregistrement dans la table tag.
     Commande fInsertTag;

/// Commande précompilée pour ajouter un enregistrement dans la table changeset.
     Commande fInsertChangeset;

/// Commande précompilée pour ajouter un enregistrement dans la table
/// changeset_tags.
     Commande fInsertChangesetTags;

/// Commande précompilée pour ajouter un enregistrement dans la table node.
     Commande fInsertNode;

/// Commande précompilée pour ajouter un enregistrement dans la table node_tags.
     Commande fInsertNodeTags;

/// Commande précompilée pour ajouter un enregistrement dans la table way.
     Commande fInsertWay;

/// Commande précompilée pour ajouter un enregistrement dans la table way_tags.
     Commande fInsertWayTags;

/// Commande précompilée pour ajouter un enregistrement dans la table way_nodes.
     Commande fInsertWayNodes;

/// Commande précompilée pour ajouter un enregistrement dans la table relation.
     Commande fInsertRelation;

/// Commande précompilée pour ajouter un enregistrement dans la table
/// relation_tags.
     Commande fInsertRelationTags;

/// Commande précompilée pour ajouter un enregistrement dans la table
/// relation_members.
     Commande fInsertRelationMembers;

/// Nombre de Changeset.
     unsigned fNbChangesets;

/// Nombre de Node.
     unsigned fNbNodes;

/// Nombre de Way.
     unsigned fNbWays;

/// Nombre de Changeset.
     unsigned fNbRelations;

     /*
      * Méthode statique qui est appelée régulièrement par le moteur SQLite3 afin
      * d'afficher la progression de l'application.
      * Pour le moment cette méthode est sans action.
      * \param apBaseOsm Un pointeur sur l'instance courante de BaseOsm.
      * \return Une valeur <> 0 provoque l'arrêt du moteur SQLite.
       /
     //        static int progress(void *const apBaseOsm);
     */

     /**
      * Retourne l'identifiant d'une paire Key, Value et crée cette paire si elle
      * n'existe pas.
      * \param aKey Une chaîne contenant la clé du Tag
      * \param aValue Une chaîne contenant la valeur du Tag.
      * \return Un entier unique pour la paire clé / valeur.
      */
     unsigned long getIdTag(const string& aKey,
                            const string& aValue);


     /**
      * Ajoute un enregistrement dans la table user ou ignore l'ordre si
      * l'identifiant existe déjà.
      * \param aTop Une référence sur la classe Top, abstraite ultime, pouvant
      *             fournir l'uid et le nom de l'utilisateur ayant modifié l'élément.
      */
     void insertUser(const Top& aTop);

     /**
      * Assure la définition des 6 premiers champs d'une Commande pré-compilée à
      * partir des champs d'un Element :
      * - id,
      * - version,
      * - changeset,
      * - uid,
      * - visible,
      * - timestamp.
      * \param aCommande Une référence sur une Commande pré-compilée.
      * \param aElement Une référence sur l'Element dont les attributs seront liés
      */
     void bindElement(Commande& aCommande,
                      const Element& aElement);

protected:
     /**
      * Ajoute un enregistrement dans la table Changeset.
      * \param aChangeset Une référence sur un Changeset.
      */
     void insertChangeset(const Changeset& aChangeset);

     /**
      * Ajoute les enregistrements nécessaires dans la table ChangesetTags.
      * \param aTop Une référence sur un Changeset.
      * \param aTags Une référence sur une liste de Tag associés.
      */
     void insertChangesetTags(const Top& aTop,
                              const Top::ListeTags& aTags);

     /**
      * Ajoute un enregistrement dans la table Node.
      * \param aNode Une référence sur un Node.
      */
     void insertNode(const Node& aNode);

     /**
      * Ajoute les enregistrements nécessaires dans la table NodeTags.
      * \param aTop Une référence sur un Node.
      */
     void insertNodeTags(const Top& aTop) {
          insertNodeTags(aTop, aTop.tags());
     }

     /**
      * Ajoute les enregistrements nécessaires dans la table NodeTags.
      * \param aTop Une référence sur un Node.
      * \param aTags Une référence sur une liste de Tag associés.
      */
     void insertNodeTags(const Top& aTop,
                         const Top::ListeTags& aTags);

     /**
      * Ajoute un enregistrement dans la table Way.
      * \param aWay Une référence sur un Way.
      */
     void insertWay(const Way& aWay);

     /**
      * Ajoute les enregistrements nécessaires dans la table WayTags.
      * \param aTop Une référence sur un Way.
      */
     void insertWayTags(const Top& aTop) {
          insertWayTags(aTop, aTop.tags());
     }

     /**
      * Ajoute les enregistrements nécessaires dans la table WayTags.
      * \param aTop Une référence sur un Way.
      * \param aTags Une référence sur une liste de Tag associés.
      */
     void insertWayTags(const Top& aTop,
                        const Top::ListeTags& aTags);

     /**
      * Ajoute les enregistrements nécessaires dans la table WayNodes.
      * \param aTop Une référence sur un Way.
      */
     void insertWayNodes(const Way& aWay) {
          insertWayNodes(aWay, aWay.nodes());
     }

     /**
      * Ajoute les enregistrements nécessaires dans la table WayNodes.
      * \param aTop Une référence sur un Way.
      * \param aNodes Une référence sur une liste de Node associés.
      */
     void insertWayNodes(const Top& aTop,
                         const Way::ListeNodes& aNodes);

     /**
      * Ajoute un enregistrement dans la table Relation.
      * \param aRelation Une référence sur une Relation.
      */
     void insertRelation(const Relation& aRelation);

     /**
      * Ajoute les enregistrements nécessaires dans la table RelationTags.
      * \param aTop Une référence sur une Relation.
      */
     void insertRelationTags(const Top& aTop) {
          insertRelationTags(aTop, aTop.tags());
     };

     /**
      * Ajoute les enregistrements nécessaires dans la table RelationTags.
      * \param aTop Une référence sur une Relation.
      * \param aTags Une référence sur une liste de Tag associés.
      */
     void insertRelationTags(const Top& aTop,
                             const Top::ListeTags& aTags);

     /**
      * Ajoute les enregistrements nécessaires dans la table RelationMembers.
      * \param aTop Une référence sur une Relation.
      */
     void insertRelationMembers(const Relation& aRelation) {
          insertRelationMembers(aRelation, aRelation.getMembers());
     }

     /**
      * Ajoute les enregistrements nécessaires dans la table RelationMembers.
      * \param aTop Une référence sur une Relation.
      * \param aMembers Une référence sur une liste de Member associés.
      */
     void insertRelationMembers(const Top& aTop,
                                const list<Member>& aMembers);


public:
     /**
      * \brief Constructeur de l'instance.
      *
      * Initialise une connexion au fichier de la base SQLite3.
      * \param aPath Chemin vers le fichier.
      * \param aInitSpatialite Booléen indiquant si la base doit être initialisée
      *                        avec le script init_spatialite-2.3.sql.
      * \param aFlags Options d'ouverture du fichier. Par défaut en lecture/écriture
      *               et création en cas d'absence.
      */
     BaseOsm(const string& aPath,
             const bool aInitSpatialite = false,
             const int aFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

     /**
      * \brief Destructeur virtuel de l'instance.
      *
      * Sans action.
      */
     virtual ~BaseOsm(void) {};

     /**
      * Ajoute un Changeset dans la base.
      * \param aChangeset Une référence sur un Changeset qui sera ajouté dans la base.
      */
     void add(const Changeset& aChangeset);

     /**
      * Ajoute un Node dans la base.
      * \param aNode Une référence sur un Node qui sera ajouté dans la base.
      */
     void add(const Node& aNode);

     /**
      * Ajoute un Way dans la base.
      * \param aWay Une référence sur un Way qui sera ajouté dans la base.
      */
     void add(const Way& aWay);

     /**
      * Ajoute une Relation dans la base.
      * \param aRelation Une référence sur une Relation qui sera ajoutée dans la base.
      */
     void add(const Relation& aRelation);

     /**
      * Créé les indexes généralement nécessaires.
      */
     void createIndexes();

     void modify(const Node& aNode);
     void modify(const Way& aWay);
     void modify(const Relation& aRelation);

     void suppress(const Node& aNode);
     void suppress(const Way& aWay);
     void suppress(const Relation& aRelation);

     /**
      * Retourne le nombre de Changeset dans la base (estimation).
      * \return Une estimation du nombre de Changeset.
      */
     unsigned nbChangesets() const {
          return fNbChangesets;
     }

     /**
      * Retourne le nombre de Node dans la base (estimation).
      * \return Une estimation du nombre de Node.
      */
     unsigned nbNodes() const {
          return fNbNodes;
     }

     /**
      * Retourne le nombre de Way dans la base (estimation).
      * \return Une estimation du nombre de Way.
      */
     unsigned nbWays() const {
          return fNbWays;
     }

     /**
      * Retourne le nombre de Relation dans la base (estimation).
      * \return Une estimation du nombre de Relation.
      */
     unsigned nbRelations() const {
          return fNbRelations;
     }

};

#endif // BASEOSM_H
