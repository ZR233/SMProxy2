#pragma once
/*************************************************
Author:zr
Date:2018-04-20
Description:公用信息接口
**************************************************/

#include <string>
#include <vector>
#include "Field.hpp"


namespace smproxy {
	class IBaseParam
	{
	protected:
		// The buf
		FieldStr ISMG_ID_;
		FieldStr sp_ID_;
		FieldStr serial_numb_;
		FieldInt int_serial_numb_;
		bytes buf_;
	public:
		IBaseParam():
			ISMG_ID_(6,"0"),
			sp_ID_(6,"0"),
			serial_numb_(10,"0"),
			int_serial_numb_(4,0)
		{
		};

		//Gets the ISMG identifier.
		std::string getISMGId() {
			return ISMG_ID_.get();
		}
		//Gets the SP identifier.
		std::string getSpId() {
			return sp_ID_.get();
		}
		std::string getSerialNumb() {
			return serial_numb_.get();
		}

		//Sets the ISMG identifier.
		void setISMGId(std::string  ISMG_id)
		{
			this->ISMG_ID_ = ISMG_id;
		}
		//Sets the SP identifier.
		void setSpId(std::string  sp_id)
		{
			this->sp_ID_ = sp_id;
		}

		void setSerialNumb(std::string serial_numb) {
			this->serial_numb_ = serial_numb;
		};

		//取缓存
		const bytes& getBuf() { return buf_; };



		virtual ~IBaseParam() {};
	};
}