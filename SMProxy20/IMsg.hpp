#pragma once
/*************************************************
Author:zr
Date:2018-03-5
Description:消息体接口
**************************************************/


#include <string>
#include <vector>
#include "IHead.hpp"

class IMsg
{
public:
	IMsg(): 
		ISMG_id_(6),
		msg_id_(30),
		msg_id_this_(30),
		sp_number_(21),
		resp_result_(4)
	{
		ISMG_id_ = "0";
		msg_id_ = "0";
		msg_id_this_ = "0";
		sp_number_ = "0";
		resp_result_ = 99;
	};

	/// <summary>
	/// Gets the MSG identifier.
	/// </summary>
	/// <returns>
	/// string
	/// </returns>
	std::string getMsgId() {
		return msg_id_.get();
	}
	/// <summary>
	/// Gets the MSG identifier this.
	/// </summary>
	/// <returns></returns>
	std::string getMsgIdThis() {
		return msg_id_this_.get();
	}
	uint32_t getResp_result() {
		return resp_result_.get();
	}
	/// <summary>
	/// Sets the MSG identifier.
	/// </summary>
	/// <param name="msg_id">The MSG identifier.</param>
	void setMsgId(std::string  msg_id)
	{
		this->msg_id_ = msg_id;
	}
	/// <summary>
	/// Sets the MSG identifier this.
	/// </summary>
	/// <param name="msg_id_this">The MSG identifier this.</param>
	void setMsgIdThis(std::string  msg_id_this)
	{
		this->msg_id_this_ = msg_id_this;
	}


	// Sets the serial numb.
	//	放入指令流水号字符串，原样返回
	void setSerialNumb(std::string serial_numb)
	{
		hd_->setSerialNumb(serial_numb);
	}


	// Sets the serial numb.
	//放入指令流水号计数，按协议生成下一个流水号
	void setSerialNumb(uint32_t serial_numb)
	{
		hd_->setSerialNumb(hd_->createSerualNum(serial_numb));
	}

	/// <summary>
	/// Gets the buf.
	/// </summary>
	/// <returns></returns>
	std::vector<uint8_t> getBuf() { return buf_; };


	//Sets the spid.
	//传入发来的SPID 即SN1
	void setSPID(std::string SPID) {
		hd_->setSPID(SPID);
	};
	std::string getSpNumber() {
		return sp_number_.get();
	};

	void setSpNumber(std::string sp_number) {
		sp_number_ = sp_number;
	};

	void setHeadType(IHead* hd)
	{
		hd_ = hd;
	}
	/// <summary>
	/// Adds the head.
	/// </summary>
	virtual void addHead() {
		hd_->header(buf_.size(), hd_->getCmdID(), hd_->getSerialNumb());
		size_t head_len = hd_->getBuf().size();
		hd_->header(head_len + buf_.size(), hd_->getCmdID(), hd_->getSerialNumb());
		buf_.insert(buf_.begin(), hd_->getBuf().begin(), hd_->getBuf().end());
	};
	virtual std::string getISMG_id() {
		return ISMG_id_.get();
	}
	
	virtual void setISMG_id(std::string ISMG_id)
	{
		this->ISMG_id_ = ISMG_id;
	}
	virtual ~IMsg(){};
protected:

	// The buf
	std::vector<uint8_t> buf_;

	IHead* hd_;

	smproxy::FieldStr ISMG_id_;
	smproxy::FieldStr msg_id_;
	smproxy::FieldStr msg_id_this_;
	smproxy::FieldStr sp_number_;
	smproxy::FieldInt resp_result_;

};
