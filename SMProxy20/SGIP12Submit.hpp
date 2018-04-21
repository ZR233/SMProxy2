#pragma once
#include "ISubmit.hpp"
#include "SGIP12Head.hpp"

class SGIP12Submit :
	public ISubmit
{
public:
	SGIP12Submit() :ISubmit(3) {
		setHeadType(&hd);
	}

	bytes submit(std::vector<std::string> &user_num, std::string &msg) override
	{
		int pt = 20;
		buf_.clear();
		//拷贝SP接入号 
		sp_number_.resize(21);
		buf_.assign(sp_number_.begin(), sp_number_.end());
		pt += 21;
		
		//拷贝付费号码
		charge_number_.resize(21);
		buf_.insert(buf_.end(), charge_number_.begin(), charge_number_.end());
		pt += 21;
		//拷贝接收短消息的手机号数量
		dest_total_ = user_num.size();
		buf_.push_back(dest_total_);
		pt += 1;
		//拷贝接收短消息的手机号
		for (int i = 0; i < user_num.size(); i++)
		{
			bytes temp_ph;
			temp_ph.assign(user_num[i].begin(), user_num[i].end());
			temp_ph.resize(21);
			buf_.insert(buf_.end(), temp_ph.begin(), temp_ph.end());
			pt += 21;
		}
		//拷贝企业代码
		SPID.resize(5);
		buf_.insert(buf_.end(), SPID.begin(), SPID.end());
		pt += 5;
		//拷贝业务代码，由SP定义
		service_id_.resize(10);
		buf_.insert(buf_.end(), service_id_.begin(), service_id_.end());
		pt += 10;

		//拷贝计费类型
		buf_.push_back((unsigned char)std::stoi(fee_type_));
		pt += 1;

		//拷贝取值范围0-99999，该条短消息的收费值，单位为分，由SP定义
		fee_value_.resize(6);
		buf_.insert(buf_.end(), fee_value_.begin(), fee_value_.end());
		pt += 6;
		//拷贝取值范围0-99999，赠送用户的话费，单位为分，由SP定义，特指由SP向用户发送广告时的赠送话费
		given_value_.resize(6);
		buf_.insert(buf_.end(), given_value_.begin(), given_value_.end());
		pt += 6;

		//拷贝
		buf_.push_back(agent_flag_);
		pt += 1;
		//拷贝
		buf_.push_back(morelateto_MT_flag_);
		pt += 1;
		//拷贝
		buf_.push_back(msg_level_);
		pt += 1;
		//拷贝
		valid_time_.resize(16);
		buf_.insert(buf_.end(), valid_time_.begin(), valid_time_.end());
		pt += 16;
		//拷贝
		at_time_.resize(16);
		buf_.insert(buf_.end(), at_time_.begin(), at_time_.end());
		pt += 16;
		//拷贝
		buf_.push_back(report_flag_);
		pt += 1;
		//拷贝
		buf_.push_back(tp_pid_);
		pt += 1;
		//拷贝
		buf_.push_back(tp_udhi_);
		pt += 1;
		//拷贝
		buf_.push_back(msg_fmt_);
		pt += 1;
		//拷贝
		buf_.push_back(message_type_);
		pt += 1;
		//拷贝短信内容长度
		uint8_t temp_char_message_length[4];
		unsigned int temp_ml = htonl(msg.size());
		memcpy(temp_char_message_length, &temp_ml, 4);
		buf_.insert(buf_.end(), &temp_char_message_length[0], &temp_char_message_length[4]);
		pt += 4;
		//拷贝短信内容
		buf_.insert(buf_.end(), msg.begin(), msg.end());


		for (size_t i = 0; i < 8; i++)
		{
			buf_.push_back(0);
		}
		pt += (msg.size());
		pt += 8;


		return buf_;
	}

	void recvSubmit(bytes &buf) override {
		hd.recvHead(buf);
		msg_id_this = hd.getSerialNumb();
		msg_id = hd.getSerialNumb();
		if (buf.size() < (20 + 21 + 21 + 1))
		{
			throw std::runtime_error("submit 消息长度错误");
		}
		int pt = 20;

		//拷贝SP接入号 
		sp_number_.assign(21, 0);
		for (int i = 0; i < 21; i++)
		{
			sp_number_[i] = buf[i + pt];
		}
		sp_number_ = sp_number_.c_str();
		pt += 21;
		extnum_ = sp_number_;
		//拷贝付费号码
		charge_number_.assign(21, 0);
		for (int i = 0; i < 21; i++)
		{
			charge_number_[i] = buf[i + pt];
		}
		charge_number_ = charge_number_.c_str();
		pt += 21;

		//拷贝接收短消息的手机号数量
		dest_total_ = buf[pt];
		pt += 1;
		num_.clear();
		for (uint8_t i = 0; i < dest_total_; i++)
		{
			char temp[21] = { 0 };
			for (int j = 0; j < 21; j++)
			{
				temp[j] = buf[pt + j];

			}
			std::string tempstr(temp);
			tempstr = tempstr.c_str();
			num_.push_back(tempstr);
			pt += 21;
		}

		//拷贝企业代码
		SPID.assign(5, 0);
		for (int i = 0; i < 5; i++)
		{
			SPID[i] = buf[i + pt];
		}
		SPID = SPID.c_str();
		pt += 5;
		//拷贝业务代码，由SP定义
		service_type_.assign(10, 0);
		for (int i = 0; i < 10; i++)
		{
			service_type_[i] = buf[i + pt];
		}
		service_type_ = service_type_.c_str();
		pt += 10;

		//拷贝计费类型
		fee_type_ = std::to_string(int(buf[pt]));
		pt += 1;

		//拷贝取值范围0-99999，该条短消息的收费值，单位为分，由SP定义
		fee_value_.assign(6, 0);
		for (int i = 0; i < 6; i++)
		{
			fee_value_[i] = buf[i + pt];
		}
		fee_value_ = fee_value_.c_str();
		pt += 6;
		//拷贝取值范围0-99999，赠送用户的话费，单位为分，由SP定义，特指由SP向用户发送广告时的赠送话费
		given_value_.assign(6, 0);
		for (int i = 0; i < 6; i++)
		{
			given_value_[i] = buf[i + pt];
		}
		given_value_ = given_value_.c_str();
		pt += 6;

		//拷贝
		agent_flag_ = buf[pt];
		pt += 1;
		//拷贝
		morelateto_MT_flag_ = buf[pt];
		pt += 1;
		//拷贝
		msg_level_ = buf[pt];
		pt += 1;
		//拷贝
		valid_time_.assign(16, 0);
		for (int i = 0; i < 16; i++)
		{
			valid_time_[i] = buf[i + pt];
		}
		valid_time_ = valid_time_.c_str();
		pt += 16;
		//拷贝
		at_time_.assign(16, 0);
		for (int i = 0; i < 16; i++)
		{
			at_time_[i] = buf[i + pt];
		}
		at_time_ = at_time_.c_str();
		pt += 16;
		//拷贝
		report_flag_ = buf[pt];
		pt += 1;

		//拷贝
		tp_pid_ = buf[pt];
		pt += 1;

		//拷贝
		tp_udhi_ = buf[pt];
		pt += 1;

		//拷贝
		msg_fmt_ = buf[pt];
		pt += 1;

		//拷贝
		message_type_ = buf[pt];
		pt += 1;

		//拷贝短信内容长度
		char temp_char_message_length[4];
		for (int i = 0; i < 4; i++)
		{
			temp_char_message_length[i] = buf[i + pt];
		}
		memcpy(&message_length_, temp_char_message_length, 4);
		message_length_ = ntohl(message_length_);
		pt += 4;

		//长短信处理-------------------------------------------------
		auto ptr = buf.begin() + pt;
		int min = 0;
		if (tp_udhi_ == 1)
		{
			//byte 1 : 05, 表示剩余协议头的长度
			min = (uint32_t)(uint8_t)(*ptr) + 1;
			++ptr;
			//byte 2 : 00, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为1（格式中的XX值）。
			//byte 2 : 08, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为2（格式中的XX值）。
			uint8_t c = *ptr;
			++ptr;
			++ptr;
			//byte 2 : 00, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为1（格式中的XX值）。
			if (c == 0)
			{
				long_msg_id_ = (uint32_t)(uint8_t)(*ptr);
				++ptr;
			}
			//byte 2 : 08, 这个值在GSM 03.40规范9.2.3.24.1中规定，表示随后的这批超长短信的标识位长度为2（格式中的XX值）。
			else if (c == 8)
			{
				uint16_t d = 0;
				memcpy_s(&d, 2, &(*ptr), 2);
				ptr += 2;
				long_msg_id_ = (uint32_t)d;
			}
			else
			{
				throw smproxy::exception("长短信内容头格式错误", 1);
			}
			same_msg_total_ = (uint8_t)*ptr;
			++ptr;
			same_msg_num_ = (uint8_t)*ptr;
			++ptr;
		}
		//拷贝短信内容
		switch (msg_fmt_)
		{
		case 8:
		{
			int nDstLength;        // UNICODE宽字符数目
			WCHAR wchar[200] = { 0 };  // UNICODE串缓冲区
			std::wstring wstr(400, L'\0');
			// 高低字节对调，拼成UNICODE
			for (int i = 0; i < (message_length_ - min) / 2; i++)
			{
				wstr[i] = (uint8_t)*ptr++ << 8;    // 先高位字节
				wstr[i] |= (uint8_t)*ptr++;        // 后低位字节
			}
			wstr = wstr.c_str();
			// UNICODE串-->字符串
			message_content_ = boost::locale::conv::from_utf<wchar_t>(wstr, "GBK");
		}
		break;
		case 15:
			message_content_.assign(ptr, ptr + message_length_ - min);
			break;
		default:
			throw smproxy::exception("未识别编码号", 9);
			break;
		}


		pt += message_length_;
		pt += 8;

	}
	virtual ~SGIP12Submit(){};
	SGIP12Head hd;
};
