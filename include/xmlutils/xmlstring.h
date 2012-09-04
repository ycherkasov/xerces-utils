/* 
 * File:   string.h
 * Author: ycherkasov
 *
 * Created on 8 Ноябрь 2010 г., 13:41
 */

#ifndef STRING_H
#define	STRING_H

#include <string>
#include <xercesc/util/XMLString.hpp>
#include "xmlutils/xerces_auto_ptr.h"

namespace xerces{

/** @brief This class implements RAII-wrapper under Xerces wide-char string.<br>
 * It is just a adapter for xerces_auto_ptr<XMLString> to implement more usable
 * and convenient interface.
 * */
class string{
public:

    /** @brief Construct XML wide-string from ASCII-string.
     * */
    string(const char* s) : _str(s) { }

    /** @brief Construct XML wide-string from XML wide char (UTF-16).
     * */
    string(XMLCh* s) : _str(s) {}

    /** @brief Construct XML wide-string from const XML wide char (UTF-16).
     * */
    string(const XMLCh* s) : _str(s) {}

    /** @brief Get standard string
     * @return new std::string converted from XML string.
     * */
    std::string get_string() const {
	return _str.get_string();
    }

    /** @brief Get wide-char string
     * @return wide-char UTF-16 string.
     * */
    const XMLCh* get_wchar() const {
        return _str.get();
    }


private:
    /**@brief Smart string array we hold */
    xerces_auto_ptr<const char* > _str;
};

}

#endif	/* STRING_H */

