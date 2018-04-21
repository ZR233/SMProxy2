#pragma once
#include "ISubmit.hpp"
#include "CMPP20Head.hpp"

/*************************************************
Author:zr
Date:2018-01-17
Description:CMPP submit处理
**************************************************/

namespace smproxy {
	/**
	* \brief 用于递交短信
	*/
	class CMPP20Submit :
		public ISubmit
	{
	private:
		Buffer body_;
	public:
		CMPP20Submit() :ISubmit()
		{
			body_.pushField(msg_id_.set(8,0));
			body_.pushField(pk_total_.setSize(1));
			body_.pushField(pk_number_.setSize(1));
			body_.pushField(report_flag_.set(1,1));
			body_.pushField(msg_level_.setSize(1));
			body_.pushField(service_id_.set(10,"0"));
			body_.pushField(fee_usertype_.set(1,2));
			body_.pushField(fee_number_.setSize(21));
			body_.pushField(tp_pid_.set(1,0));
			body_.pushField(tp_udhi_.set(1,0));
			body_.pushField(msg_fmt_.set(1,0));
			body_.pushField(sp_ID_.set(6,"0"));
			body_.pushField(fee_type_.setSize(2));
			body_.pushField(fee_code_.setSize(6));
			body_.pushField(valid_time_.setSize(17));
			body_.pushField(at_time_.setSize(17));
			body_.pushField(src_ID_.set(21,"0"));
			body_.pushField(dest_total_.setSize(1));

		}
		virtual ~CMPP20Submit() {};

		//接收submit
		//@buf 待解析数据
		//@buf_loc 解析起始位置迭代器
		void recvSubmit(bytes &buf, bytes::iterator buf_loc) override {
			auto iter = body_.reslov(buf, buf_loc);
			if (buf.size() < (116 + 21 * dest_total_.get() + 1))
			{
				BOOST_THROW_EXCEPTION(exception("submit消息长度错误", 4));
			}
			//拷贝手机号
			if (dest_total_.get() >100)
			{
				throw smproxy::exception("手机号过多", 9);
			}
			dest_term_IDs_.clear();
			for (auto i = 0; i <dest_total_.get(); i++)
			{
				std::string tempstr;
				tempstr.assign(iter, iter + 21);
				tempstr = tempstr.c_str();
				dest_term_IDs_.push_back(tempstr);
				iter += 21;
			}
			message_length_ = uint32_t(*iter);
			if (message_length_.get()>140)
			{
				BOOST_THROW_EXCEPTION(exception("短信内容过长", 6));
			}
			++iter;
			if (buf.size() < (116 + 21 * dest_total_.get() + 1 + message_length_.get()))
			{
				BOOST_THROW_EXCEPTION(exception("submit消息长度错误", 1));
			}

			//长短信处理-------------------------------------------------

			int min = 0;
			if (tp_udhi_.get() == 1)
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
					throw smproxy::exception("长短信内容头格式错误", 1);
				}
				same_msg_total_ = (uint8_t)*iter;
				++iter;
				same_msg_num_ = (uint8_t)*iter;
				++iter;
			}
			//拷贝信息内容-----------------------------------
			message_content_.assign(iter, iter + message_length_.get() - min);
			//switch (msg_fmt_.get())
			//{
			//case 8:
			//case 9:
			//{

			//	int nDstLength;        // UNICODE宽字符数目
			//	WCHAR wchar[200] = { 0 };  // UNICODE串缓冲区
			//	std::wstring wstr(400, L'\0');
			//	// 高低字节对调，拼成UNICODE
			//	for (int i = 0; i < (message_length_.get() - min) / 2; i++)
			//	{
			//		wstr[i] = uint16_t(*iter++) << 8;    // 先高位字节
			//		wstr[i] |= *iter++;        // 后低位字节
			//	}
			//	wstr = wstr.c_str();
			//	// UNICODE串-->字符串
			//	message_content_ = boost::locale::conv::from_utf<wchar_t>(wstr, "GBK");
			//}
			//break;
			//case 15:
			//	message_content_.assign(iter, iter + message_length_ - min);
			//	break;
			//default:
			//	throw smproxy::exception("未识别编码号", 9);
			//}
		}

		const bytes& submit(
			char* src_ID,
			std::vector<std::string> &user_num,
			int msg_fmt,
			std::vector<uint8_t>& message_content,
			int pk_total = 0,
			int pk_number = 0,
			int tp_pid = 0,
			int tp_udhi = 0,
			int same_msg_total = 0,
			int same_msg_num = 0,
			int long_msg_id = 0
		)
		{
			src_ID_.set(src_ID);
			msg_fmt_ = msg_fmt;
			pk_total_ = pk_total;
			tp_pid_ = tp_pid;
			tp_udhi_ = tp_udhi;
			same_msg_total_ = same_msg_total;
			same_msg_num_ = same_msg_num;
			long_msg_id_ = long_msg_id;
			dest_total_ = user_num.size();
			message_length_ = message_content.size();
			buf_ = body_.to_bytes();

			//拷贝手机号----------
			if (dest_total_.get() > 100)
			{
				BOOST_THROW_EXCEPTION(exception("submit 手机号超过100个"));
			}
			for (int i = 0; i < user_num.size(); i++)
			{
				bytes temp_ph;
				temp_ph.assign(user_num[i].begin(), user_num[i].end());
				temp_ph.resize(21);
				buf_.insert(buf_.end(), temp_ph.begin(), temp_ph.end());
			}

			//拷贝信息
			if (message_length_.get() > 140)
			{
				BOOST_THROW_EXCEPTION(exception("submit 短信内容超过140"));
			}

			buf_.push_back(uint8_t(message_length_.get()));
			buf_.insert(buf_.end(), message_content.begin(), message_content.end());
			for (size_t i = 0; i < 8; i++)
			{
				buf_.push_back('\0');
			}
		}
	};


}

