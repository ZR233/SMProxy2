#pragma once
#include "IResp.hpp"
#include <sstream>
#include "Utils.hpp"
namespace smproxy {
	class CMPP20Resps:
		public IResp
	{
	private:
		FieldInt msg_id_int_;
		Buffer connect_buffer_;
		Buffer submit_buffer_;

	public:
		CMPP20Resps():
			msg_id_int_(8,0)
		{
			//---------connect-----------
			connect_buffer_.pushField(status_);
			connect_buffer_.pushField(authenticatorISMG_);
			connect_buffer_.pushField(version_);

			submit_buffer_.pushField(msg_id_int_);
			submit_buffer_.pushField(status_);

		}
		~CMPP20Resps() = default;
		void recvLoginResp(bytes& buf, bytes::iterator buf_loc) override
		{
			if (buf.size() < 30)
			{
				BOOST_THROW_EXCEPTION(exception("connect_resp字符串长度错误", 1));
			}
			connect_buffer_.reslov(buf, buf_loc);
		}//recvLoginResp

		void recvSubmitResp(bytes& buf, bytes::iterator buf_loc) override
		{
			if (buf.size() < 21)
			{
				BOOST_THROW_EXCEPTION(exception("submit_resp字符串长度错误", 1));
			}
			submit_buffer_.reslov(buf, buf_loc);
			ISMG_Id_ = std::to_string(Utils::getISMGFromCMPP20MsgID(msg_id_int_.get()));
			msg_id_ = std::to_string(msg_id_int_.get());
		}//recvSubmitResp

		void recvDeliverResp(bytes& buf, bytes::iterator buf_loc)override
		{

		}//recvDeliverResp

		bytes deliverRespBytes(int status, std::string msg_id) override
		{
			uint64_t msg_id_int;
			std::istringstream is(msg_id);
			is >> msg_id_int;
			msg_id_int_ = msg_id_int;
			status_ = status;
			auto buf = submit_buffer_.to_bytes();
			return buf;
		}

		void recvActiveResp(bytes& buf, bytes::iterator buf_loc) override {}
		bytes activeRespBytes(int status) override {
			return bytes();
		}
	};//CMPP20Resps
}//smproxy
