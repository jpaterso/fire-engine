/**
 * FILE:    Hash.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Hash.cpp 114 2007-10-04 04:57:03Z jpaterso $
 * PURPOSE: Template specializations for quite a few types.
**/

#include "Hash.h"
#include "string.h"
#include "KeyEvent.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

namespace fire_engine
{

/** Default hashing function: implements the Jenkin-one-at-a-time (JOAAT)
 algorithm */
static u32 default_hash(s32 key)
{
	u32 h = static_cast<u32>(key);
	h = h + (h << 10);
	h = h ^ (h >> 6);
	h = h + (h << 3);
	h = h ^ (h >> 11);
	h = h + (h << 15);
	return h;
}

static u32 hash_string(const c8 * str)
{
	u32 h = 0;
	for (u32 i = 0; i < strlen(str); i++)
		h += str[i];
	return h;
}

template <> u32 Hash<u8>::hash(const u8& key)
{
	return default_hash(key);
}

template <> u32 Hash<s8>::hash(const s8& key)
{
	return default_hash(key);
}

template <> u32 Hash<c8>::hash(const c8& key)
{
	return default_hash(key);
}

template <> u32 Hash<u16>::hash(const u16& key)
{
	return default_hash(key);
}

template <> u32 Hash<s16>::hash(const s16& key)
{
	return default_hash(key);
}

template <> u32 Hash<u32>::hash(const u32& key)
{
	return default_hash(key);
}

template <> u32 Hash<s32>::hash(const s32& key)
{
	return default_hash(key);
}

template <> u32 Hash<f32>::hash(const f32& key)
{
	return default_hash(static_cast<s32>(key));
}

template <> u32 Hash<f64>::hash(const f64& key)
{
	return default_hash(static_cast<s32>(key));
}

template <> u32 Hash<KeyEvent::EKEY_CODE>::hash(const KeyEvent::EKEY_CODE& key)
{
	return default_hash(static_cast<s32>(key));
}

template <> u32 Hash<void*>::hash(void* const& key)
{
	return default_hash((s32)key);
}

template <> bool Hash<u8>::equal(const u8& a, const u8& b)
{
	return a == b;
}

template <> bool Hash<s8>::equal(const s8& a, const s8& b)
{
	return a == b;
}

template <> bool Hash<c8>::equal(const c8& a, const c8& b)
{
	return a == b;
}

template <> bool Hash<u16>::equal(const u16& a, const u16& b)
{
	return a == b;
}

template <> bool Hash<s16>::equal(const s16& a, const s16& b)
{
	return a == b;
}

template <> bool Hash<u32>::equal(const u32& a, const u32& b)
{
	return a == b;
}

template <> bool Hash<s32>::equal(const s32& a, const s32& b)
{
	return a == b;
}

template <> bool Hash<f32>::equal(const f32& a, const f32& b)
{
	return a == b;
}

template <> bool Hash<f64>::equal(const f64& a, const f64& b)
{
	return a == b;
}

template <> bool Hash<string>::equal(const string& a, const string& b)
{
	return a == b;
}

template <> bool Hash<KeyEvent::EKEY_CODE>::equal(const KeyEvent::EKEY_CODE& a, const KeyEvent::EKEY_CODE& b)
{
	return a == b;
}

template <> bool Hash<void*>::equal(void* const& a, void* const& b)
{
	return a == b;
}

template <> u32 Hash<string>::hash(const string& key)
{
	return hash_string(key.c_str());
}

u32 StringHashIgnoreCase::hash(const string& key)
{
	u32 h = 0;
	for (s32 i = 0; i < key.length(); i++)
		h = h+tolower(key.c_str()[i]);
	return h;
}

bool StringHashIgnoreCase::equal(const string& a, const string& b)
{
	return a.equalsIgnoreCase(b);
}
}
