/**
 *  $Id: string.cpp 5 2007-06-11 03:14:50Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the string class, as defined in string.h
**/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#include "string.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

namespace fire_engine
{

string::string()
	: mContent(0), mLength(0)
{
}

string::string(const c8 * str)
{
	mLength  = strlen(str);
	mContent = new c8[mLength+1];
#if defined(_MSC_VER)
	strcpy_s(mContent, mLength+1, str);
#else
	strncpy(mContent, str, mLength);
	mContent[mLength] = '\0';
#endif
}

string::string(const string& str)
{
	mLength = str.length();
	mContent = new c8[mLength+1];
#if defined(_MSC_VER)
	strcpy_s(mContent, mLength+1, str.c_str());
#else
	strncpy(mContent, str.c_str(), mLength);
	mContent[mLength] = '\0';
#endif
}

string::string(const c8 * str, s32 pre_calculated_length)
{
	mLength = pre_calculated_length;
	mContent = new c8[mLength+1];
#if defined(_MSC_VER)
	strcpy_s(mContent, mLength+1, str);
#else
	strncpy(mContent, str, mLength);
	mContent[mLength] = '\0';
#endif
}

string::string(c8 * str, bool keep_string)
{
	if (keep_string)
	{
		mContent = str;
		mLength = strlen(str);
	}
	else
	{
		mLength = strlen(str);
		mContent = new c8[mLength+1];
#if defined(_MSC_VER)
		strcpy_s(mContent, mLength+1, str);
#else
		strncpy(mContent, str, mLength);
		mContent[mLength] = '\0';
#endif
	}
}

string::~string()
{
	if (mContent != 0)
		delete [] mContent;
}

s32 string::length(void) const
{
	return mLength;
}

c8 string::at(s32 i) const
{
	if (i >= mLength)
		return (-1);
	return mContent[i];
}

s32 string::cmp(const string& other) const
{
	return strcmp(mContent, other.c_str());
}

s32 string::cmp(const c8 * other) const
{
	return strcmp(mContent, other);
}

const c8 * string::c_str(void) const
{
	return mContent;
}

const string& string::append(const string& other)
{
	*this += other;
	return *this;
}

const string& string::append(const c8 * other)
{
	*this += other;
	return *this;
}

bool string::isSubstring(const string& str) const
{
	return strstr(mContent, str.c_str()) != NULL;
}

bool string::isSubstring(const c8 * str) const
{
	return strstr(mContent, str) != NULL;
}

const c8 * string::substring(const c8 * str) const
{
	return strstr(mContent, str);
}

void string::replaceAll(c8 charA, c8 charB)
{
	c8 * buf = mContent;
	while (*buf != '\0')
	{
		if (*buf == charA)
		{
			*buf = charB;
		}
		buf++;
	}
}

void string::makeLower()
{
	for (s32 i = 0; i < mLength; i++)
		mContent[i] = tolower(mContent[i]);
}

bool string::equalsIgnoreCase(const string& str) const
{
	if (mLength != str.length())
		return false;
	for (s32 i = 0; i < mLength; i++)
		if (tolower(mContent[i]) != tolower(str.c_str()[i]))
			return false;
	return true;
}

bool string::equals(const string &l, const string &r)
{
	return l == r;
}

bool string::equalsIgnoreCase(const string& l, const string& r)
{
	return l.equalsIgnoreCase(r);
}

const string& string::operator=(const string& other)
{
	if (mContent != 0)
		delete [] mContent;
	mLength = other.length();
	mContent = new c8[mLength+1];
#if defined(_MSC_VER)
	strcpy_s(mContent, mLength+1, other.c_str());
#else
	strncpy(mContent, other.c_str(), mLength);
	mContent[mLength] = '\0';
#endif
	return *this;
}

const string& string::operator=(const c8 * other)
{
	if (mContent != 0)
		delete [] mContent;
	mLength = strlen(other);
	mContent = new c8[mLength+1];
#if defined(_MSC_VER)
	strcpy_s(mContent, mLength+1, other);
#else
	strncpy(mContent, other, mLength);
	mContent[mLength] = '\0';
#endif
	return *this;
}

bool string::operator==(const string& other) const
{
	return this->cmp(other) == 0;
}

bool string::operator==(const c8 * other) const
{
	return this->cmp(other) == 0;
}

bool string::operator!=(const string& other) const
{
	return this->cmp(other) != 0;
}

bool string::operator!=(const c8 * other) const
{
	return this->cmp(other) != 0;
}

bool string::operator<(const string& other) const
{
	return this->cmp(other.c_str()) < 0;
}

bool string::operator<(const c8 * other) const
{
	return this->cmp(other) < 0;
}

bool string::operator<=(const string& other) const
{
	return this->cmp(other.c_str()) <= 0;
}

bool string::operator<=(const c8 * other) const
{
	return this->cmp(other) <= 0;
}

bool string::operator>(const string& other) const
{
	return this->cmp(other.c_str()) > 0;
}

bool string::operator>(const c8 * other) const
{
	return this->cmp(other) > 0;
}

bool string::operator>=(const string& other) const
{
	return this->cmp(other.c_str()) >= 0;
}

bool string::operator>=(const c8 * other) const
{
	return this->cmp(other) >= 0;
}

string string::operator+(const string& other) const
{
	return *this + other.c_str();
}

string string::operator+(const c8 * other) const
{
	s32 strlen_other = strlen(other);
	c8 * tmp = new c8[mLength+strlen_other+1];
	for (s32 i = 0; i < mLength; i++)
		tmp[i] = mContent[i];
#if defined(_MSC_VER)
	strcpy_s(&tmp[mLength], strlen_other+1, other);
#else
	strncpy(&tmp[mLength], other, strlen_other);
	tmp[mLength+strlen_other] = '\0';
#endif
	return string(tmp, true);
}

string string::operator+(s32 num) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%d", num);
#else
	snprintf(tmp, 20, "%d", num);
#endif
	return *this + tmp;
}

string string::operator+(f32 flt) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%.4f", flt);
#else
	snprintf(tmp, 20, "%.4f", flt);
#endif
	return *this + tmp;
}

string string::operator+(f64 flt) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%.4f", flt);
#else
	snprintf(tmp, 20, "%.4f", flt);
#endif
	return *this + tmp;
}

string string::operator+(u32 num) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%d", num);
#else
	snprintf(tmp, 20, "%d", num);
#endif
	return *this + tmp;
}


string string::operator+(c8 c) const
{
	c8 tmp[2];
#if defined(_MSC_VER)
	sprintf_s(tmp, 2, "%c", c);
#else
	snprintf(tmp, 2, "%c", c);
#endif
	return *this + tmp;
}

string string::operator+(bool b) const
{
	if (b)
		return *this + "true";
	else
		return *this + "false";
}


const string& string::operator+=(const string& other)
{
	*this = *this + other;
	return *this;
}

const string& string::operator+=(const c8 * other)
{
	*this = *this + other;
	return *this;
}

const string& string::operator+=(s32 num)
{
	*this = *this + num;
	return *this;
}

const string& string::operator+=(f32 flt)
{
	*this = *this + flt;
	return *this;
}

const string& string::operator+=(f64 flt)
{
	*this = *this + flt;
	return *this;
}

const string& string::operator+=(u32 num)
{
	*this = *this + num;
	return *this;
}

const string& string::operator+=(c8 c)
{
	*this = *this + c;
	return *this;
}

const string& string::operator+=(bool b)
{
	*this = *this + (b ? "true" : "false");
	return *this;
}

c8& string::operator[](s32 index)
{
	return mContent[index];
}
} // namespace fire_engine

