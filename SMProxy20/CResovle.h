#pragma once
#include "SMProxy2.h"
#include "IBaseParam.hpp"
#include <boost\shared_ptr.hpp>

#include "IHead.hpp"
#include "ISubmit.hpp"
#include "IBind.hpp"
#include "IResp.hpp"
#include "IDeliver.hpp"
#include "IReport.hpp"

//#include "CMPP20Connect.hpp"
//#include "CMPP20Submit.hpp"
//#include "CMPP20Deliver.hpp"
//
//
//#include "SGIP12Deliver.hpp"
//#include "SGIP12Report.hpp"
//#include "SGIP12Submit.hpp"
//#include "SGIP12Bind.hpp"
//
//#include "SMGP303Login.hpp"
//#include "SMGP303Submit.hpp"
//#include "SMGP303Deliver.hpp"
//
//
//#include "SGIP12Resp.hpp"
//#include "CMPP20Resp.hpp"
//#include "SMGP303Resp.hpp"
/*************************************************
Author:zr
Date:2018-01-18
Description:解析 字符串
**************************************************/




namespace smproxy {
	//解析 字符串
	class CResolve
	{
	public:
		CResolve(
			Protocol protocol,
			std::string sp_id
		);
		~CResolve() {}

		//解析数据
		void resolveBuf(bytes& buf);

		//生成Submit数据
		bytes submitBytes(
			int& serial_numb,
			const char* src_ID,
			std::vector<std::string> &user_num,
			std::vector<uint8_t>& message_content
		);

		//生成login数据
		bytes loginBytes(
			int& serial_numb,
			const char* password,
			int login_type
		);

		//生成Deliver_Resp数据
		bytes deliverRespBytes(std::string serial_numb, int status, std::string msg_id);

		//生成active_Resp数据
		bytes activeRespBytes(std::string serial_numb, int status);

		bytes activeBytes(int& serial_numb);
		/*void recvHead(std::vector<uint8_t> &buf)
		{
			ch->recvHead(buf);
			switch (protocol_)
			{
			case SGIP12:
				switch (ch->getCmdID())
				{
				case 1:
					cmd_id = Bind;
					break;
				case 0x80000001:
					cmd_id = BindResp;
					break;
				case 2:
					cmd_id = UnBind;
					break;
				case 0x80000002:
					cmd_id = UnBindResp;
					break;
				case 3:
					cmd_id = Submit;
					break;
				case 0x80000003:
					cmd_id = SubmitResp;
					break;
				case 4:
					cmd_id = Deliver;
					break;
				case 0x80000004:
					cmd_id = DeliverResp;
					break;
				case 5:
					cmd_id = Report;
					break;
				case 0x80000005:
					cmd_id = ReportResp;
					break;
				default:
					throw smproxy::exception("[CResovle.cpp]错误指令号：" + std::to_string(ch->getCmdID()), 7);
				}
				break;
			case SMGP303:
				switch (ch->getCmdID())
				{
				case 1:
					cmd_id = Bind;
					break;
				case 0x80000001:
					cmd_id = BindResp;
					break;
				case 2:
					cmd_id = Submit;
					break;
				case 0x80000002:
					cmd_id = SubmitResp;
					break;
				case 3:
					cmd_id = Deliver;
					break;
				case 0x80000003:
					cmd_id = DeliverResp;
					break;
				case 4:
					cmd_id = ActiveTest;
					break;
				case 0x80000004:
					cmd_id = ActiveTestResp;
					break;
				case 6:
					cmd_id = UnBind;
					break;
				case 0x80000006:
					cmd_id = UnBindResp;
					break;
				default:
					throw smproxy::exception("[CResovle.cpp]错误指令号：" + std::to_string(ch->getCmdID()), 2);
				}
				break;
			case CMPP20:
				switch (ch->getCmdID())
				{
				case 1:
					cmd_id = Bind;
					break;
				case 0x80000001:
					cmd_id = BindResp;
					break;
				case 2:
					cmd_id = UnBind;
					break;
				case 0x80000002:
					cmd_id = UnBindResp;
					break;
				case 4:
					cmd_id = Submit;
					break;
				case 0x80000004:
					cmd_id = SubmitResp;
					break;
				case 5:
					cmd_id = Deliver;
					break;
				case 0x80000005:
					cmd_id = DeliverResp;
					break;
				case 8:
					cmd_id = ActiveTest;
					break;
				case 0x80000008:
					cmd_id = ActiveTestResp;
					break;
				default:
					throw smproxy::exception("[CResovle.cpp]错误指令号：" + std::to_string(ch->getCmdID()), 2);
				}
				break;
			}
			std::string spid_temp = ch->getSPID();
			if (spid_temp.size() == 0)
				spid_temp = "0";
			SN1 = std::stoll(spid_temp);
			SN2 = ch->getSN2();
			SN3 = ch->getIntSerialNumb();
			serial_numb = ch->getSerialNumb();
		}*/
		/*void recvBody(std::vector<uint8_t> &buf)
		{
			switch (cmd_id)
			{
			case Bind://收到Bind请求
				cb->recvBind(buf);
				username = cb->getName();
				password_ = cb->getPass();
				time_stamp = cb->getTimestamp();
				break;
			case BindResp://收到Bind回应
			{
				resp_ = cr->recvResp(buf);
				authenticator_source_ = cr->getRespInstance()->getAuthenticatorSource();
				shared_secret_ = cr->getRespInstance()->getSharedSecret();
			}
			break;
			case UnBind:
				break;
			case UnBindResp:
				resp_ = cr->recvResp(buf);
				break;
			case SubmitResp://收到submit回应
				resp_ = cr->recvResp(buf);
				ISMG_id_ = cr->getRespInstance()->getISMG_id();
				break;
			case Submit://收到submit请求
				cs->recvSubmit(buf);
				msg_id_this_ = cs->getMsgId();
				msg_id = cs->getMsgId();
				ph_nums = *(cs->getPhNums());
				msg_text = *(cs->getText());
				pk_total = cs->getSame_msg_total();
				pk_number = cs->getSame_msg_num();

				TP_udhi = cs->getTP_udhi();
				same_msg_total = cs->getSame_msg_total();
				same_msg_num = cs->getSame_msg_num();
				long_msg_id = cs->getLong_msg_id();
				extnum_ = cs->getExtnum();
				break;
			case Deliver://收到上行短信
				break;
			case DeliverResp://收到上行短信 回应
				dlv->recvDeliverResp(buf);
				msg_id_this_ = dlv->getMsgId();
				result = dlv->getResp_result();
				break;
			case Report://收到短信报告
				break;
			case ReportResp://收到短信报告 回应

				rp->recvReportResp(buf);
				msg_id_this_ = rp->getMsgId();
				result = rp->getResp_result();
				break;
			case ActiveTest://收到心跳检测
				break;
			case ActiveTestResp://收到心跳检测 回应
				break;
			default:
				smproxy::exception k("错误指令ID");
				k.add("[解析字符串]");
				throw k;
			}
		}*/

		Command getCmdId() {
			return cmd_id_;
		};
		//用户名
		std::string& getUsr() { return username; };
		//密码
		std::string& getPassWord() {
			return password_;
		};
		//获取时间戳
		unsigned int& getTS() {
			return time_stamp;
		};
		unsigned int& getSN1() {
			return SN1;
		};
		unsigned int& getSN2() {
			return SN2;
		};
		unsigned int& getSN3() {
			return SN3;
		};
		int getErrCode()
		{
			return err_code_;
		}
		std::string getSerial_numb() {
			return hd->getSerialNumb();
		};
		//发送至电话号码
		std::vector<std::string>& getPhNums() { return ph_nums; };

		//一条msg_id对应几条submit
		int getPk_total() {
			return pk_total;
		};
		//一条msg_id的第几条submit
		int getPk_number() {
			return pk_number;
		};
		//长短信总条数
		int getSame_msg_total() { return same_msg_total; };
		//长短信第几条
		int getSame_msg_num() { return same_msg_num; };
		//长短信总体短信标识
		int getLong_msg_id() {
			return long_msg_id;
		};
		int getRespStatus() {
			return rsp->getStatus();
		};

		std::string getISMGIdFromSubmitResp()
		{
			return rsp->getISMGId();
		}
		std::string getDestId()
		{
			return dest_id_;
		}

		std::string getMsgId() {
			return msg_id_;
		}
		std::string getMsgIdThis() {
			return msg_id_this_;
		};

		int getTPPid()
		{
			return dlv->getTpPid();
		}

		int getTPUhdi()
		{
			return dlv->getTpUdhi();
		}

		int getMsgFmt() { return dlv->getMsgFmt(); }

		bytes getMsgContent() { return message_content_; };

		int getMsgLen() { return msg_len_; };

		void setMsg_id(std::string msg_id) {
			this->msg_id_ = msg_id;
		}
		void setResult(uint8_t result) {
			this->result = result;
		}
		uint8_t getResult() {
			return result;
		};

		std::string getExtnum() {
			return extnum_;
		};
		std::string getSrcTerminalId()
		{
			return src_terminal_id_;
		}
		void setSpId(std::string sp_id) {
			sp_ID_ = sp_id;
		}
		std::string getSpId() {
			return sp_ID_;
		};

		std::string getReportStat()
		{
			return rp->getStat();
		}
		std::string getISMGId()
		{
			return ISMG_id_;
		}
	private:
		Protocol protocol_;
		std::string msg_id_;
		std::string msg_id_this_;
		uint8_t result;
		//指令号 1：连接 2：断开 3：提交信息 4:上行短信 5:短信报告 8:心跳检测
		Command cmd_id_;
		std::string username;
		std::string password_;

		std::string src_terminal_id_;
		std::string dest_id_;
	


		unsigned int time_stamp;
		unsigned int SN1;
		unsigned int SN2;
		unsigned int SN3;
		std::vector<std::string> ph_nums;


		bytes message_content_;
		int msg_len_;
		unsigned int resp_;
		size_t total_len;//消息总长度
		std::string serial_numb; //消息流水号, 顺序累加, 步长为1, 循环使用（一对请求和应答消息的流水号必须相同）
								 //std::string source_addr;//源地址，为SP_Id，即SP的企业代码。cmpp 6字节

		int err_code_;
		int pk_total;
		int pk_number;
		int TP_udhi;

		int same_msg_total;
		int same_msg_num;
		int long_msg_id; //长短信总体短信标识

		//CMPP2.0 connect ——————

		std::string authenticator_source_;
		std::string shared_secret_;

		std::string ISMG_id_;//网关id

		std::string extnum_;//扩展码


		std::string sp_ID_;
		//接口
		boost::shared_ptr<IHead> hd;
		boost::shared_ptr<IBind> bd;
		boost::shared_ptr<ISubmit> sb;
		boost::shared_ptr<IResp> rsp;
		boost::shared_ptr<IDeliver> dlv;
		boost::shared_ptr<IReport> rp;
		bytes buf_;
	};

}//namespace smproxy