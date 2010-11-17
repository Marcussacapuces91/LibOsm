#include "basesqlite3threaded.h"

#include "async/sqlite3async.h"

BaseSQLite3Threaded::BaseSQLite3Threaded(const string& aNom,
          const int aFlags) :
     BaseSQLite3(aNom, aFlags),
     fThread()
{
     if (sqlite3async_initialize(0, 1) != SQLITE_OK)
          throw(Exception("Erreur sur sqlite3async_initialize",
                          __FILE__, __LINE__, __PRETTY_FUNCTION__));

// Initialize and set thread detached attribute
     pthread_attr_t attr;
     pthread_attr_init(&attr);
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

     if (pthread_create(&fThread, &attr, Thread, static_cast<void *>(this)))
          throw(Exception("Erreur de création du thread",
                          __FILE__, __LINE__, __PRETTY_FUNCTION__));
// Destruction de l'attribut.
     pthread_attr_destroy(&attr);
}

BaseSQLite3Threaded::~BaseSQLite3Threaded()
{
// Demande l'arrêt de sqlite3async_run() dès que la queue est vide.
     sqlite3async_control(SQLITEASYNC_HALT, SQLITEASYNC_HALT_IDLE);

// Attente de l'arrêt naturel du thread.
     void* status;
     if (pthread_join(fThread, &status))
          throw(Exception("Erreur sur pthread_join",
                          __FILE__, __LINE__, __PRETTY_FUNCTION__));

     sqlite3async_shutdown();
}

void* BaseSQLite3Threaded::Thread(void* /* apParam */)
{
     sqlite3async_run();
//     pthread_exit(0);
     return 0;
}
