/**
 *  \file src/buffer.hpp
 * 
 *  \brief Memory management for buffer
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_BUFFER_HPP_
#define _CPP_SAS7BDAT_SRC_BUFFER_HPP_

#include "memory.hpp"
#include <iostream>
#include <cstring>

namespace cppsas7bdat {
  namespace INTERNAL {

    class MBUFFER {
    private:
      size_t m_size{0};
      MEMORY::PALIGNEDMEM m_buffer{};
      
    public:
      MBUFFER() {}
      explicit MBUFFER(const size_t _size)
	: m_size(_size),
	  m_buffer(INTERNAL::MEMORY::aligned_alloc(size()))
      {
      }      
      
      MBUFFER(const MBUFFER&) = delete;
      MBUFFER(MBUFFER&&) = default;
      MBUFFER& operator=(const MBUFFER&) = delete;
      MBUFFER& operator=(MBUFFER&&) = delete;
      
      void resize(const size_t _size)
      {
	// Realloc (new+copy) only to increase the size of the buffer.
	if(_size > size()) {
	  // Allocate a new buffer
	  INTERNAL::MEMORY::PALIGNEDMEM buffer{INTERNAL::MEMORY::aligned_alloc(_size)};
	  // Copy the current into the new buffer if the previous buffer is valid.
	  if(m_buffer && size()) std::memcpy(buffer.get(), m_buffer.get(), size());
	  // Swap the new buffer with the current one
	  std::swap(m_buffer, buffer);
	}
	m_size = _size;
      }

      template<typename _DataSource>
      bool read_stream(_DataSource& _is, const size_t _read_length, const size_t _offset_in_buffer=0)
      {
	// Make sure the buffer is big enough to hold the data
	resize(_offset_in_buffer + _read_length);
	// Read the stream
	return _is->read_bytes(m_buffer.get()+_offset_in_buffer, _read_length);
	//if(_is)
	  //_is.read(reinterpret_cast<char*>(m_buffer.get())+_offset_in_buffer, static_cast<std::streamsize>(_read_length));
	// Did we manage to read the requested data?
	//return _is.good();
      }
      
      size_t size() const noexcept { return m_size; }
      
      const uint8_t* data(const size_t _offset,
			  [[maybe_unused]] const size_t _length) const noexcept
      {
	assert(_offset + _length <= size());
	return m_buffer.get() + _offset;
      }
      
      uint8_t* data(const size_t _offset,
		    [[maybe_unused]] const size_t _length) noexcept
      {
	assert(_offset + _length <= size());
	return m_buffer.get() + _offset;
      }
      
      uint8_t operator[](const size_t _offset) const noexcept { return *data(_offset, 1); }

      void set(const size_t _offset, const uint8_t _v) noexcept {
	*data(_offset, 1) = _v;
      };
      void memset(const size_t _offset, const uint8_t _v, const size_t _length) noexcept {
	std::memset(data(_offset, _length), _v, _length);
      };
      void memcpy(const size_t _offset_dest, const size_t _offset_src, const size_t _length) noexcept {
	std::memcpy(data(_offset_dest, _length), data(_offset_src, _length), _length);
      }
      void memcpy(const size_t _offset, const BYTES& _src, const size_t _length) noexcept {
	assert(_src.size() >= _length);
	std::memcpy(data(_offset, _length), _src.data(), _length);
      }

      BYTES as_bytes() const noexcept
      {
	return INTERNAL::get_bytes(m_buffer.get(), size());
      }
    };

    template<Endian _endian>
    class EBUFFER : public MBUFFER {
    public:
      constexpr static auto endian=_endian;
      
      EBUFFER() {}
      explicit EBUFFER(const size_t _size) : MBUFFER(_size) {}
      EBUFFER(MBUFFER&& _buffer) : MBUFFER(std::move(_buffer)) {}

      uint8_t get_byte(const size_t _offset) const noexcept { return *data(_offset, 1); }

      BYTES get_bytes(const size_t _offset, const size_t _length) const noexcept
      {
	return INTERNAL::get_bytes(data(_offset, _length), _length);
      }
      
      int16_t get_int16(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, int16_t>(data(_offset, sizeof(int16_t)));
      }
      
      uint16_t get_uint16(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, uint16_t>(data(_offset, sizeof(uint16_t)));
      }
      
      int32_t get_int32(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, int32_t>(data(_offset, sizeof(int32_t)));
      }
      
      uint32_t get_uint32(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, uint32_t>(data(_offset, sizeof(uint32_t)));
      }

      double get_double(const size_t _offset) const noexcept
      {
	return INTERNAL::get_double<_endian>(data(_offset, sizeof(double)));
      }
      
      std::string_view get_string(const size_t _offset, const size_t _length) const noexcept
      {
	return INTERNAL::get_string_trim(data(_offset, _length), _length);
      }
      
      std::string_view get_string_untrim(const size_t _offset, const size_t _length) const noexcept
      {
	return INTERNAL::get_string(data(_offset, _length), _length);
      }
    };

    template<Endian _endian, Format _format>
    class BUFFER : public EBUFFER<_endian> {
    public:
      constexpr static auto format=_format;
      using EBUFFER<_endian>::data;
      using integer_t = typename number_types<_format>::integer_t;
      using uinteger_t = typename number_types<_format>::uinteger_t;
      constexpr static size_t integer_size = sizeof(integer_t);
      constexpr static size_t uinteger_size = sizeof(uinteger_t);
      
      BUFFER() {}
      explicit BUFFER(const size_t _size) : EBUFFER<_endian>(_size) {}
      BUFFER(MBUFFER&& _buffer) : EBUFFER<_endian>(std::move(_buffer)) {}

      integer_t get_integer(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, integer_t>(data(_offset, sizeof(integer_t)));
      }

      uinteger_t get_uinteger(const size_t _offset) const noexcept
      {
	return INTERNAL::get_val<_endian, uinteger_t>(data(_offset, sizeof(uinteger_t)));
      }
    };
   
  }
}

#endif
