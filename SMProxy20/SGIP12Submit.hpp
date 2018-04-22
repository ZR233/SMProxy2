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
		//����SP����� 
		sp_number_.resize(21);
		buf_.assign(sp_number_.begin(), sp_number_.end());
		pt += 21;
		
		//�������Ѻ���
		charge_number_.resize(21);
		buf_.insert(buf_.end(), charge_number_.begin(), charge_number_.end());
		pt += 21;
		//�������ն���Ϣ���ֻ�������
		dest_total_ = user_num.size();
		buf_.push_back(dest_total_);
		pt += 1;
		//�������ն���Ϣ���ֻ���
		for (int i = 0; i < user_num.size(); i++)
		{
			bytes temp_ph;
			temp_ph.assign(user_num[i].begin(), user_num[i].end());
			temp_ph.resize(21);
			buf_.insert(buf_.end(), temp_ph.begin(), temp_ph.end());
			pt += 21;
		}
		//������ҵ����
		SPID.resize(5);
		buf_.insert(buf_.end(), SPID.begin(), SPID.end());
		pt += 5;
		//����ҵ����룬��SP����
		service_id_.resize(10);
		buf_.insert(buf_.end(), service_id_.begin(), service_id_.end());
		pt += 10;

		//�����Ʒ�����
		buf_.push_back((unsigned char)std::stoi(fee_type_));
		pt += 1;

		//����ȡֵ��Χ0-99999����������Ϣ���շ�ֵ����λΪ�֣���SP����
		fee_value_.resize(6);
		buf_.insert(buf_.end(), fee_value_.begin(), fee_value_.end());
		pt += 6;
		//����ȡֵ��Χ0-99999�������û��Ļ��ѣ���λΪ�֣���SP���壬��ָ��SP���û����͹��ʱ�����ͻ���
		given_value_.resize(6);
		buf_.insert(buf_.end(), given_value_.begin(), given_value_.end());
		pt += 6;

		//����
		buf_.push_back(agent_flag_);
		pt += 1;
		//����
		buf_.push_back(morelateto_MT_flag_);
		pt += 1;
		//����
		buf_.push_back(msg_level_);
		pt += 1;
		//����
		valid_time_.resize(16);
		buf_.insert(buf_.end(), valid_time_.begin(), valid_time_.end());
		pt += 16;
		//����
		at_time_.resize(16);
		buf_.insert(buf_.end(), at_time_.begin(), at_time_.end());
		pt += 16;
		//����
		buf_.push_back(report_flag_);
		pt += 1;
		//����
		buf_.push_back(tp_pid_);
		pt += 1;
		//����
		buf_.push_back(tp_udhi_);
		pt += 1;
		//����
		buf_.push_back(msg_fmt_);
		pt += 1;
		//����
		buf_.push_back(message_type_);
		pt += 1;
		//�����������ݳ���
		uint8_t temp_char_message_length[4];
		unsigned int temp_ml = htonl(msg.size());
		memcpy(temp_char_message_length, &temp_ml, 4);
		buf_.insert(buf_.end(), &temp_char_message_length[0], &temp_char_message_length[4]);
		pt += 4;
		//������������
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
			throw std::runtime_error("submit ��Ϣ���ȴ���");
		}
		int pt = 20;

		//����SP����� 
		sp_number_.assign(21, 0);
		for (int i = 0; i < 21; i++)
		{
			sp_number_[i] = buf[i + pt];
		}
		sp_number_ = sp_number_.c_str();
		pt += 21;
		extnum_ = sp_number_;
		//�������Ѻ���
		charge_number_.assign(21, 0);
		for (int i = 0; i < 21; i++)
		{
			charge_number_[i] = buf[i + pt];
		}
		charge_number_ = charge_number_.c_str();
		pt += 21;

		//�������ն���Ϣ���ֻ�������
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

		//������ҵ����
		SPID.assign(5, 0);
		for (int i = 0; i < 5; i++)
		{
			SPID[i] = buf[i + pt];
		}
		SPID = SPID.c_str();
		pt += 5;
		//����ҵ����룬��SP����
		service_type_.assign(10, 0);
		for (int i = 0; i < 10; i++)
		{
			service_type_[i] = buf[i + pt];
		}
		service_type_ = service_type_.c_str();
		pt += 10;

		//�����Ʒ�����
		fee_type_ = std::to_string(int(buf[pt]));
		pt += 1;

		//����ȡֵ��Χ0-99999����������Ϣ���շ�ֵ����λΪ�֣���SP����
		fee_value_.assign(6, 0);
		for (int i = 0; i < 6; i++)
		{
			fee_value_[i] = buf[i + pt];
		}
		fee_value_ = fee_value_.c_str();
		pt += 6;
		//����ȡֵ��Χ0-99999�������û��Ļ��ѣ���λΪ�֣���SP���壬��ָ��SP���û����͹��ʱ�����ͻ���
		given_value_.assign(6, 0);
		for (int i = 0; i < 6; i++)
		{
			given_value_[i] = buf[i + pt];
		}
		given_value_ = given_value_.c_str();
		pt += 6;

		//����
		agent_flag_ = buf[pt];
		pt += 1;
		//����
		morelateto_MT_flag_ = buf[pt];
		pt += 1;
		//����
		msg_level_ = buf[pt];
		pt += 1;
		//����
		valid_time_.assign(16, 0);
		for (int i = 0; i < 16; i++)
		{
			valid_time_[i] = buf[i + pt];
		}
		valid_time_ = valid_time_.c_str();
		pt += 16;
		//����
		at_time_.assign(16, 0);
		for (int i = 0; i < 16; i++)
		{
			at_time_[i] = buf[i + pt];
		}
		at_time_ = at_time_.c_str();
		pt += 16;
		//����
		report_flag_ = buf[pt];
		pt += 1;

		//����
		tp_pid_ = buf[pt];
		pt += 1;

		//����
		tp_udhi_ = buf[pt];
		pt += 1;

		//����
		msg_fmt_ = buf[pt];
		pt += 1;

		//����
		message_type_ = buf[pt];
		pt += 1;

		//�����������ݳ���
		char temp_char_message_length[4];
		for (int i = 0; i < 4; i++)
		{
			temp_char_message_length[i] = buf[i + pt];
		}
		memcpy(&message_length_, temp_char_message_length, 4);
		message_length_ = ntohl(message_length_);
		pt += 4;

		//�����Ŵ���-------------------------------------------------
		auto ptr = buf.begin() + pt;
		int min = 0;
		if (tp_udhi_ == 1)
		{
			//byte 1 : 05, ��ʾʣ��Э��ͷ�ĳ���
			min = (uint32_t)(uint8_t)(*ptr) + 1;
			++ptr;
			//byte 2 : 00, ���ֵ��GSM 03.40�淶9.2.3.24.1�й涨����ʾ���������������ŵı�ʶλ����Ϊ1����ʽ�е�XXֵ����
			//byte 2 : 08, ���ֵ��GSM 03.40�淶9.2.3.24.1�й涨����ʾ���������������ŵı�ʶλ����Ϊ2����ʽ�е�XXֵ����
			uint8_t c = *ptr;
			++ptr;
			++ptr;
			//byte 2 : 00, ���ֵ��GSM 03.40�淶9.2.3.24.1�й涨����ʾ���������������ŵı�ʶλ����Ϊ1����ʽ�е�XXֵ����
			if (c == 0)
			{
				long_msg_id_ = (uint32_t)(uint8_t)(*ptr);
				++ptr;
			}
			//byte 2 : 08, ���ֵ��GSM 03.40�淶9.2.3.24.1�й涨����ʾ���������������ŵı�ʶλ����Ϊ2����ʽ�е�XXֵ����
			else if (c == 8)
			{
				uint16_t d = 0;
				memcpy_s(&d, 2, &(*ptr), 2);
				ptr += 2;
				long_msg_id_ = (uint32_t)d;
			}
			else
			{
				throw smproxy::exception("����������ͷ��ʽ����", 1);
			}
			same_msg_total_ = (uint8_t)*ptr;
			++ptr;
			same_msg_num_ = (uint8_t)*ptr;
			++ptr;
		}
		//������������
		switch (msg_fmt_)
		{
		case 8:
		{
			int nDstLength;        // UNICODE���ַ���Ŀ
			WCHAR wchar[200] = { 0 };  // UNICODE��������
			std::wstring wstr(400, L'\0');
			// �ߵ��ֽڶԵ���ƴ��UNICODE
			for (int i = 0; i < (message_length_ - min) / 2; i++)
			{
				wstr[i] = (uint8_t)*ptr++ << 8;    // �ȸ�λ�ֽ�
				wstr[i] |= (uint8_t)*ptr++;        // ���λ�ֽ�
			}
			wstr = wstr.c_str();
			// UNICODE��-->�ַ���
			message_content_ = boost::locale::conv::from_utf<wchar_t>(wstr, "GBK");
		}
		break;
		case 15:
			message_content_.assign(ptr, ptr + message_length_ - min);
			break;
		default:
			throw smproxy::exception("δʶ������", 9);
			break;
		}


		pt += message_length_;
		pt += 8;

	}
	virtual ~SGIP12Submit(){};
	SGIP12Head hd;
};
