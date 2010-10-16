#ifndef BASESQLITE3THREADED_H
#define BASESQLITE3THREADED_H

#include "basesqlite3.h"

#include <pthread.h>

class BaseSQLite3Threaded : public BaseSQLite3 {
public:
     /**
      * \brief Constructeur de classe.
      *
      * Initialise l'accès à la base de données.
      * \param aNom Chemin du fichier contenant la base de données.
      * \param aFlags Indicateurs SQLite précisant le mode d'ouverture du
      *               fichier. Par défaut,
      *               c'est en SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE.
      */
     BaseSQLite3Threaded(const string& aNom,
                         const int aFlags = SQLITE_OPEN_READWRITE |
                                            SQLITE_OPEN_CREATE);

     /** Default destructor */
     virtual ~BaseSQLite3Threaded();

protected:
private:
     /** Copy constructor
      *  \param other Object to copy from
      */
     BaseSQLite3Threaded(const BaseSQLite3Threaded& other);

     /** Assignment operator
      *  \param other Object to assign from
      *  \return A reference to this
      */
     BaseSQLite3Threaded& operator=(const BaseSQLite3Threaded& other);

///
     pthread_t fThread;

     /**
      * Méthode statique exécutée par l'autre thread.
      * \param apParam un pointeur sur l'instance de BaseSqlite3Thread qui a
      *                créé le thread.
      * \return
      */
     static void* Thread(void* apParam);
};

#endif // BASESQLITE3THREADED_H
