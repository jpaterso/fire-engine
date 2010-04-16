/**
 *  $Id: String.cpp 5 2007-06-11 03:14:50Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  This file defines the String class, as defined in string.h
**/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#include "String.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

namespace fire_engine
{

String::String()
	: mContent(0), mLength(0)
{
}

String::String(const c8 * str)
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

String::String(const String& str)
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

String::String(const c8 * str, s32 pre_calculated_length)
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

String::String(c8 * str, bool keep_string)
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

String::~String()
{
	if (mContent != 0)
		delete [] mContent;
}

s32 String::length(void) const
{
	return mLength;
}

c8 String::at(s32 i) const
{
	if (i >= mLength)
		return (-1);
	return mContent[i];
}

s32 String::cmp(const String& other) const
{
	return strcmp(mContent, other.c_str());
}

s32 String::cmp(const c8 * other) const
{
	return strcmp(mContent, other);
}

const c8 * String::c_str(void) const
{
	return mContent;
}

const String& String::append(const String& other)
{
	*this += other;
	return *this;
}

const String& String::append(const c8 * other)
{
	*this += other;
	return *this;
}

bool String::isSubstring(const String& str) const
{
	return strstr(mContent, str.c_str()) != NULL;
}

bool String::isSubstring(const c8 * str) const
{
	return strstr(mContent, str) != NULL;
}

const c8 * String::substring(const c8 * str) const
{
	return strstr(mContent, str);
}

void String::replaceAll(c8 charA, c8 charB)
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

void String::makeLower()
{
	for (s32 i = 0; i < mLength; i++)
		mContent[i] = tolower(mContent[i]);
}

bool String::equalsIgnoreCase(const String& str) const
{
	if (mLength != str.length())
		return false;
	for (s32 i = 0; i < mLength; i++)
		if (tolower(mContent[i]) != tolower(str.c_str()[i]))
			return false;
	return true;
}

bool String::equals(const String &l, const String &r)
{
	return l == r;
}

bool String::equalsIgnoreCase(const String& l, const String& r)
{
	return l.equalsIgnoreCase(r);
}

const String& String::operator=(const String& other)
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

const String& String::operator=(const c8 * other)
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

bool String::operator==(const String& other) const
{
	return this->cmp(other) == 0;
}

bool String::operator==(const c8 * other) const
{
	return this->cmp(other) == 0;
}

bool String::operator!=(const String& other) const
{
	return this->cmp(other) != 0;
}

bool String::operator!=(const c8 * other) const
{
	return this->cmp(other) != 0;
}

bool String::operator<(const String& other) const
{
	return this->cmp(other.c_str()) < 0;
}

bool String::operator<(const c8 * other) const
{
	return this->cmp(other) < 0;
}

bool String::operator<=(const String& other) const
{
	return this->cmp(other.c_str()) <= 0;
}

bool String::operator<=(const c8 * other) const
{
	return this->cmp(other) <= 0;
}

bool String::operator>(const String& other) const
{
	return this->cmp(other.c_str()) > 0;
}

bool String::operator>(const c8 * other) const
{
	return this->cmp(other) > 0;
}

bool String::operator>=(const String& other) const
{
	return this->cmp(other.c_str()) >= 0;
}

bool String::operator>=(const c8 * other) const
{
	return this->cmp(other) >= 0;
}

String String::operator+(const String& other) const
{
	return *this + other.c_str();
}

String String::operator+(const c8 * other) const
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
	return String(tmp, true);
}

String String::operator+(s32 num) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%d", num);
#else
	snprintf(tmp, 20, "%d", num);
#endif
	return *this + tmp;
}

String String::operator+(f32 flt) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%.4f", flt);
#else
	snprintf(tmp, 20, "%.4f", flt);
#endif
	return *this + tmp;
}

String String::operator+(f64 flt) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%.4f", flt);
#else
	snprintf(tmp, 20, "%.4f", flt);
#endif
	return *this + tmp;
}

String String::operator+(u32 num) const
{
	c8 tmp[20];
#if defined(_MSC_VER)
	sprintf_s(tmp, 20, "%d", num);
#else
	snprintf(tmp, 20, "%d", num);
#endif
	return *this + tmp;
}


String String::operator+(c8 c) const
{
	c8 tmp[2];
#if defined(_MSC_VER)
	sprintf_s(tmp, 2, "%c", c);
#else
	snprintf(tmp, 2, "%c", c);
#endif
	return *this + tmp;
}

String String::operator+(bool b) const
{
	if (b)
		return *this + "true";
	else
		return *this + "false";
}


const String& String::operator+=(const String& other)
{
	*this = *this + other;
	return *this;
}

const String& String::operator+=(const c8 * other)
{
	*this = *this + other;
	return *this;
}

const String& String::operator+=(s32 num)
{
	*this = *this + num;
	return *this;
}

const String& String::operator+=(f32 flt)
{
	*this = *this + flt;
	return *this;
}

const String& String::operator+=(f64 flt)
{
	*this = *this + flt;
	return *this;
}

const String& String::operator+=(u32 num)
{
	*this = *this + num;
	return *this;
}

const String& String::operator+=(c8 c)
{
	*this = *this + c;
	return *this;
}

const String& String::operator+=(bool b)
{
	*this = *this + (b ? "true" : "false");
	return *this;
}

c8& String::operator[](s32 index)
{
	return mContent[index];
}
} // namespace fire_engine

