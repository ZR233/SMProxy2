#pragma once
#include "ISubmit.hpp"
#include "SMGP303Head.hpp"

/*************************************************
Author:zr
Date:2018-04-11
Description:SMGPv303 submit处理
**************************************************/



/**
* \brief 用于递交短信
*/
class SMGP303Submit :
	public ISubmit
{
public:
	SMGP303Submit() :ISubmit(0x00000002)
	{
		setHeadType(&hd_);
	}
	virtual ~SMGP303Submit() {};
	// 生成submit buf
	bytes submit(std::vector<std::string> &user_num, std::string &msg) override
	{
		if (user_num.size() > 100 || user_num.size() < 1)
			throw smproxy::exception("手机号数量错误");
		std::string msg_str = msg.c_str();
		if (msg_str.size() > 140)
			throw smproxy::exception("短信内容过长");
		buf_.clear();
		/*
		MsgType 短消息类型。
		对于回执消息该字段无效；对于文本短消息，该字段表示短消息的消息流向：
		0＝MO消息（终端发给SP）；
		6＝MT消息（SP发给终端，包括WEB上发送的点对点短消息）；
		7＝点对点短消息；
		其它保留。
		*/
		buf_.push_back(6);
		/*
		NeedReport 是否要求返回状态报告。
		0＝不要求返回状态报告；
		1＝要求返回状态报告；
		其它保留。
		*/
		buf_.push_back(report_flag_);
		/* 
		Priority
		短消息发送优先级。
		0＝低优先级；
		1＝普通优先级；
		2＝较高优先级；
		3＝高优先级；
		其它保留。
		*/
		buf_.push_back(msg_level_);

		//service_id
		service_id_.resize(10);
		buf_.insert(buf_.end(), service_id_.begin(), service_id_.end());
		//fee_type
		fee_type_.resize(2);
		buf_.insert(buf_.end(), fee_type_.begin(), fee_type_.end());
		//fee_code
		fee_code_.resize(6);
		buf_.insert(buf_.end(), fee_code_.begin(), fee_code_.end());
		//FixedFee
		std::string fixed_fee(6,9);
		buf_.insert(buf_.end(), fixed_fee.begin(), fixed_fee.end());

		//MsgFormat
		buf_.push_back(msg_fmt_);
		//ValidTime
		valid_time_.resize(17);
		buf_.insert(buf_.end(), valid_time_.begin(), valid_time_.end());
		//AtTime
		at_time_.resize(17);
		buf_.insert(buf_.end(), at_time_.begin(), at_time_.end());
		//SrcTermID
		auto msg_src = msg_src_.toBytes();
		buf_.insert(buf_.end(), msg_src.begin(), msg_src.end());
		//ChargeTermID
		fee_number_.resize(21);
		buf_.insert(buf_.end(), fee_number_.begin(), fee_number_.end());

		//DestTermIDCount
		dest_total_ = user_num.size();
		buf_.push_back(dest_total_);
		//DestTermID
		for (auto i = 0; i < user_num.size(); i++)
		{
			bytes temp_ph;
			temp_ph.assign(user_num[i].begin(), user_num[i].end());
			temp_ph.resize(21);
			buf_.insert(buf_.end(), temp_ph.begin(), temp_ph.end());
		}
		//MsgLength
		message_length_ = msg_str.size();
		buf_.push_back(uint8_t(message_length_));
		//MsgContent
		buf_.insert(buf_.end(), msg_str.begin(), msg_str.end());
		//Reserve
		for (size_t i = 0; i < 8; i++)
		{
			buf_.push_back('\0');
		}
		if (tp_udhi_ > 0)
		{
			
			{
				smproxy::FieldTLV tp_udhi(0x0002);
				tp_udhi.set(0x0002, tp_udhi_);
				auto temp = tp_udhi.toBytes();
				buf_.insert(buf_.end(), temp.begin(), temp.end());
			}
			{
				smproxy::FieldTLV pk_total(0x0009);
				pk_total.set(0x0009, pk_total_);
				auto temp = pk_total.toBytes();
				buf_.insert(buf_.end(), temp.begin(), temp.end());
			}
			{
				smproxy::FieldTLV pk_number(0x000A);
				pk_number.set(0x000A, pk_number_);
				auto temp = pk_number.toBytes();
				buf_.insert(buf_.end(), temp.begin(), temp.end());
			}
		}
	}
	// 接收submit
	void recvSubmit(bytes &buf) override {
		auto iter = buf.begin();
		if (buf.size() < 105 )
			BOOST_THROW_EXCEPTION(smproxy::exception("数据长度错误", 10));
		iter += 12;
		/*
		MsgType 短消息类型。
		对于回执消息该字段无效；对于文本短消息，该字段表示短消息的消息流向：
		0＝MO消息（终端发给SP）；
		6＝MT消息（SP发给终端，包括WEB上发送的点对点短消息）；
		7＝点对点短消息；
		其它保留。
		*/
		uint8_t MsgType = *iter;
		++iter;
		/*
		NeedReport 是否要求返回状态报告。
		0＝不要求返回状态报告；
		1＝要求返回状态报告；
		其它保留。
		*/
		report_flag_ = *iter;
		++iter;
		/*
		Priority
		短消息发送优先级。
		0＝低优先级；
		1＝普通优先级；
		2＝较高优先级；
		3＝高优先级；
		其它保留。
		*/
		msg_level_ = *iter;
		++iter;

		//service_id
		service_id_ = service_id_.assign(iter, iter + 10).c_str();
		iter += 10;
		//fee_type
		fee_type_ = fee_type_.assign(iter, iter + 2).c_str();
		iter += 2;
		//fee_code
		fee_code_ = fee_code_.assign(iter, iter + 6).c_str();
		iter += 6;
		//FixedFee
		auto fixed_fee = fee_code_.assign(iter, iter + 6).c_str();
		iter += 6;
		//MsgFormat
		msg_fmt_ = *iter;
		++iter;
		//ValidTime
		valid_time_ = valid_time_.assign(iter, iter + 17).c_str();
		iter += 17;
		//AtTime
		at_time_ = at_time_.assign(iter, iter + 17).c_str();
		iter += 17;
		//SrcTermID
		extnum_ = extnum_.assign(iter, iter + 21).c_str();
		iter += 21;
		//ChargeTermID
		fee_number_ = fee_number_.assign(iter, iter + 21).c_str();
		iter += 21;

		//DestTermIDCount
		dest_total_ = *iter;
		++iter;
		if (buf.size() < 105 + dest_total_*21 + 8)
			BOOST_THROW_EXCEPTION(smproxy::exception("数据长度错误", 10));
		if (dest_total_ >100)
		{
			BOOST_THROW_EXCEPTION(smproxy::exception("手机号过多", 129));
		}
		//DestTermID
		num_.clear();
		for (auto i = 0; i <dest_total_; i++)
		{
			std::string tempstr;
			tempstr.assign(iter, iter + 21);
			tempstr = tempstr.c_str();
			if (tempstr.size()>11)
			{
				BOOST_THROW_EXCEPTION(smproxy::exception("手机格式错", 10));
			}
			num_.push_back(tempstr);
			iter += 21;
		}
		//MsgLength
		message_length_ = uint32_t(*iter);
		if (int(message_length_)>140)
		{
			BOOST_THROW_EXCEPTION(smproxy::exception("短信内容过长", 36));
		}
		if (buf.size() < (105 + 21 * dest_total_ + 1 + int(message_length_) + 8))
		{
			BOOST_THROW_EXCEPTION(smproxy::exception("submit消息长度错误", 10));
		}
		++iter;
		//MsgContent
		bytes message_content;
		message_content.assign(iter, iter + message_length_);
		iter += message_length_;
		//Reserve
		iter += 8;
		//长短信处理-------------------------------------------------
		while (iter != buf.end())
		{
			bytes temp;
			uint16_t tag;
			uint16_t len;
			temp.assign(iter, iter + 2);
			memcpy_s(&tag, 2, &(temp[0]), 2);
			tag = ntohs(tag);
			iter += 2;
			temp.assign(iter, iter + 2);
			memcpy_s(&len, 2, &(temp[0]), 2);
			len = ntohs(len);
			if (len == 0 || len > 60000)
			{
				throw smproxy::exception("错误的TLV数值长度", 10);
			}
			iter += 2;
			switch (tag)
			{
			case 0x0001://TP_pid
				tp_pid_ = *iter;
				++iter;
				break;
			case 0x0002://TP_udhi
				tp_udhi_ = *iter;
				++iter;
				break;
			case 0x0004://ChargeUserType
			case 0x0005://ChargeTermType
			case 0x0007://DestTermType
				++iter;
				break;
			case 0x0009://PkTotal
				pk_total_ = *iter;
				++iter;
				break;
			case 0x000A://PkNumber
				pk_number_ = *iter;
				++iter;
				break;
			case 0x000B://SubmitMsgType
			case 0x000C://SPDealReslt
			case 0x000D://SrcTermType
			case 0x000F://NodesCount
			case 0x0011://SrcType
				++iter;
				break;
			case 0x0003://LinkID
				iter += 20;
				break;
			case 0x0006://ChargeTermPseudo
			case 0x0008://DestTermPseudo
			case 0x000E://SrcTermPseudo
				iter += len;
				break;
			case 0x0010://MsgSrc
				SPID.assign(iter, iter + 8);
				iter += 8;
				break;
			case 0x0012://MServiceID
				iter += 21;
				break;
			default:
				BOOST_THROW_EXCEPTION(smproxy::exception("错误的TLV可选参数标签", 99));
				break;
			}
		}
		int min = 0;
		iter = message_content.begin();
		if (tp_udhi_ == 1)
		{
			//byte 1 : 05, 表示剩余协议头的长度
			min = (uint32_t)(uint8_t)(*iter) + 1;
			++iter;
			//byte 2 : 00, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为1（格式中的XX值）。
			//byte 2 : 08, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为2（格式中的XX值）。
			uint8_t c = *iter;
			++iter;
			++iter;
			//byte 2 : 00, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为1（格式中的XX值）。
			if (c == 0)
			{
				long_msg_id_ = (uint32_t)(uint8_t)(*iter);
				++iter;
			}
			//byte 2 : 08, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为2（格式中的XX值）。
			else if (c == 8)
			{
				uint16_t d = 0;
				memcpy_s(&d, 2, &(*iter), 2);
				iter += 2;
				long_msg_id_ = (uint32_t)d;
			}
			else
			{
				BOOST_THROW_EXCEPTION(smproxy::exception("长短信内容头格式错误", 10));
			}
			same_msg_total_ = (uint8_t)*iter;
			++iter;
			same_msg_num_ = (uint8_t)*iter;
			++iter;
		}
		//拷贝信息内容-----------------------------------
		switch (msg_fmt_)
		{
		case 8:
		case 9:
		{

			int nDstLength;        // UNICODE宽字符数目
			WCHAR wchar[200] = { 0 };  // UNICODE串缓冲区
			std::wstring wstr(400, L'\0');
			// 高低字节对调，拼成UNICODE
			for (int i = 0; i < (message_length_ - min) / 2; i++)
			{
				wstr[i] = uint16_t(*iter++) << 8;    // 先高位字节
				wstr[i] |= *iter++;        // 后低位字节
			}
			wstr = wstr.c_str();
			// UNICODE串-->字符串
			message_content_ = boost::locale::conv::from_utf<wchar_t>(wstr, "GBK");
		}
		break;
		case 15:
			message_content_.assign(iter, iter + message_length_ - min);
			break;
		default:
			BOOST_THROW_EXCEPTION(smproxy::exception("未识别编码号", 10));
		}
	}


	// 生成submit buf 
	bytes submit(std::vector<std::string> &user_num, uint8_t msg_fmt, std::vector<uint8_t> &msg) override
	{
		buf_.assign(8, 0);
		buf_.push_back(pk_total_);
		buf_.push_back(pk_number_);
		buf_.push_back(report_flag_);
		buf_.push_back(msg_level_);
		service_id_.resize(10);
		buf_.insert(buf_.end(), service_id_.begin(), service_id_.end());
		buf_.push_back(fee_usertype_);
		fee_number_.resize(21);
		buf_.insert(buf_.end(), fee_number_.begin(), fee_number_.end());
		buf_.push_back(tp_pid_);
		buf_.push_back(tp_udhi_);
		buf_.push_back(msg_fmt_);
		extnum_.resize(6);
		buf_.insert(buf_.end(), extnum_.begin(), extnum_.end());
		fee_type_.resize(2);
		buf_.insert(buf_.end(), fee_type_.begin(), fee_type_.end());
		fee_code_.resize(6);
		buf_.insert(buf_.end(), fee_code_.begin(), fee_code_.end());
		valid_time_.resize(17);
		buf_.insert(buf_.end(), valid_time_.begin(), valid_time_.end());
		at_time_.resize(17);
		buf_.insert(buf_.end(), at_time_.begin(), at_time_.end());
		sp_number_.resize(21);
		buf_.insert(buf_.end(), sp_number_.begin(), sp_number_.end());
		dest_total_ = user_num.size();
		buf_.push_back(dest_total_);

		//拷贝手机号----------
		if (user_num.size() > 100)
		{
			throw std::runtime_error("submit 手机号超过100个");
		}
		for (int i = 0; i < user_num.size(); i++)
		{
			bytes temp_ph;
			temp_ph.assign(user_num[i].begin(), user_num[i].end());
			temp_ph.resize(21);
			buf_.insert(buf_.end(), temp_ph.begin(), temp_ph.end());
		}
		//拷贝信息
		if (msg.size() > 140)
		{
			throw std::runtime_error("submit 短信内容超过140");
		}
		message_length_ = (msg.size());
		buf_.push_back(uint8_t(message_length_));
		buf_.insert(buf_.end(), msg.begin(), msg.end());
		for (size_t i = 0; i < 8; i++)
		{
			buf_.push_back('\0');
		}
	}

private:
	SMGP303Head hd_;
};

