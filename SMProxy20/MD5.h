#pragma once
#include <string>
#include <vector>

/*************************************************
Author:zr
Date:2018-01-16
Description:MD5 ����
**************************************************/

class MD5 {
public:
	MD5() = default;
	~MD5() = default;
	/**
	* Same as 'md5sum' command, returned size is always 32.
	*/
	std::string md5sum(const void* dat, size_t len);

	/**
	* Return Calculated raw result(always little-endian), the size is always 16.
	* @out Output result.
	*/
	void md5bin(const void* dat, size_t len, unsigned char out[16]);

	/**
	* Generate shorter md5sum by something like base62
	* instead of base16 or base10. 0~61 are represented by 0-9a-zA-Z
	*/
	std::string md5sum6(const void* dat, size_t len);
};