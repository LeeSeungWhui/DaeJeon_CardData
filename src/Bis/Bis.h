/*
 * BIS.h
 *
 *  Created on: 2018. 3. 14.
 *      Author: lsh
 */

#ifndef SRC_BIS_BIS_H_
#define SRC_BIS_BIS_H_
#include "BisData.h"
#include <unordered_map>
class BIS {
public:
	BIS();
	BIS(sql::ResultSet* res);
	virtual ~BIS();
	void apply_date(sql::ResultSet* res);

	std::unordered_multimap<std::string, BISDATA*> data;
	std::unordered_multimap<std::string,std::string> data_apply_date;
};

#endif /* SRC_BIS_BIS_H_ */
