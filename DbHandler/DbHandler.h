#ifndef DBHANDLER_DBHANDLER_H_
#define DBHANDLER_DBHANDLER_H_

#include <sqlite3.h>
#include <stdio.h>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

class DbHandler {
 public:
  DbHandler();
  ~DbHandler();

  static int ReadMetadata(int);
  static std::string getTrackPath(int);
  static int getCount();
};

#endif /* DBHANDLER_DBHANDLER_H_ */
