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

#ifndef BASESQLITE3_H
#define BASESQLITE3_H

#include <spatialite/sqlite3.h>
#include <string>
#include <fstream>
#include "exception.h"
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

/**
 * Wrapper autour du sqlite3_stmt qui permet son instanciation aisée et sa
 * libération automatique après.
 */
class Commande {
private:
     /**
      * Crée un sqlite_stmt à partir d'une requête SQL.
      * \param apSqlite3 Un pointeur vers une connexion SQLite3.
      * \param aSql Une chaîne contenant la requête SQL.
      * \return Un pointeur sur le sqlite3_stmt.
      * \throw Exception en cas d'erreur pendant la création.
      */
     static sqlite3_stmt* preparer(sqlite3 *const apSqlite3,
                                   const string& aSql) {
          sqlite3_stmt* pRes = 0;
          if (SQLITE_OK != sqlite3_prepare_v2(apSqlite3, aSql.c_str(), aSql.size(), &pRes, 0))
               throw Exception(sqlite3_errmsg(apSqlite3) + (" dans '" + aSql + "'"),
                               __FILE__, __LINE__, __PRETTY_FUNCTION__);
          if (!pRes) throw Exception(string("Prepared statment NULL pour la requete ") + aSql,
                                          __FILE__, __LINE__, __PRETTY_FUNCTION__);
          return pRes;
     }

     /**
      * Opérateur d'affectation rendu inaccessible.
      */
     Commande& operator=(const Commande&);

/// Un pointeur sur la base concernée (nécessaire pour récupérer les messages
/// d'erreur).
     sqlite3 *const fpSqlite3;

/// Un pointeur sur une commande.
     sqlite3_stmt *const fpSqlite3_stmt;

public:

     /**
      * \brief Constructeur d'instance.
      *
      * Initialise et prépare la commande à partir de la requête SQL présentée.
      * \param apSqlite3 Un pointeur sur l'instance de base de données.
      * \param aSql Une chaîne contenant la requête SQL à préparer.
      * \throw Exception en cas d'erreur pendant la création.
      */
     Commande(sqlite3 *const apSqlite3,
              const string aSql) :
          //        fSql(aSql),
          fpSqlite3(apSqlite3),
          fpSqlite3_stmt(preparer(apSqlite3, aSql)) {
          assert(fpSqlite3);
          assert(fpSqlite3_stmt);
     }

     /**
      * \brief Constructeur de copie.
      *
      * Assure la construction d'une nouvelle Commande à partir de la requête SQL de
      * la première, ou crée une nouvelle Commande vide.
      * \param aCommande Instance à copier.
      */
     Commande(const Commande& aCommande) :
          fpSqlite3(aCommande.fpSqlite3),
          fpSqlite3_stmt(aCommande.fpSqlite3_stmt ?
                         preparer(fpSqlite3,
                                  sqlite3_sql(aCommande.fpSqlite3_stmt)) :
                         0) {
          assert(fpSqlite3);
          assert(fpSqlite3_stmt);
     }

     /**
      * \brief Destructeur de l'instance.
      *
      * Libère toutes les ressources liées à la commande.
      */
     ~Commande(void) {
// cerr << "~Commande de " << sqlite3_sql(fpSqlite3_stmt) << " (" << fpSqlite3_stmt << ")" << endl;
          assert(fpSqlite3);
          assert(fpSqlite3_stmt);
          if (sqlite3_finalize(fpSqlite3_stmt) != SQLITE_OK) {
// C'est le résultat d'une erreur précédente.
/// \see http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.3
//                cerr << sqlite3_errmsg(fpSqlite3) << " a " << __FILE__ << ":";
//                cerr << __LINE__ << " dans " << __PRETTY_FUNCTION__ << endl;
          }
     }

     /*
      * \brief Redéfinit la commande.
      *
      * Libère la commande existante et pré-calcule une nouvelle commande SQL.
      * \param apSqlite3 Un pointeur sur l'instance de base de données.
      * \param aSql Une chaîne contenant la requête SQL à préparer.
      * \throw Exception en cas d'erreur pendant la création.
      *
         void setStatment(sqlite3 *const apSqlite3,
                          const string& aSql)
         {
             assert(apSqlite3);
             assert(aSql.size());
             if (fpSqlite3_stmt) {
                 assert(fpSqlite3);
                 if (SQLITE_OK != sqlite3_finalize(fpSqlite3_stmt))
                    throw Exception(sqlite3_errmsg(fpSqlite3), __FILE__, __LINE__, __PRETTY_FUNCTION__);
             }
             fpSqlite3 = apSqlite3;
     //        fSql = aSql;
             fpSqlite3_stmt = preparer(apSqlite3, aSql);
         }
     */

     /**
      * Définit l'opérateur de cast pour qu'il retourne le sqlite3_stmt*
      * sous-jacent de l'instance courante.
      * \note : J'aime beaucoup cette trouvaille
      * \see http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B#Other_operators
      * \return Un pointeur sur le sqlite3_stmt.
      */
//    sqlite3_stmt* operator*() { assert(fpSqlite3_stmt); return fpSqlite3_stmt; }
     operator sqlite3_stmt*() {
          assert(fpSqlite3_stmt);
          return fpSqlite3_stmt;
     }

};


/**
 * Wrapper de la librairie SQLite3.
 */
class BaseSQLite3 {
private:
     /**
      * Opérateur d'affectation déclaré privé pour être rendu inutilisable.
      */
     BaseSQLite3& operator=(const BaseSQLite3&);

     /**
      * Constructeur de copie déclaré privé pour être rendu inutilisable.
      */
     BaseSQLite3(const BaseSQLite3&);

     /**
      * Méthode statique générant une connexion SQLite3.
      * \param aNom Une chaîne représentant un nom de fichier (non vide).
      * \param aFlags Indicateurs du mode d'ouverture de la base.
      * \return Un pointeur sur une connexion Sqlite3. Cette connexion devra être
      *         libérée par une commande sqlite3_close.
      * \throw Exception en cas d'erreur à l'ouverture de la base.
      */
     static sqlite3* open(const string& aNom,
                          const int aFlags);

protected:
/// Handler de la connexion à la base de données.
     sqlite3 *const fpSqlite3;

     /**
      * Vérifie que le code erreur transmis est SQLITE_OK. Si la valeur est
      * différente, lève une exception avec le message d'erreur correspondant.
      * \warning dans certains cas (SQLITE_ROW), la méthode appelante doit filtrer
      *          avant de faire l'appel pour éviter la génération de l'exception.
      * \throw Exception Lorsque le code erreur transmis est différent de SQLITE_OK.
      * \param aError Le code erreur à analyser.
      * \param aFichier Nom du fichier où s'est fait l'appel, par défaut __FILE__.
      * \param aLigne Numéro de la ligne où s'est fait l'appel, par défaut __LINE__.
      * \param aFonction Fonction ou méthode où s'est fait l'appel, par défaut
      *                  __PRETTY_FUNCTION__.
      */
     inline void check(const int aError,
                       const string& aFichier = __FILE__,
                       const unsigned aLigne = __LINE__,
                       const string& aFonction = __PRETTY_FUNCTION__) const {
          assert(fpSqlite3);
          switch (aError) {
          case SQLITE_MISUSE :
               throw Exception("Mauvaise utilisation",
                               aFichier, aLigne, aFonction);
          case SQLITE_OK :
          case SQLITE_ROW :
          case SQLITE_DONE :
               break;
          default :
               const char *const p = sqlite3_errmsg(fpSqlite3);
               throw Exception((p ? p : "Erreur inconnue"),
                               aFichier, aLigne, aFonction);
          }
     }

     /*
      * Construit et retourne une commande préparée.
      * \param aSql Une chaîne représentant la commande SQL.
      * \return Une instance de Commande, initialisée avec l'instance de la base et
      *         la commande SQL.
      *
             Commande makeCommande(const string& aSql)
             {
                 return Commande(this->fpSqlite3, aSql);
             }
     */

public:
     /**
      * \brief Constructeur de classe.
      *
      * Initialise l'accès à la base de données.
      * \param aNom Chemin du fichier contenant la base de données.
      * \param aFlags Indicateurs SQLite précisant le mode d'ouverture du fichier.
      *               Par défaut, c'est en SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE.
      */
     BaseSQLite3(const string& aNom,
                 const int aFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

     /**
      * Destructeur de classe.
      */
     virtual ~BaseSQLite3(void);

     /**
      * Exécute une requête SQL sans attendre de résultat.
      * \param aSql Une chaîne contenant la requête SQL.
      */
     void exec(const string& aSql);

     /**
      * Exécute la/les requête(s) SQL présentent dans le fichier indiqué.
      * \param aPath Le chemin du fichier à exécuter.
      */
     void execFile(const string& aPath);

     static int cbkReturnUnsigned(void* apResult,
                                  int aNbCols,
                                  char** apValues,
                                  char** apNames) {
          assert(apResult);
          assert(apValues);
          assert(apNames);
          if (aNbCols != 1) return -1; // erreur
          const string s = *apValues;
          istringstream iss(s);
          unsigned *const p = static_cast<unsigned*>(apResult);
          iss >> *p;
          return 0;
     }

     unsigned cacheSize() const {
          unsigned result = 0;
          check(sqlite3_exec(fpSqlite3, "PRAGMA cache_size",
                             BaseSQLite3::cbkReturnUnsigned, &result, 0),
                __FILE__, __LINE__, __PRETTY_FUNCTION__);
          return result;
     }

     void cacheSize(const unsigned aSize) {
          ostringstream oss;
          oss << "PRAGMA cache_size=" << aSize;
          exec(oss.str());
     }
};

#endif
