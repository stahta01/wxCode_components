#ifndef __SQLITE_DATABASE_LAYER_H__
#define __SQLITE_DATABASE_LAYER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/arrstr.h"

#include "DatabaseLayer.h"

#include "sqlite3.h"


class PreparedStatement;

class SqliteDatabaseLayer : public DatabaseLayer
{
public:
  // ctor()
  SqliteDatabaseLayer();
  SqliteDatabaseLayer(const wxString& strDatabase);
  SqliteDatabaseLayer(sqlite3* pDatabase) { m_pDatabase = pDatabase; }
  
  // dtor()
  virtual ~SqliteDatabaseLayer();
  
  // open database
  virtual bool Open(const wxString& strDatabase);
  
  // close database  
  virtual bool Close();
  
  // Is the connection to the database open?
  virtual bool IsOpen();

  // transaction support
  virtual void BeginTransaction();
  virtual void Commit();
  virtual void RollBack();
  
  // query database
  virtual bool RunQuery(const wxString& strQuery, bool bParseQuery);
  virtual DatabaseResultSet* RunQueryWithResults(const wxString& strQuery);
  
  // PreparedStatement support
  virtual PreparedStatement* PrepareStatement(const wxString& strQuery);
  PreparedStatement* PrepareStatement(const wxString& strQuery, bool bLogForCleanup);
  
  // Database schema API contributed by M. Szeftel (author of wxActiveRecordGenerator)
  virtual bool TableExists(const wxString& table);
  virtual bool ViewExists(const wxString& view);
  virtual wxArrayString GetTables();
  virtual wxArrayString GetViews();
  virtual wxArrayString GetColumns(const wxString& table);

  static int TranslateErrorCode(int nCode);

private:
  
  sqlite3* m_pDatabase;
};

#endif // __SQLITE_DATABASE_LAYER_H__

