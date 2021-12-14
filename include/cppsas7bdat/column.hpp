/**
 *  \file cppsas7bdat/column.hpp
 * 
 *  \brief Column description
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_COLUMN_HPP_
#define _CPP_SAS7BDAT_COLUMN_HPP_

#include <string>
#include <memory>
#include <cstdint>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

namespace cppsas7bdat {

  using STRING = std::string_view;
  using NUMBER = double;
  using INTEGER = int32_t;
  using DATETIME = boost::posix_time::ptime;
  using DATE = boost::gregorian::date;
  using TIME = boost::posix_time::time_duration;

  struct Column {
    enum class Type { unknown, string, number, integer, datetime, date, time };
    using PBUF= const void*;
	
  private:
    struct FormatterConcept;
    using PIMPL = std::shared_ptr<FormatterConcept>;

    struct FormatterConcept {
      virtual ~FormatterConcept() {}

      virtual Type     type() const noexcept = 0;
      virtual STRING   get_string(PBUF _p) const noexcept = 0;
      virtual NUMBER   get_number(PBUF _p) const noexcept = 0;
      virtual INTEGER  get_integer(PBUF _p) const noexcept = 0;
      virtual DATETIME get_datetime(PBUF _p) const noexcept = 0;
      virtual DATE     get_date(PBUF _p) const noexcept = 0;
      virtual TIME     get_time(PBUF _p) const noexcept = 0;
    };

    template<typename _Fp>
    struct FormatterModel : public FormatterConcept {
      FormatterModel(_Fp&& _formatter)
	: formatter(std::forward<_Fp>(_formatter))
      {
      }
      FormatterModel(const FormatterModel&) = default;
	  

      Type type() const noexcept final
      {
	return formatter.type;
      }

      STRING get_string(PBUF _p) const noexcept final
      {
	return formatter.get_string(_p);
      }
	  
      NUMBER get_number(PBUF _p) const noexcept final
      {
	return formatter.get_number(_p);
      }
	  
      INTEGER get_integer(PBUF _p) const noexcept final
      {
	return formatter.get_integer(_p);
      }
      
      DATETIME get_datetime(PBUF _p) const noexcept final
      {
	return formatter.get_datetime(_p);
      }
      
      DATE get_date(PBUF _p) const noexcept final
      {
	return formatter.get_date(_p);
      }
      
      TIME get_time(PBUF _p) const noexcept final
      {
	return formatter.get_time(_p);
      }
    private:
      _Fp formatter;
    };

  public:
    template<typename _Fp>
    Column(const std::string& _name,
	   const std::string& _label,
	   const std::string& _format,
	   _Fp&& _formatter)
      : name(_name),
	label(_label),
	format(_format),
	pimpl(std::make_shared< FormatterModel<_Fp> >(std::forward<_Fp>(_formatter)))
    {
    }
    Column(const Column& _rhs) = default;
    Column(Column&& _rhs) noexcept= default;
    
    const std::string name;
    const std::string label;
    const std::string format;

    Type     type() const noexcept { return pimpl->type(); }
    STRING   get_string(PBUF _p) const noexcept { return pimpl->get_string(_p); }
    NUMBER   get_number(PBUF _p) const noexcept { return pimpl->get_number(_p); }
    INTEGER  get_integer(PBUF _p) const noexcept { return pimpl->get_integer(_p); }
    DATETIME get_datetime(PBUF _p) const noexcept { return pimpl->get_datetime(_p); }
    DATE     get_date(PBUF _p) const noexcept { return pimpl->get_date(_p); }
    TIME     get_time(PBUF _p) const noexcept { return pimpl->get_time(_p); }
    
  private:
    PIMPL pimpl;
  };

  using COLUMNS = std::vector<Column>;

  std::string_view to_string(const Column::Type _x);
  std::ostream& operator<<(std::ostream& os, const Column::Type _x);
}


#endif
