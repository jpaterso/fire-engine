/**
 *  $Id: String.h 5 2007-06-11 03:14:50Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The string class is useful for storing fixed strings, and wrap opreations
 *  such as comparison, sub-string search, copying...
**/

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

/** A class to easily represent a C-style null-terminated string. It makes operations
 like appending and comparison much easier by providing an easy-to-use interface with
 intuitive operations, like + for appending and >, <, >=, <=, == and != for comparison. */
class _FIRE_ENGINE_API_ string
{
  public:
	/** Default constructor.*/
    string();

	/** Construct a string from a null-terminated sequence of characters. */
    string(const c8 * str);

	/** Construct a string from a null-terminated sequence of characters, and specify the length.
	 This can speed things up if the length is pre-calculated, but should be used carefully. */
	string(const c8 * str, s32 pre_calculated_length);

	/** Construct a string with a pre-allocated buffer. The string will be kept, and liberated
	 when the string is destroyed. */
	string(c8 * str, bool keep_string);

	/** Construct a string from another string. */
	string(const string& str);

	/** Destructor - frees the allocated memory. */
	~string();

    /** Returns the length of the string. */
    s32 length() const;

	/** Returns the character at position i in the string. */
	c8 at(s32 i) const;

    /** Compare two strings, and return the difference of the first mismatched characters, or
	 0 if the strings were equal. */
    s32 cmp(const string& other) const;

	/** Compare two strings, and return the difference of the first mismatched characters, or
	0 if the strings were equal. */
    s32 cmp(const c8 * other) const;

	/** Returns a pointer to the data in the string. */
    const c8 * c_str() const;

    /** Appends another string onto the current string, and returns the result. */
    const string& append(const string& other);

    /** Appends a null-terminated sequence of characters onto the current string, and
	 returns the result. */
    const string& append(const c8 * other);

    /** Returns whether the given string is a substring of the current string. */
    bool isSubstring(const string& str) const;

	/** Returns whether the given null-terminated sequence of characters is a substring of
	 the current string. */
	bool isSubstring(const c8 * str) const;

    /** Locate the first occurence of str in the current string, and return a pointer to that
	 location. Returns 0 (NULL) if str is not found. */
    const c8 * substring(const c8 * str) const;

	/** Replaces all occurences of charA with charB.
	 \param charA The character to look for and replace.
	 \param charB The character to replace charA with. */
	void replaceAll(c8 charA, c8 charB);

	/** Makes all the characters in the string lowercase. */
	void makeLower();

	/** Checks whether the given string is equal to this string, ignoring case. */
	bool equalsIgnoreCase(const string& str) const;

	/** Returns whether two strings are identical. */
	static bool equals(const string& l, const string& r);

	/** Returns whether two strings match, ignoring case. */
	static bool equalsIgnoreCase(const string& l, const string& r);

    /** Assign a new string to the current string. */
    const string& operator=(const string& other);

	/** Assign a new sequence of characters to the current string. */
    const string& operator=(const c8 * other);

    /** Returns whether the given string is identical to the current string. */
    bool operator==(const string& other) const;

	/** Returns whether the given null-terminated sequence of characters is identical
	 to the current string. */
    bool operator==(const c8 * other) const;

	/** Returns whether the given string is different from the current string. */
    bool operator!=(const string& other) const;

	/** Returns whether the given sequence of characters is different from the current string. */
    bool operator!=(const c8 * other) const;

	/** Returns whether the current string is smaller (sorted by alphabetical order) than
	 the given string. */
    bool operator<(const string& other) const;

	/** Returns whether the current string is smaller (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator<(const c8 * other) const;

	/** Returns whether the current string is smaller or equal (sorted by alphabetical order) than
	the given string. */
    bool operator<=(const string& other) const;

	/** Returns whether the current string is smaller or equal (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator<=(const c8 * other) const;

	/** Returns whether the current string is greater (sorted by alphabetical order) than
	the given string. */
    bool operator>(const string& other) const;

	/** Returns whether the current string is greater (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator>(const c8 * other) const;

	/** Returns whether the current string is greater or equal (sorted by alphabetical order) than
	the given string. */
    bool operator>=(const string& other) const;

	/** Returns whether the current string is greater or equal (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator>=(const c8 * other) const;

    /** Returns the result of concatenating a string onto the current one. */
    string operator+(const string& other) const;

	/** Returns the result of concatenating a sequence of characters onto the current one. */
    string operator+(const c8 * other) const;

	/** Returns the result of concatenating an integer onto the current one. */
    string operator+(s32 num) const;

	/** Returns the result of concatenating a floating point number onto the current one. */
    string operator+(f32 flt) const;

	/** Returns the result of concatenating a floating point number onto the current one. */
	string operator+(f64 flt) const;

	/** Returns the result of concatenating an unsigned integer onto the current one. */
    string operator+(u32 num) const;

	/** Returns the result of concatenating a single character onto the current one. */
    string operator+(c8 c)    const;

	/** Returns the result of concatenating a boolean value onto the current one. */
	string operator+(bool b)  const;

	/** Appends a string onto the current string. */
    const string& operator+=(const string& other);

	/** Appends a sequence of characters onto the current string. */
    const string& operator+=(const c8 * other);

	/** Appends an integer onto the current string. */
    const string& operator+=(s32 num);

	/** Appends a floating point number onto the current string. */
    const string& operator+=(f32 flt);

	/** Appends a floating point number onto the current string. */
	const string& operator+=(f64 flt);

	/** Appends an unsigned integer onto the current string. */
    const string& operator+=(u32 num);

	/** Appends a single character onto the current string. */
    const string& operator+=(c8 c);

	/** Appends a boolean value onto the current string. */
	const string& operator+=(bool b);

    /** Returns a reference to the character at a given index in the string. */
    c8& operator[](s32 index);

  private:
    c8 *  mContent;
    s32   mLength;
};
} // namespace fire_engine

#endif // STRING_H_INCLUDED

