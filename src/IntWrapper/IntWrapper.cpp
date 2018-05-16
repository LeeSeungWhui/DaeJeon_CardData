/*
 * INTWRAPPER.cpp
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#include "IntWrapper.h"
#include "../Raw/Raw.h"
INTWRAPPER::INTWRAPPER()
{
	// TODO Auto-generated constructor stub
	idx = 0;
}

INTWRAPPER::INTWRAPPER(int idx)
{
	this->idx = idx;
}
INTWRAPPER::~INTWRAPPER()
{
	// TODO Auto-generated destructor stub
}

bool INTWRAPPER::sortBy_card_tkdt(INTWRAPPER* lhs, INTWRAPPER* rhs) // 카드번호, 승차일시 순으로 정렬
{
	RAWDATA* lhs_raw = (*RAW::data.find(lhs->idx)).second;
	RAWDATA* rhs_raw = (*RAW::data.find(rhs->idx)).second;
	if(lhs_raw->VIRTL_CARD_NO == rhs_raw->VIRTL_CARD_NO) // 카드번호
	{
		if(lhs_raw->TKCAR_DT.tm_hour == rhs_raw->TKCAR_DT.tm_hour) // 승차 시
		{
			if(lhs_raw->TKCAR_DT.tm_min == rhs_raw->TKCAR_DT.tm_min) // 승차 분
			{
				return (lhs_raw->TKCAR_DT.tm_sec < rhs_raw->TKCAR_DT.tm_sec); // 승차 초
			}
			else
			{
				return (lhs_raw->TKCAR_DT.tm_min < rhs_raw->TKCAR_DT.tm_min);
			}
		}
		else
		{
			return (lhs_raw->TKCAR_DT.tm_hour < rhs_raw->TKCAR_DT.tm_hour);
		}
	}
	else
	{
		return (lhs_raw->VIRTL_CARD_NO < rhs_raw->VIRTL_CARD_NO);
	}
}

bool INTWRAPPER::sortBy_route_car_etc(INTWRAPPER* lhs, INTWRAPPER* rhs) // 노선ID,차량ID,승차일시,카드번호 순으로 정렬
{
	RAWDATA* lhs_raw = (*RAW::data.find(lhs->idx)).second;
	RAWDATA* rhs_raw = (*RAW::data.find(rhs->idx)).second;
	if(lhs_raw->ROUTE_ID == rhs_raw->ROUTE_ID) // 노선ID
	{
		if(lhs_raw->CAR_ID == rhs_raw->CAR_ID) // 차량ID
		{
			if (lhs_raw->TKCAR_DT.tm_hour == rhs_raw->TKCAR_DT.tm_hour) // 승차 시
			{
				if (lhs_raw->TKCAR_DT.tm_min == rhs_raw->TKCAR_DT.tm_min) // 승차 분
				{
					if(lhs_raw->TKCAR_DT.tm_sec == rhs_raw->TKCAR_DT.tm_sec) // 승차 초
					{
						return (lhs_raw->VIRTL_CARD_NO < rhs_raw->VIRTL_CARD_NO); // 카드ID
					}
					else
					{
						return (lhs_raw->TKCAR_DT.tm_sec < rhs_raw->TKCAR_DT.tm_sec);
					}
				}
				else
				{
					return (lhs_raw->TKCAR_DT.tm_min < rhs_raw->TKCAR_DT.tm_min);
				}
			}
			else
			{
				return (lhs_raw->TKCAR_DT.tm_hour < rhs_raw->TKCAR_DT.tm_hour);
			}
		}
		else
		{
			return (lhs_raw->CAR_ID < rhs_raw->CAR_ID);
		}
	}
	else
	{
		return (lhs_raw->ROUTE_ID < rhs_raw->ROUTE_ID);
	}
}
