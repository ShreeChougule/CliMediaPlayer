/*
 * printprogress.h
 *
 *  Created on: 09-Apr-2019
 *      Author: kpit
 */

#ifndef DBUS_INDEXER_PRINTPROGRESS_H_
#define DBUS_INDEXER_PRINTPROGRESS_H_

#include <stdio.h>
#include <cstdlib>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

static void printProgress(int curr_cnt, int cnt) {
  double percentage = (curr_cnt / (float)cnt);
  int val = (int)(percentage * 100);
  int lpad = (int)(percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
}

#endif /* DBUS_INDEXER_PRINTPROGRESS_H_ */
