/*
 * INTWRAPPER.h
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#ifndef SRC_INTWRAPPER_INTWRAPPER_H_
#define SRC_INTWRAPPER_INTWRAPPER_H_

class INTWRAPPER {
public:
	INTWRAPPER();
	INTWRAPPER(int idx);
	virtual ~INTWRAPPER();
	long idx;
	static bool sortBy_card_tkdt(INTWRAPPER* lhs, INTWRAPPER* rhs); // 카드번호, 승차일시 순으로 정렬
	static bool sortBy_route_car_etc(INTWRAPPER* lhs, INTWRAPPER* rhs); // 노선id, 차량id, 승차일시, 카드번호 순으로 정
};

#endif /* SRC_INTWRAPPER_INTWRAPPER_H_ */
