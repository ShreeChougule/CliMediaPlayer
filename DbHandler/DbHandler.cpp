
#include "DbHandler.h"
// sqlite3_stmt *DbHandler::m_handle = NULL;
// sqlite3 *DbHandler::m_stmt = NULL;

DbHandler::DbHandler() {}

DbHandler::~DbHandler() {}

int DbHandler::ReadMetadata(int trackID) {
  sqlite3_stmt *m_stmt;
  sqlite3 *m_handle;
  int total_row;
  std::string val;
  std::string name;
  char query[100];
  sprintf(query, "SELECT * FROM songMaster WHERE id=%d;", trackID);

  int rc = sqlite3_open("../build/mediadb.db", &m_handle);

  sqlite3_prepare(m_handle, query, -1, &m_stmt, 0);
  sqlite3_step(m_stmt);
  int cols = sqlite3_column_count(m_stmt);

  for (int col = 0; col < cols; col++) {
    val = (char *)sqlite3_column_text(m_stmt, col);
    name = (const char *)sqlite3_column_name(m_stmt, col);
    if (name == "song_title") printf("\n\t Title : %s\n", val.c_str());
    if (name == "album_name") printf("\n\t Album : %s\n", val.c_str());
    if (name == "artist_name") printf("\n\t Artist : %s\n", val.c_str());
    if (name == "genre_name") printf("\n\t Genre : %s\n", val.c_str());
  }

  sqlite3_finalize(m_stmt);
  sqlite3_close(m_handle);

  return 1;
}

std::string DbHandler::getTrackPath(int trackID) {
  sqlite3_stmt *m_stmt;
  sqlite3 *m_handle;
  std::string val;
  std::string name;
  char query[100];
  std::string filePath = "";
  sprintf(query, "SELECT * FROM songMaster WHERE id=%d;", trackID);

  int rc = sqlite3_open("../build/mediadb.db", &m_handle);

  sqlite3_prepare(m_handle, query, -1, &m_stmt, 0);
  sqlite3_step(m_stmt);
  int cols = sqlite3_column_count(m_stmt);

  for (int col = 0; col < cols; col++) {
    val = (char *)sqlite3_column_text(m_stmt, col);
    name = (const char *)sqlite3_column_name(m_stmt, col);
    if (name == "song_path") filePath = val;
  }

  sqlite3_finalize(m_stmt);
  sqlite3_close(m_handle);

  return filePath;
}

int DbHandler::getCount() {
  sqlite3_stmt *m_stmt;
  sqlite3 *m_handle;
  std::string val;
  char query[100];
  sprintf(query, "SELECT COUNT(*) FROM songMaster;");

  int rc = sqlite3_open("../build/mediadb.db", &m_handle);

  sqlite3_prepare(m_handle, query, -1, &m_stmt, 0);
  sqlite3_step(m_stmt);
  val = (char *)sqlite3_column_text(m_stmt, 0);

  sqlite3_finalize(m_stmt);
  sqlite3_close(m_handle);

  return (stoi(val));
}
