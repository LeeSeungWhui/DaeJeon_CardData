/*
 * RAW.cpp
 *
 *  Created on: 2018. 3. 13.
 *      Author: lsh
 */

#include "Raw.h"
#include <fstream>
/*
환승시간이 60분인 노선ID
30300005	30300015	30300029	30300042	30300059	30300130	30300094
30300006	30300097	30300030	30300045	30300060	30300075	30300095
30300007	30300016	30300117	30300046	30300061	30300076	30300096
30300008	30300017	30300118	30300048	30300062	30300078	30300103
30300009	30300018	30300032	30300108	30300065	30300080	30300129
30300010	30300021	30300099	30300050	30300066	30300082
30300011	30300025	30300034	30300136	30300067	30300083
30300012	30300026	30300035	30300052	30300068	30300084
30300013	30300027	30300036	30300127	30300069	30300092
30300014	30300028	30300039	30300057	30300073	30300093
*/
string transfer_60min[] = {
	"30300005",	"30300006",	"30300007",	"30300008",	"30300009",	"30300010",	"30300011",	"30300012",	"30300013",	"30300014",	"30300015",	"30300097",	"30300016",
	"30300017",	"30300018",	"30300021",	"30300025",	"30300026",	"30300027",	"30300028",	"30300029",	"30300030",	"30300117",	"30300118",	"30300032",	"30300099",
	"30300034",	"30300035",	"30300036",	"30300039",	"30300042",	"30300045",	"30300046",	"30300048",	"30300108",	"30300050",	"30300136",	"30300052",	"30300127",
	"30300057",	"30300059",	"30300060",	"30300061",	"30300062",	"30300065",	"30300066",	"30300067",	"30300068",	"30300069",	"30300073",	"30300130",	"30300075",
	"30300076",	"30300078",	"30300080",	"30300082",	"30300083",	"30300084",	"30300092",	"30300093",	"30300094",	"30300095",	"30300096",	"30300103",	"30300129"
};
bool findseq(vector<RAWDATA*>* stack, int idx1, int idx2, vector<int>* temporal_seq, int max);
unordered_multimap<long, RAWDATA*> RAW::data;

RAW::RAW() {
	// TODO Auto-generated constructor stub

}

RAW::~RAW() {
	// TODO Auto-generated destructor stub
	for(auto n:data)
	{
		delete n.second;
	}
	data.clear();
	for(auto n:rawdataIdx)
	{
		delete n;
	}
	rawdataIdx.clear();
}

RAW::RAW(sql::ResultSet* res)
{
	cout << " 원시데이터 가져오는 중...";
	long rawidx = 0;
	while(res->next() == true)
	{
		string sttn_id = res->getString("승차정류장ID");
		if(sttn_id != "0000000" && sttn_id != "~") // 승차정류장ID가 ~나 0인 경우 제거
		{
			if(res->getString("노선ID") == "30300080" && sttn_id == "8007268")
			{

			}
			else
			{
				RAWDATA* rawdata = new RAWDATA(res);
				RAW::data.insert(pair<long, RAWDATA*>(rawidx, rawdata));
				rawdataIdx.push_back(new INTWRAPPER(rawidx));
				++rawidx;
			}
		}
	}
	cout << "완료!" << endl;
}

void RAW::print(ostream* fout)
{
	*fout <<
			"카드번호(암호화),정산지역코드,승차일시,승차전하차일시,하차일시,환승횟수,교통수단,승차전교통수단,교통사업자ID," <<
			"교통사업자,노선ID,노선명,차량ID,차량등록번호,운행출발일시,운행종료일시,승차정류장ID,승차정류장명,승차정류장행정동," <<
			"하차정류장ID,하차정류장명,하차정류장행정동,카드권종교통사용자구분,금액권종교통사용자구분,발행사명,승객수,승차금액," <<
			"하차금액,환승할인금액,통행거리,통행시간,트랜젝션ID,승하차위반금액,승차정류장순서,차량별운행차수, 요일"
			 <<
			endl;
	for (auto n : rawdataIdx)
	{
		pair<long, RAWDATA*> d = *RAW::data.find(n->idx);
		d.second->print(fout);
	}
}

void RAW::print_normalize(ostream* fout) // 표준화 출력함수
{
	*fout <<
			"일련번호,가상카드번호,정산지역코드,카드구분코드,차량ID,차량등록번호,운행출발일시,운행종료일시,교통수단코드," <<
			"노선ID,교통사업자ID,승차일시,발권시간,승차정류장ID,하차일시,하차정류장ID,트랜젝션ID,환승횟수," <<
			"사용자구분코드,이용객수,승차금액,하차금액,이용거리,탑승시간"
			 <<
			endl;
	int seq = 1; // 일련번호
	for (auto n : rawdataIdx)
	{
		pair<long, RAWDATA*> d = *RAW::data.find(n->idx);
		d.second->print_normalize(fout, seq);
		++seq;
	}
}

void RAW::transitRevise()
{
	cout << "환승횟수 보정 작업 중...";
	//string trnsit_id_list[3]; // 이미 환승한 노선ID 체크용 배열
	string route_id_pre = ""; // 이전 버스 노선ID
	int trnsit_cnt = 0; // 현재 환승횟수
	string card_no_pre = ""; // 이전 카드번호
	string card_no_next = ""; // 다음 카드번호
	tm gff_dt_pre; // 이전 row의 하차시간;

	rawdataIdx.sort(INTWRAPPER::sortBy_card_tkdt); // 카드번호, 승차시간 순으로 정렬

	for(auto n:rawdataIdx)
	{
		RAWDATA* tuple = (*RAW::data.find(n->idx)).second;
		if(card_no_pre == "") // 첫번째 row
		{
			card_no_pre = tuple->VIRTL_CARD_NO; // 현재 row의 카드번호
			tuple->TRNSIT_CNTC = "0"; // 환승횟수 0
			//trnsit_id_list[0] = tuple->ROUTE_ID; // 노선id리스트에 추가
			route_id_pre = tuple->ROUTE_ID;
		}
		else // 두번째 row부터
		{
			card_no_next = tuple->VIRTL_CARD_NO; // 다음 row 카드번호
			string route_id_next = tuple->ROUTE_ID; // 다음 row 노선id
			tm tkcar_dt_aft = tuple->TKCAR_DT; // 다음 승차시각

			bool card_no_check = (card_no_pre == card_no_next); // 카드 번호 확인
			bool trnsit_cnt_check = (trnsit_cnt < 3); // 환승횟수 확인
			bool gff_dt_check = (gff_dt_pre.tm_year != 0); // 하차시간 존재 확인

			bool trnsit_possibility_check = false; // 환승 가능 여부
			if((card_no_check == true) && (trnsit_cnt_check == true) && (gff_dt_check == true)) // 위의 check들이 true일때만 확인
			{
				time_t gff_dt_pre_t = mktime(&gff_dt_pre);
				time_t tkcar_dt_aft_t = mktime(&tkcar_dt_aft);
				double diff = difftime(tkcar_dt_aft_t, gff_dt_pre_t); // 하차와 승차간의 시간차
				double transfer_limit = 30*60; // 환승가능시간(30분)
				for(auto n:transfer_60min) // 60분까지 환승가능한 노선인지 확인
				{
					if(n == route_id_next)
					{
						transfer_limit = 60*60;
						break;
					}
				}
				if(diff <= transfer_limit) // 시간 차가 환승가능시간 이내일때
				{
					trnsit_possibility_check = true;
					/* 임시로 빼놓은 기능
					for(auto n:trnsit_id_list) // 이전에 환승한 노선id일 경우 환승불가
					{
						if(n == route_id_next)
						{
							trnsit_possibility_check = false;
							break;
						}
					}*/
					// 이전 버스의 노선ID와 비교
					if(route_id_pre == route_id_next)
					{
						trnsit_possibility_check = false;
					}
				}
			}
			if(trnsit_possibility_check == true) // 환승 가능
			{
				trnsit_cnt++; // 환승횟수 추가
				/*if(trnsit_cnt < 3)
				{
					trnsit_id_list[trnsit_cnt] = route_id_next; // 노선id리스트에 현재 노선id추가 (동일노선 환승불가 체크용)
				}*/
				tuple->TRNSIT_CNTC = to_string(trnsit_cnt);
			}
			else // 환승 불가
			{
				tuple->TRNSIT_CNTC = "0"; // 다음 row의 환승횟수는 0
				trnsit_cnt = 0;
				/*// 노선id리스트 비우기
				trnsit_id_list[1] = trnsit_id_list[2] = "";
				trnsit_id_list[0] = tuple->ROUTE_ID;*/
			}
			route_id_pre = route_id_next;
			card_no_pre = card_no_next;
		}
		gff_dt_pre = tuple->GFF_DT; // 현재 row 하차 시간
	}
	cout << "완료!" << endl;
}

void RAW::editraw(BIS* bis) // 정류장순서와 운행차수 추가
{
	cout << "정류장순서, 차량별운행차수 계산 중...";
	// 1. 승차정류장 순서 넣기
	ofstream o("error.txt", ios::app);
	for(auto n = rawdataIdx.begin(); n != rawdataIdx.end();)
	{
		RAWDATA* tuple = (*RAW::data.find((*n)->idx)).second;
		string apply_date_str = "";
		tm apply_date_tm;
		time_t apply_date_t =  0;
		tm temp_apply_date_tm;
		time_t temp_apply_date_t = 0;
		// 적용일시 계산
		if(bis->data_apply_date.find(tuple->ROUTE_ID) != bis->data_apply_date.end())
		{
			auto pair_itr = bis->data_apply_date.equal_range(tuple->ROUTE_ID);
			time_t tkcar_date = mktime(&tuple->TKCAR_DT);
			for(;pair_itr.first != pair_itr.second ;pair_itr.first++)
			{
				RAWDATA::setDate( &temp_apply_date_tm,(*pair_itr.first).second);
				temp_apply_date_t = mktime(&temp_apply_date_tm);
				if(tkcar_date >= temp_apply_date_t && temp_apply_date_t >= apply_date_t)
				{
					apply_date_t = temp_apply_date_t;
					apply_date_tm = temp_apply_date_tm;
					apply_date_str = (*pair_itr.first).second;
				}
			}
		}

		// 적용일시에 맞는 승차정류장 순서 가져오기
		if(bis->data.find(tuple->ROUTE_ID + tuple->TKCAR_STTN_ID + apply_date_str) != bis->data.end())
		{
			auto pair_itr = bis->data.equal_range(tuple->ROUTE_ID + tuple->TKCAR_STTN_ID + apply_date_str);
			for(;pair_itr.first != pair_itr.second ;pair_itr.first++)
			{
				tuple->STTN_SEQ.push_back((*pair_itr.first).second->STTN_SEQ);
			}
			sort(tuple->STTN_SEQ.begin(), tuple->STTN_SEQ.end()); // 오름차순 정렬
			++n;
		}
		else
		{
			o << "날짜 : " << RAWDATA::dateTostr(tuple->TKCAR_DT) <<
					"\t노선ID : " << tuple->ROUTE_ID << "\t정류장ID : " << tuple->TKCAR_STTN_ID << endl;
			delete tuple;
			RAW::data.erase((*n)->idx);
			delete (*n);
			n = rawdataIdx.erase(n);
		}
	}
	o.close();
	// 2. 노선ID, 차량ID, 승차일시, 카드번호, 승차정류장 순으로 정렬
	rawdataIdx.sort(INTWRAPPER::sortBy_route_car_etc);
	ofstream o2("temp.csv");
	this->print(&o2);
	o2.close();

	// 3. 승차정류장순서가 2개인 row에 대한 작업...
	RAWDATA* seq_min = nullptr;
	RAWDATA* seq_max = nullptr;
	vector<RAWDATA*> stack; // 승차정류장순서 보정이 필요한 데이터들
	bool revising = false; // 보정시작할지 말지 여부

	for(auto n:rawdataIdx)
	{
		RAWDATA* tuple = (*RAW::data.find(n->idx)).second;
//		if(tuple->VIRTL_CARD_NO == "m2XPBPXrQXrUYeZH+USyFIQv/bXgVg4cX0DoQeaOsnA=" && tuple->ROUTE_ID == "30300048")
//		{
//			cout << "here" << endl;
//		}
		if(stack.size() == 0) // stack이 비어있을 땐 정류장순서보정이 필요없음. min값만 갱신하면 됨
		{
			if(tuple->ROUTE_ID != "S3001") // 지하철에 대해서는 계산하지 않음
			{
				if(tuple->STTN_SEQ.size() == 1) // 정류장순서가 1:1매칭
				{
					seq_min = tuple;
					seq_max = nullptr;
				}
				else // 정류장 순서가 2개 이상임
				{
					if(seq_min != nullptr && (seq_min->CAR_ID != tuple->CAR_ID || seq_min->ROUTE_ID != tuple->ROUTE_ID)) // 차량ID, 노선ID가 다르면 min 없음
					{
						seq_min = nullptr;
					}
					if(seq_min != nullptr)
					{
						stack.push_back(seq_min);
					}
					stack.push_back(tuple); // 스택에 추가
					seq_max = nullptr;
				}
			}
		}
		else // stack이 비어있지 않으면 정류장순서보정이 필요
		{
			if(stack.back()->CAR_ID != tuple->CAR_ID ||
				stack.back()->ROUTE_ID != tuple->ROUTE_ID ||
				rawdataIdx.back() == n) // 차량ID,노선ID가 다르거나 데이터의 마지막이라면 max값이 없음
			{
				revising = true; // 보정시작
			}
			else // 같은 차량임!
			{
				if(tuple->STTN_SEQ.size() == 1) // 정류장 순서가 1:1 매칭되면 이 row의 정류장이 가장 뒤쪽임!
				{
					seq_max = tuple;
					revising = true; // 보정 시작
				}
				else // 이 row도 정류장 순서가 2개 이상이라고?
				{
					stack.push_back(tuple); // 스택행
				}
			}
			if(revising == true) // 보정
			{
				// case 1: min= null, max
				if(seq_min == nullptr && seq_max != nullptr) // max보다 큰 값 제거
				{
					for(auto stack_data:stack)
					{
						for(unsigned int idx = 0; idx < stack_data->STTN_SEQ.size();)
						{
							if(stack_data->STTN_SEQ[idx] > seq_max->STTN_SEQ[0])
							{
								stack_data->STTN_SEQ.erase(stack_data->STTN_SEQ.begin() + idx);
							}
							else
							{
								idx++;
							}
						}
					}
				}
				// case 2: min, max = null
				else if(seq_min != nullptr && seq_max == nullptr) // min보다 작은 값 제거
				{
					for(auto stack_data:stack)
					{
						for(unsigned int idx = 0; idx < stack_data->STTN_SEQ.size();)
						{
							if(stack_data->STTN_SEQ[idx] < seq_min->STTN_SEQ[0])
							{
								stack_data->STTN_SEQ.erase(stack_data->STTN_SEQ.begin() + idx);
							}
							else
							{
								idx++;
							}
						}
					}
				}
				// case 3: min = null, max = null -> 바로 보정
				else if(seq_min == nullptr && seq_max == nullptr)
				{
					/* blank */
				}
				// case 4: min, max
				else if(seq_min->STTN_SEQ[0] < seq_max->STTN_SEQ[0]) // min이 max보다 작은 경우(정상)
				{
					for(auto stack_data:stack) // min보다 작은 값, max보다 큰 값 날리기
					{
						for(unsigned int idx = 0; idx < stack_data->STTN_SEQ.size();)
						{
							if(stack_data->STTN_SEQ[idx] < seq_min->STTN_SEQ[0] ||
								stack_data->STTN_SEQ[idx] > seq_max->STTN_SEQ[0])
							{
								stack_data->STTN_SEQ.erase(stack_data->STTN_SEQ.begin() + idx);
							}
							else
							{
								idx++;
							}
						}
					}
				}
				else // min이 max보다 큰경우(종점을 돌아서 다시 출발한 경우)
				{
					for(auto stack_data:stack) // min보다 작고 max보다 큰 값 날리기(max~min으로 가는 경로는 안지난 경로기때문)
					{
						for(unsigned int idx = 0; idx < stack_data->STTN_SEQ.size();)
						{
							if(stack_data->STTN_SEQ[idx] < seq_min->STTN_SEQ[0] &&
								stack_data->STTN_SEQ[idx] > seq_max->STTN_SEQ[0])
							{
								stack_data->STTN_SEQ.erase(stack_data->STTN_SEQ.begin() + idx);
							}
							else
							{
								idx++;
							}
						}
					}
				}
				// 정류장순서찾기
				vector<int> temporal_seq;
				int i =0;
				int max = 0;
				if(seq_max != nullptr)
				{
					max = seq_max->STTN_SEQ[0];
					stack.push_back(seq_max);
				}
				if(findseq(&stack, 0, 0, &temporal_seq, max) == true)
				{
					for(auto stack_data:stack)
					{
						stack_data->STTN_SEQ.clear();
						stack_data->STTN_SEQ.push_back(temporal_seq[i]);
						++i;
					}
				}
				seq_min = seq_max = nullptr;
				revising = false;
				stack.clear();
			}
		}
	}

	// 4. 차령별운행차수 계산
	RAWDATA* data_pre = nullptr;
	for(auto n:rawdataIdx)
	{
		RAWDATA* tuple = (*RAW::data.find(n->idx)).second;
		if(data_pre != nullptr) // 둘째 row부터
		{
			if(tuple->ROUTE_ID == data_pre->ROUTE_ID && // 노선ID, 차량ID가 같으면 첫운행이 아닐 수도 잇음
				tuple->CAR_ID == data_pre->CAR_ID &&
				tuple->ROUTE_ID != "S3001")
			{
				if((tuple->STTN_SEQ[0] - data_pre->STTN_SEQ[0]) < 0) // 한바퀴 돌았음
				{
					tuple->CAR_OPRAT_CNT = data_pre->CAR_OPRAT_CNT + 1; // 운행차수++
				}
				else
				{
					tuple->CAR_OPRAT_CNT = data_pre->CAR_OPRAT_CNT;
				}
			}
			else if(tuple->ROUTE_ID == "S3001")
			{
				tuple->CAR_OPRAT_CNT = 0;
			}
		}
		data_pre = tuple;
	}

	// 다시 카드번호,승차일시 순으로 정렬
	rawdataIdx.sort(INTWRAPPER::sortBy_card_tkdt);
	cout << "완료!" << endl;
}

bool findseq(vector<RAWDATA*>* stack, int idx1, int idx2, vector<int>* temporal_seq, int max)
{
	if(stack->size() == 1) // 1개밖에 없을 경우
	{
		temporal_seq->push_back(stack->at(idx1)->STTN_SEQ[idx2]);
		return true;
	}
	else
	{
		for(unsigned int i = 0; i < stack->at(idx1+1)->STTN_SEQ.size(); i++)
		{
			int found = -1;
			if(stack->at(idx1)->STTN_SEQ[idx2] <= stack->at(idx1+1)->STTN_SEQ[i]) // 정류장순서가 순차적으로 되야함
			{
				found = max;
			}
			else if(stack->at(idx1+1)->STTN_SEQ[i] <= max)// 중간에 종점을 지나침!
			{
				found = 0;
			}
			if(found != -1)
			{
				temporal_seq->push_back(stack->at(idx1)->STTN_SEQ[idx2]);
				if(idx1 == (int)stack->size()-2)
				{
					temporal_seq->push_back(stack->at(idx1+1)->STTN_SEQ[i]);
					return true;
				}
				// 종점은 한번만 지나치므로 한번 지나치면 max값을 0으로 보정
				else if(findseq(stack, idx1+1, i, temporal_seq,found) == true)
				{
					return true;
				}
				else
				{
					temporal_seq->pop_back();
				}
			}
		}
	}
	return false;
}
