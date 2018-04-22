#pragma once
#include <string>
#include <vector>
#include "Field.hpp"
#include "IBaseParam.hpp"
#include "SMProxy2.h"
/*************************************************
Author:zr
Date:2018-03-5
Description:head 接口
**************************************************/

namespace smproxy {


	//head 接口
	class IHead :
		public IBaseParam
	{
		typedef std::vector<uint8_t> bytes;
	public:
		IHead() :
			SN1_(4, 3053100000 + std::stol(sp_ID_.get())),
			SN2_(4),
			cmdID_(0),
			total_len_(4)
		{
			buf_.clear();
		}
		//SGIP1.2 SN2
		void setSN2(uint32_t SN2) { this->SN2_.set(SN2); };
		//生成下一个流水号
		virtual uint32_t createSerualNum(uint32_t serial_numb) {
			if (serial_numb == 99999)
			{
				serial_numb = 0;
			}
			++serial_numb;
			return serial_numb;
		};
		virtual bytes header(uint32_t msg_len, Command cmdID, std::string serial_numb) = 0;
		virtual bytes::iterator recvHead(bytes& buf) = 0;
		// 消息长度
		uint32_t getMsgLen() { return total_len_.get(); };
		// 指令ID
		Command getCmdID() { return cmd_ID_; };

		void setCmdID(uint32_t cmdID) { this->cmdID_ = cmdID; };

		// SN2
		uint32_t getSN2() { return SN2_.get(); };

		// 取纯数字流水号
		uint32_t getIntSerialNumb() { return int_serial_numb_.get(); };
		virtual ~IHead() {};
		smproxy::FieldInt SN1_;//通信节点编号
		smproxy::FieldInt SN2_;
		smproxy::FieldInt cmdID_;
		smproxy::FieldInt total_len_;
		Command cmd_ID_;
	protected:

	};//IHead

}//smproxy