/**
 *  $Id: string.h 5 2007-06-11 03:14:50Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  The String class is useful for storing fixed strings, and wrap opreations
 *  such as comparison, sub-String search, copying...
**/

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"

namespace fire_engine
{

/** A class to easily represent a C-style null-terminated String. It makes operations
 like appending and comparison much easier by providing an easy-to-use interface with
 intuitive operations, like + for appending and >, <, >=, <=, == and != for comparison. */
class _FIRE_ENGINE_API_ String
{
  public:
	/** Default constructor.*/
    String();

	/** Construct a String from a null-terminated sequence of characters. */
    String(const c8 * str);

	/** Construct a String from a null-terminated sequence of characters, and specify the length.
	 This can speed things up if the length is pre-calculated, but should be used carefully. */
	String(const c8 * str, s32 pre_calculated_length);

	/** Construct a String with a pre-allocated buffer. The String will be kept, and liberated
	 when the String is destroyed. */
	String(c8 * str, bool keep_string);

	/** Construct a String from another String. */
	String(const String& str);

	/** Destructor - frees the allocated memory. */
	~String();

    /** Returns the length of the String. */
    s32 length() const;

	/** Returns the character at position i in the String. */
	c8 at(s32 i) const;

    /** Compare two strings, and return the difference of the first mismatched characters, or
	 0 if the strings were equal. */
    s32 cmp(const String& other) const;

	/** Compare two strings, and return the difference of the first mismatched characters, or
	0 if the strings were equal. */
    s32 cmp(const c8 * other) const;

	/** Returns a pointer to the data in the String. */
    const c8 * c_str() const;

    /** Appends another String onto the current String, and returns the result. */
    const String& append(const String& other);

    /** Appends a null-terminated sequence of characters onto the current String, and
	 returns the result. */
    const String& append(const c8 * other);

    /** Returns whether the given String is a substring of the current String. */
    bool isSubstring(const String& str) const;

	/** Returns whether the given null-terminated sequence of characters is a substring of
	 the current String. */
	bool isSubstring(const c8 * str) const;

    /** Locate the first occurence of str in the current String, and return a pointer to that
	 location. Returns 0 (NULL) if str is not found. */
    const c8 * substring(const c8 * str) const;

	/** Replaces all occurences of charA with charB.
	 \param charA The character to look for and replace.
	 \param charB The character to replace charA with. */
	void replaceAll(c8 charA, c8 charB);

	/** Makes all the characters in the String lowercase. */
	void makeLower();

	/** Checks whether the given String is equal to this String, ignoring case. */
	bool equalsIgnoreCase(const String& str) const;

	/** Returns whether two strings are identical. */
	static bool equals(const String& l, const String& r);

	/** Returns whether two strings match, ignoring case. */
	static bool equalsIgnoreCase(const String& l, const String& r);

    /** Assign a new String to the current String. */
    const String& operator=(const String& other);

	/** Assign a new sequence of characters to the current String. */
    const String& operator=(const c8 * other);

    /** Returns whether the given String is identical to the current String. */
    bool operator==(const String& other) const;

	/** Returns whether the given null-terminated sequence of characters is identical
	 to the current String. */
    bool operator==(const c8 * other) const;

	/** Returns whether the given String is different from the current String. */
    bool operator!=(const String& other) const;

	/** Returns whether the given sequence of characters is different from the current String. */
    bool operator!=(const c8 * other) const;

	/** Returns whether the current String is smaller (sorted by alphabetical order) than
	 the given String. */
    bool operator<(const String& other) const;

	/** Returns whether the current String is smaller (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator<(const c8 * other) const;

	/** Returns whether the current String is smaller or equal (sorted by alphabetical order) than
	the given String. */
    bool operator<=(const String& other) const;

	/** Returns whether the current String is smaller or equal (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator<=(const c8 * other) const;

	/** Returns whether the current String is greater (sorted by alphabetical order) than
	the given String. */
    bool operator>(const String& other) const;

	/** Returns whether the current String is greater (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator>(const c8 * other) const;

	/** Returns whether the current String is greater or equal (sorted by alphabetical order) than
	the given String. */
    bool operator>=(const String& other) const;

	/** Returns whether the current String is greater or equal (sorted by alphabetical order) than
	the given null-terminated sequence of characters. */
    bool operator>=(const c8 * other) const;

    /** Returns the result of concatenating a String onto the current one. */
    String operator+(const String& other) const;

	/** Returns the result of concatenating a sequence of characters onto the current one. */
    String operator+(const c8 * other) const;

	/** Returns the result of concatenating an integer onto the current one. */
    String operator+(s32 num) const;

	/** Returns the result of concatenating a floating point number onto the current one. */
    String operator+(f32 flt) const;

	/** Returns the result of concatenating a floating point number onto the current one. */
	String operator+(f64 flt) const;

	/** Returns the result of concatenating an unsigned integer onto the current one. */
    String operator+(u32 num) const;

	/** Returns the result of concatenating a single character onto the current one. */
    String operator+(c8 c)    const;

	/** Returns the result of concatenating a boolean value onto the current one. */
	String operator+(bool b)  const;

	/** Appends a String onto the current String. */
    const String& operator+=(const String& other);

	/** Appends a sequence of characters onto the current String. */
    const String& operator+=(const c8 * other);

	/** Appends an integer onto the current String. */
    const String& operator+=(s32 num);

	/** Appends a floating point number onto the current String. */
    const String& operator+=(f32 flt);

	/** Appends a floating point number onto the current String. */
	const String& operator+=(f64 flt);

	/** Appends an unsigned integer onto the current String. */
    const String& operator+=(u32 num);

	/** Appends a single character onto the current String. */
    const String& operator+=(c8 c);

	/** Appends a boolean value onto the current String. */
	const String& operator+=(bool b);

    /** Returns a reference to the character at a given index in the String. */
    c8& operator[](s32 index);

  private:
    c8 *  mContent;
    s32   mLength;
};
} // namespace fire_engine

#endif // STRING_H_INCLUDED

