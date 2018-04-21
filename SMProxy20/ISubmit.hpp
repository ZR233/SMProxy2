#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:消息体接口
**************************************************/

#include "Field.hpp"
#include "IBaseParam.hpp"

namespace smproxy {


	//brief submit 接口
	class ISubmit:
		virtual public IBaseParam
	{
	public:
		typedef std::vector<uint8_t> bytes;
		ISubmit()
		{
		}

		virtual	~ISubmit() = default;


		/**
		* \brief  生成submit buf
		* \param userNum 电话号码vector
		* \param msg 短信内容
		* \return 发送缓存字符串
		*/
		virtual	const bytes& submit(
			char* msg_src,
			std::vector<std::string> &user_num,
			int msg_fmt,
			std::vector<uint8_t>& message_content,
			int pk_total = 1,
			int pk_number = 1,
			int tp_pid = 0,
			int tp_udhi = 0,
			int same_msg_total = 1,
			int same_msg_num = 1,
			int long_msg_id = 0
		) = 0;
		//接收submit
		//@buf 待解析数据
		//@buf_loc 解析起始位置迭代器
		virtual void recvSubmit(bytes &buf, bytes::iterator buf_loc) = 0;

		const bytes& getBuf()
		{
			return buf_;
		}


		std::vector<std::string>& getPhNums() { return dest_term_IDs_; };
		// 取短消息长度
		int getTextL() { return message_length_.get(); };
		// 取短消息内容
		std::vector<uint8_t>& getText() { return message_content_; };
		// 取手机号数量
		int getUserCount() { return int(dest_total_.get()); };
		void setTP_pid(uint8_t TP_pid) { tp_pid_ = TP_pid; };
		int getTP_udhi() { return tp_udhi_.get(); };
		void setTP_udhi(uint8_t TP_udhi) { tp_udhi_ = TP_udhi; };

		int getSame_msg_total() { return same_msg_total_.get(); };
		int getSame_msg_num() { return same_msg_num_.get(); };
		uint32_t getLong_msg_id() { return long_msg_id_.get(); };
		int getPk_total() { return  pk_total_.get(); };
		void setPk_total(uint8_t  pk_total) { pk_total_ = pk_total; };
		int getPk_number() { return  pk_number_.get(); };
		void setPk_number(uint8_t pk_number) { pk_number_ = pk_number; };
		void setMsg_src_(std::string msg_src) { src_ID_ = msg_src; };
		std::string getMsg_src_() { return src_ID_.get(); };
	protected:
		//------------常用参数---------------

		//信息内容来源(SP_号码及扩展码)
		FieldStr src_ID_;
		/*	信息格式
		0：ASCII
		8：UCS2编码
		15:含GB汉字
		*/
		FieldInt msg_fmt_;
		//接收信息的用户数量(===1)
		FieldInt dest_total_;
		//手机号vector
		std::vector<std::string> dest_term_IDs_;
		//短信长度
		FieldInt message_length_;
		//短信内容
		std::vector<uint8_t> message_content_;
		//数据串
		std::vector<uint8_t> buf_;


		//----------不常用参数------------   


		FieldInt msg_level_;					//	信息级别
		FieldStr service_id_;			//	业务类型
		FieldInt fee_usertype_;			//	计费用户类型字段
		FieldStr fee_number_;			//	被计费用户的号码
		FieldStr fee_type_;				//	资费类别
		FieldStr fee_code_;				//	资费代码（以分为单位）
		FieldStr valid_time_;				//	存活有效期，参见SMPP3.3
		FieldStr at_time_;				//	定时发送时间，参见SMPP3.3 

		FieldStr charge_number_;			//付费号码，手机号码前加“86”国别标志；当且仅当群发且对用户收费时为空；
											//如果为空，则该条短消息产生的费用由UserNumber代表的用户支付；
											//如果为全零字符串“000000000000000000000”，表示该条短消息产生的费用由SP支付。*/
		FieldStr service_type_;//业务类型，是数字、字母和符号的组合。
		FieldStr fee_value_;
		FieldStr given_value_;
		/**
		* \brief 代收费标志，0：应收；1：实收
		*/
		FieldInt agent_flag_;
		/**
		* \brief 引起MT消息的原因
		* 0-MO点播引起的第一条MT消息；
		* 1-MO点播引起的非第一条MT消息；
		* 2-非MO点播引起的MT消息；
		* 3-系统反馈引起的MT消息。
		*/
		FieldInt morelateto_MT_flag_;
		FieldInt report_flag_; //	是否要求返回状态确认报告

		FieldInt message_type_;

		FieldStr msg_id_;
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
	};

}//smproxy