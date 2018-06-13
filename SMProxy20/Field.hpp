#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Utils.hpp"
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include "exception.hpp"
#include <map>
#include <tuple>
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
			{
				//BOOST_THROW_EXCEPTION(exception("[Field超长,设定值：" + std::to_string(size) + "][输入值：" + str +"]"));
			}
				
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
			return str_.c_str();
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
			{
				std::string value_str;
				value_str.assign(value.begin(), value.end());
				BOOST_THROW_EXCEPTION(exception("[Field超长,设定值：" + std::to_string(size) + "][输入值：" + value_str + "]"));
			}
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
		FieldInt& operator =(const uint64_t& integer)//赋值运算符
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
	class FieldTLVs :
		public Field
	{
	public:
		FieldTLVs() :
			Field(4)
		{
		}
		~FieldTLVs() {};

		void addTLV(uint16_t tag, Field* value)
		{
			values_[tag] = value;
		}
		void read(bytes& buf, bytes::iterator& iter)
		{
			while (true)
			{
				if (iter == buf.end())
					return;
				if (iter + 1 == buf.end())
					return;
				bytes temp;
				uint16_t tag;
				temp.assign(iter, iter + 2);
				memcpy_s(&tag, 2, &(temp[0]), 2);
				tag = ntohs(tag);
				if (tag<1||tag>0x0012)
					BOOST_THROW_EXCEPTION(exception("错误的TLV tag"));
				iter += 2;
				temp.assign(iter, iter + 2);
				uint16_t len;
				memcpy_s(&len, 2, &(temp[0]), 2);
				len = ntohs(len);
				if (len == 0 || len > 60000)
				{
					BOOST_THROW_EXCEPTION(exception("错误的数值长度"));
				}
				iter += 2;
				auto lv = values_[tag];
				lv->setSize(len);
				lv->read(iter);
			}
			
		}
		bytes toBytes() override
		{
			auto it = values_.begin();
			bytes buf;
			while (it != values_.end())
			{
				bytes temp;
				temp.assign(4, 0);
				auto temp_tag = htons(it->first);
				memcpy_s(&(temp[0]), 2, &temp_tag, 2);
				auto temp_len = htons(uint16_t( it->second->size));
				memcpy_s(&(temp[2]), 2, &temp_len, 2);
				buf.insert(buf.end(), temp.begin(), temp.end());
				temp = it->second->toBytes();
				buf.insert(buf.end(), temp.begin(), temp.end());

				it++;
			}
			
			return buf;
		}
	protected:

		//tag:(len, value_ptr)
		std::map<uint16_t, Field*> values_;
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

