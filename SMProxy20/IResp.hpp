#pragma once
#include "Field.hpp"


namespace smproxy {
	class IResp
	{
	protected:
		FieldInt status_;
		FieldBytes authenticatorISMG_;
		FieldInt version_;
		std::string msg_id_;
		std::string ISMG_Id_;
	public:
		IResp() :
			status_(1, 9),
			authenticatorISMG_(16),
			version_(1, 0)
		{

		};
		//virtual bytes BindResp(int status) = 0;
		virtual void recvLoginResp(bytes& buf, bytes::iterator buf_loc) = 0;
		virtual void recvSubmitResp(bytes& buf, bytes::iterator buf_loc) = 0;
		virtual void recvDeliverResp(bytes& buf, bytes::iterator buf_loc) = 0;
		virtual bytes deliverRespBytes(int status, std::string msg_id) = 0;
		virtual void recvActiveResp(bytes& buf, bytes::iterator buf_loc) = 0;
		virtual bytes activeRespBytes(int status) = 0;
		//-----------get/set-----------

		std::string getISMGId()
		{
			return ISMG_Id_;
		}


		int getStatus()
		{
			return int (status_.get());
		}

		bytes getAuthenticatorISMG()
		{
			return authenticatorISMG_.get();
		}

		int getVersion()
		{
			return int(version_.get());
		}

		std::string getMsgId()
		{
			return msg_id_;
		}
		virtual ~IResp() = default;
	};

}