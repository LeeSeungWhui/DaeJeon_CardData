/*
 * RAW.h
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#ifndef SRC_RAW_RAW_H_
#define SRC_RAW_RAW_H_
#include "../IntWrapper/IntWrapper.h"
#include "../Bis/Bis.h"
#include "RawData.h"
#include <unordered_map>
#include <list>

class RAW {
public:
	RAW();
	RAW(sql::ResultSet* res);
	virtual ~RAW();
	void print(ostream* fout); // 출력함수
	void print_normalize(ostream* fout); // 표준화 출력함수
	void transitRevise(); // 환승횟수 보정
	void editraw(BIS* bis); // 정류장순서와 운행차수 추가

	list<INTWRAPPER*> rawdataIdx;
	static unordered_multimap<long, RAWDATA*> data;
};
#endif /* SRC_RAW_RAW_H_ */
