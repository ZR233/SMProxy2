#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Utils.hpp"
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include "exception.hpp"
/*************************************************
Author:zr
Date:2018-03-15
Description:field element
**************************************************/


namespace smproxy
{
	enum TimeType{
		YYYYMMDDhhmmss = 1,
		YYMMDDhhmmss,
		YYMMDDhhmm,
		MMDDhhmmss,
		MMDDhhmm
	};
	typedef std::vector<uint8_t> bytes;
	class Field
	{
	public:
		Field( int size) :
			size(size)
		{
		};
		virtual ~Field() {};
		virtual void read(bytes::iterator &iter) = 0;
		virtual bytes toBytes() = 0;
		Field& setSize(int size)
		{
			this->size = size;
			return *this;
		}
		int size;
	};
	//octet string
	class FieldStr:
		public Field
	{
	public:
		FieldStr() :
			Field(1)
		{
		}
		FieldStr(int size) :
			Field(size)
		{
		}
		FieldStr(int size, const char* value) :
			Field(size)
		{
			str_ = value;
		}
		~FieldStr() {};
		FieldStr& operator =(const std::string& str)//赋值运算符
		{
			set(str);
			return *this;
		}
		FieldStr& setSize(int size)
		{
			this->size = size;
			return *this;
		}

		FieldStr& set(std::string str)
		{
			if (str.size() > size)
				BOOST_THROW_EXCEPTION(exception("Field超长"));
			this->str_ = str;
			this->str_.resize(size);
			return *this;
		}

		FieldStr& set(int size, std::string str)
		{
			this->size = size;
			set(str);
			return *this;
		}
		std::string get()
		{
			return str_;
		}
		void read(bytes::iterator &iter) override
		{
			str_.assign(iter, iter + size);
			str_ = str_.c_str();
			iter += size;
		}
		bytes toBytes() override
		{
			temp.assign(str_.begin(), str_.end());
			temp.resize(size);
			return temp;
		}
	protected:
		std::string str_;
		bytes temp;
	};
	class FieldBytes :
		public Field
	{
	public:
		FieldBytes() :
			Field(1)
		{
		}
		FieldBytes(int size) :
			Field(size)
		{
		}
		FieldBytes(int size, const char value) :
			Field(size)
		{
			value_.assign(size,value);
		}
		~FieldBytes() {};
		FieldBytes& operator =(bytes& value)//赋值运算符
		{
			set(value);
			return *this;
		}
		FieldBytes& setSize(int size)
		{
			this->size = size;
			return *this;
		}

		FieldBytes& set(bytes& value)
		{
			if (value.size() > size)
				BOOST_THROW_EXCEPTION(exception("Field超长"));
			this->value_ = value;
			this->value_.resize(size);
			return *this;
		}

		FieldBytes& set(int size, const char value)
		{
			this->size = size;
			value_.assign(size, value);
			return *this;
		}
		bytes& get()
		{
			return value_;
		}
		void read(bytes::iterator &iter) override
		{
			value_.assign(iter, iter + size);
			iter += size;
		}
		bytes toBytes() override
		{
			value_.resize(size);
			return value_;
		}
	protected:
		bytes value_;
		bytes temp;
	};//FieldBytes
	//Integer
	class FieldInt :
		public Field
	{
	public:
		FieldInt() :
			Field(4)
		{
		}
		FieldInt(int size) :
			Field(size)
		{
		}
		FieldInt( int size, uint64_t value) :
			Field(size)
		{
			this->integer_ = value;
		}
		~FieldInt() {};
		FieldInt& operator =(const uint32_t& integer)//赋值运算符
		{
			set(integer);
			return *this;
		}
		FieldInt& setSize(int size)
		{
			this->size = size;
			return *this;
		}
		FieldInt& set(uint64_t integer)
		{
			this->integer_ = integer;
			return *this;
		}
		FieldInt& set(int size, uint64_t integer)
		{
			this->size = size;
			this->integer_ = integer;
			return *this;
		}
		uint64_t get() 
		{
			return integer_;
		}
		void read(bytes::iterator &iter) override
		{
			temp.assign(iter, iter + size);
			switch (size)
			{
			case 1:
				integer_ = uint64_t(temp[0]);
				break;
			case 2:
				{
					uint16_t temp_int;
					memcpy_s(&temp_int, 2, &(temp[0]), 2);
					integer_ = size_t(ntohs(temp_int));
				}
				break;
			case 4:
			{
				uint32_t temp_int;
				memcpy_s(&temp_int, 4, &(temp[0]), 4);
				integer_ = size_t(ntohl(temp_int));
			}
				break;
			case 8:
			{
				memcpy_s(&integer_, 8, &(temp[0]), 8);
				integer_ = Utils::ntohll(integer_);
			}
				break;
			default:
				BOOST_THROW_EXCEPTION(exception("错误的数值长度"));
			}
			iter += size;
		}
		bytes toBytes() override
		{
			temp.assign(size, 0);
			switch (size)
			{
			case 1:
				temp[0] = uint8_t(integer_);
				break;
			case 2:
			{
				auto temp_int = htons(uint16_t(integer_));
				memcpy_s(&(temp[0]), 2, &temp_int, 2);
			}
			break;
			case 4:
			{
				auto temp_int = htonl(uint32_t(integer_));
				memcpy_s(&(temp[0]), 4, &temp_int, 4);
			}
			break;
			case 8:
			{
				auto temp_int = Utils::htonll(integer_);
				memcpy_s(&(temp[0]), 8, &temp_int, 8);
			}
			break;
			default:
				BOOST_THROW_EXCEPTION(exception("错误的数值长度"));
			}
			return temp;
		}
	protected:
		uint64_t integer_;
		bytes temp;
	};

	//TLV
	class FieldTLV :
		public Field
	{
	public:
		FieldTLV(uint16_t tag, uint16_t len = 0) :
			Field(4 + len)
		{
			setTag(tag,len);
		}
		~FieldTLV() {};

		void setTag(uint16_t tag, uint16_t len = 0)
		{
			this->tag_ = tag;
			switch (tag_)
			{
			case 0x0001://TP_pid
			case 0x0002://TP_udhi
			case 0x0004://ChargeUserType
			case 0x0005://ChargeTermType
			case 0x0007://DestTermType
			case 0x0009://PkTotal
			case 0x000A://PkNumber
			case 0x000B://SubmitMsgType
			case 0x000C://SPDealReslt
			case 0x000D://SrcTermType
			case 0x000F://NodesCount
			case 0x0011://SrcType
				value_ptr_ = boost::make_shared<FieldInt>(FieldInt(1));
				len_ = 1;
				break;
			case 0x0003://LinkID
				value_ptr_ = boost::make_shared<FieldStr>(FieldStr(20));
				len_ = 20;
				break;
			case 0x0006://ChargeTermPseudo
			case 0x0008://DestTermPseudo
			case 0x000E://SrcTermPseudo
				value_ptr_ = boost::make_shared<FieldStr>(FieldStr(len));
				len_ = len;
				break;
			case 0x0010://MsgSrc
				value_ptr_ = boost::make_shared<FieldStr>(FieldStr(8));
				len_ = 8;
				break;
			case 0x0012://MServiceID
				value_ptr_ = boost::make_shared<FieldStr>(FieldStr(21));
				len_ = 21;
				break;
			default:
				BOOST_THROW_EXCEPTION(exception("错误的TLV可选参数标签"));
				break;
			}
			size = 4 + len_;
		}
		boost::shared_ptr< Field> Val()
		{
			return value_ptr_;
		}
		uint16_t getTag()
		{
			return tag_;
		}
		void read(bytes::iterator &iter) override
		{
			bytes temp;
			temp.assign(iter, iter + 2);
			memcpy_s(&tag_, 2, &(temp[0]), 2);
			tag_ = ntohs(tag_);
			iter += 2;
			temp.assign(iter, iter + 2);
			memcpy_s(&len_, 2, &(temp[0]), 2);
			len_ = ntohs(len_);
			if (len_  == 0 || len_ > 60000)
			{
				BOOST_THROW_EXCEPTION(exception("错误的数值长度"));
			}
			iter += 2;
			setTag(tag_, len_);
			value_ptr_->read(iter);
		}
		bytes toBytes() override
		{
			temp.assign(4, 0);
			auto temp_tag = htons(tag_);
			memcpy_s(&(temp[0]), 2, &temp_tag, 2);
			auto temp_len = htons(len_);
			memcpy_s(&(temp[2]), 2, &temp_len, 2);
			auto temp_value = value_ptr_->toBytes();
			temp.insert(temp.end(), temp_value.begin(), temp_value.end());
			return temp;
		}
	protected:
		uint16_t tag_;
		uint16_t len_;
		boost::shared_ptr< Field> value_ptr_;
		bytes temp;
	};
	class Buffer
	{
	public:
		Buffer() 
		{
		};
		~Buffer() {};
		void pushField(Field &field)
		{
			field_squ.push_back(&field);
		}
		void pushBytes(bytes &bytes_)
		{
			this->bytes_.insert(this->bytes_.end(), bytes_.begin(), bytes_.end());
		}
		const std::vector<uint8_t>& to_bytes()
		{
			bytes_.clear();
			for (Field* field : field_squ)
			{
				auto temp = field->toBytes();
				bytes_.insert(bytes_.end(), temp.begin(), temp.end());
			}
			return  bytes_;
		}
		//解析数据串
		//返回解析后的指针位置
		bytes::iterator reslov(bytes& buf, bytes::iterator iter)
		{
			for (auto field_ptr : field_squ)
			{
				if (iter + field_ptr->size != buf.end())
				{
					field_ptr->read(iter);
				}
				else
				{
					BOOST_THROW_EXCEPTION(exception("数据串长度不足"));
				}
			}
			return iter;
		}
	private:
		std::vector<uint8_t> bytes_;
		std::vector<Field*> field_squ;
	};
}

