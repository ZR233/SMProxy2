#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:消息体接口
**************************************************/

#include "Field.hpp"
#include "IBaseParam.hpp"
#include "exception.hpp"
namespace smproxy {


	//brief submit 接口
	class IDeliver :
		public IBaseParam
	{
	public:
		typedef std::vector<uint8_t> bytes;
		IDeliver():
			report_flag_(1,0)
		{
		}

		virtual	~IDeliver() = default;

		virtual bytes deliverBytes(
			std::string dest_id,
			std::string service_id,
			std::string src_terminal_id,
			std::string msg_id,
			std::string stat,
			std::string submit_time,
			std::string done_time,
			uint8_t msg_coding, 
			bytes msg_text
		) = 0;
		//接收Report
		//@buf 待解析数据
		//@buf_loc 解析起始位置迭代器
		virtual void recvDeliver(bytes &buf, bytes::iterator buf_loc) = 0;


		std::string getSrcTerminalId() { return src_term_ID_.get(); };

		void setDestId(std::string msg_src) { dest_ID_ = msg_src; };
		std::string getDestId() { return dest_ID_.get(); };

		std::string getMsgIdThis() { return msg_id_this_.get(); };

		bytes getMsgContent() { return message_content_; };

		int getMsgFmt() { return int( msg_fmt_.get()); };

		int getReportFlag(){ return int(report_flag_.get()); }

		int getTpPid() { return int(tp_pid_.get()); }

		int getTpUdhi() { return int(tp_udhi_.get()); }

		int getMsgLen() { return int(message_length_.get()); };

		std::string getISMGId() { return ISMG_ID_.get(); };
	protected:
		//------------常用参数---------------

		//报告去向(SP_号码及扩展码)
		FieldStr dest_ID_;
		/*	信息格式
		0：ASCII
		8：UCS2编码
		15:含GB汉字
		*/
		FieldInt msg_fmt_;
		//报告来源(用户号码)
		FieldStr src_term_ID_;
		//短信长度
		FieldInt message_length_;

		//----------不常用参数------------   



		FieldStr service_id_;			//	业务类型

		FieldStr service_type_;//业务类型，是数字、字母和符号的组合。

		FieldInt report_flag_; //	是否为状态报告

		FieldInt message_type_;

		FieldStr msg_id_this_;
		//----------长短信参数----------------

		//相同Msg_Id的信息总条数
		FieldInt pk_total_;
		//相同Msg_Id的信息序号
		FieldInt pk_number_;
		//TP-Protocol-Identifier
		FieldInt tp_pid_;
		//TP-User-Data-Header-Indicator
		FieldInt tp_udhi_;
		//一条长短信切分数量
		FieldInt same_msg_total_;
		//长短信切分开的第几条
		FieldInt same_msg_num_;
		//长短信切片id
		FieldInt long_msg_id_;

		bytes message_content_;
	};

}//smproxy