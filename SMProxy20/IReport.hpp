#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:消息体接口
**************************************************/

#include "Field.hpp"
#include "IBaseParam.hpp"
#include "exception.hpp"
#include "IDeliver.hpp"

namespace smproxy {


	//brief submit 接口
	class IReport :
		public IDeliver
	{
	public:
		typedef std::vector<uint8_t> bytes;
		IReport()
		{
		}

		virtual	~IReport() = default;

		virtual bytes reportBytes(
			std::string dest_id,
			std::string service_id,
			std::string src_terminal_id,
			std::string msg_id,
			std::string stat,
			std::string submit_time,
			std::string done_time
		) = 0;
		//接收Report
		//@buf 待解析数据
		//@buf_loc 解析起始位置迭代器
		virtual void recvReport(bytes &buf, bytes::iterator buf_loc) = 0;

		void setSubmitTime(std::string submit_time) { submit_time_ = submit_time; };
		std::string getSubmitTime() { return submit_time_.get(); };

		void setDoneTime(std::string done_time) { done_time_ = done_time; };
		std::string getDoneTime() { return done_time_.get(); };

		std::string getStat() { return stat_.get(); };
		uint32_t getSMSCSequence() { return uint32_t(SMSC_sequence_.get()); };

		std::string getMsgId() { return msg_id_.get(); };
	protected:
		FieldStr submit_time_;			//	递交时间
		FieldStr done_time_;			//	完成时间
		FieldStr stat_;
		FieldInt SMSC_sequence_;
		FieldStr msg_id_;

	};

}//smproxy