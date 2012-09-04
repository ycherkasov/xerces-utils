/* 
 * File:   xerces_auto_ptr.h
 * Author: ycherkasov
 *
 * Created on 8 Ноябрь 2010 г., 10:35
 */

#ifndef XERCES_AUTO_PTR_H
#define	XERCES_AUTO_PTR_H

#include <stdexcept>
#include <boost/noncopyable.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

XERCES_CPP_NAMESPACE_USE
XALAN_CPP_NAMESPACE_USE

namespace xerces {

 /** @brief This class implements a set if RAII-wrappers for any Xerces-c and
  * Xalan-c library objects.<br>*/
 /** Most of the Xerces objects requires for manual initialization and
  * manual release. It is worse that we can't use standard C++ or Boost
  * smart pointers and arrays, because Xerces and Xalan uses their own
  * memory manager.
  * So, we should use owr own wrapper for safe and convenient work with
  * Xerces objects. Most of them uses <code>release()</code> method, therefore
  * we call it as a <code>do_release()</code> method of owr template wrapper.
  * Some of them, like XMLString uses special operations to release, so we use
  * template class specializations to specify its release operations.
  *
  * Usage examples:
  * <@code>
  * //XML document wrapper
  * xerces_auto_ptr<DOMDocument> xmlstring( create_dom_document() );
  *
  * // XML string wrapper
  * xerces_auto_ptr<const char*> xmlstring("mystring");
  * <@endcode>
  * BTW, use <code>dom_document</code> facade class instead of
  * <code>xerces_auto_ptr</code>, therefore it has more friendly interface
  * for main XML-document operations with nodes, attributes and XML files.
  */
template <typename T>
class xerces_auto_ptr{

public:
    /** @brief Default wrapper constructor.<br>
     * It uses default construction for wrapped object.
     *  */
    xerces_auto_ptr() : item_(0) { }

    /** @brief Wrapper constructor from resource.<br>
     * We can allocate resources outside the wrapper.
     * @param i resource to apply RAII idiom
     *  */
    explicit xerces_auto_ptr(T* i) : item_(i) { }

    /** @brief Release resources
     *  */
    ~xerces_auto_ptr() {
        do_release();
    }

    
    /** @brief Give up held data (i.e. return data without releasing)**/
    T* yield() {
        T* tempItem = item_;
        item_ = 0;
        return tempItem;
    }

    
    /** @brief Release currently held data, if any, to hold another
     *  @param i resource to assign */
    void assign(T* i) {
	do_release();
        item_ = i;
    }

    
    /** @brief Get pointer to the currently held data, if any */
    T* get() {
	return item_;
    }

    
    /** @brief Get pointer to the currently held data (for constant objects) */
    const T* get() const {
	return item_;
    }

    
    /** @brief Get pointer to the currently held data, if any */
    T * operator->() {
	return item_;
    }

    
    /** @brief Get pointer to the currently held data (for constant objects) */
    const T * operator->() const {
	return item_;
    }

    
    /** @return true if no data is held */
    bool is_released() const {
        return (0 == item_);
    }

protected:
    
    /** @brief Function to release Xerces data type (should be specialized)*/
    void do_release()
    {
        // Only release this if it has no parent (otherwise
        // parent will release it)
	if(item_ == 0)
	    return;

        if (0 == item_->getOwnerDocument())
            item_->release();
    }

private:

    /** @brief The actual data we're holding*/
    T* item_;
};


//----------------------------------------
/** @brief This template class implements a RAII specialization for XMLString objects.
 * It is nessesary because we use <code>XMLString::release()</code> static
 * method instead of normal <code>release()</code>.
 */
template <>
class xerces_auto_ptr<const char*>{
    public:

    
    /** @brief Default wrapper constructor.<br>
     * It uses default construction for wrapped object.
     *  */
    xerces_auto_ptr() : item_(0) {}

    
    /** @brief Constructor specialization for XMLString.<br>
     * Constructor specialization to allocate XML wide-char strings (UTF-16)
     * from ANSI-string.
     * @param s ANSI string
     *  */
    explicit xerces_auto_ptr(const char* s) : item_( XMLString::transcode(s) ) {}

    
    /** @brief Constructor specialization for XMLString.<br>
     * Constructor specialization to allocate XML wide-char strings (UTF-16)
     * from another wide-char string.
     * @param s ANSI string
     *  */
    explicit xerces_auto_ptr(XMLCh* s) : item_( XMLString::replicate(s) ) {}

    
    /** @brief Constructor specialization for XMLString.<br>
     * Constructor specialization to allocate XML wide-char strings (UTF-16)
     * from another constant wide-char string.
     * @param s ANSI string
     *  */
    explicit xerces_auto_ptr(const XMLCh* s) : item_( XMLString::replicate(s) ) {}

    /** @brief Release resources
     *  */
    ~xerces_auto_ptr() {
        do_release();
    }


    /** @brief Give up held data (i.e. return data without releasing)
     *  */
    XMLCh* yield() {
        XMLCh* tempItem = item_;
        item_ = 0;
        return tempItem;
    }

    /** @brief Release currently held data, if any, to hold another
     *  */
    void assign(XMLCh* i) {
        do_release();
        item_ = i;
    }

    /** @brief Get pointer to the currently held data, if any
     *  */
    const XMLCh* get() const {
        return item_;
    }

    /** @brief Get <code>std::string</code> converted from XMLString
     *  */
    std::string get_string() const {
	char* chstr = XMLString::transcode(item_);
	std::string ret(chstr);
	XMLString::release(&chstr);
	return ret;
    }

    /** @brief Get wide-char string from XMLString
     *  */
    const XMLCh* operator->() const {
        return item_;
    }

    /** @return true if no data is held
     *  */
    bool is_released() const {
        return (0 == item_);
    }

protected:
        // 
    /** @brief Specialized function to release XMLString data
     *  */
    void do_release()
    {
        // Only release this if it has no parent
	XMLString::release(&item_);
    }


private:
    /** @brief The actual data we're holding */
    XMLCh* item_;
};


//----------------------------------------
/** @brief This template class implements a RAII specialization
 * for XMLPlatformUtils. <br>
 * It is unnesesary to perform any another operations with this wrapper,
 * it is just a context to perform any operations with XML documents.
 */
template <>
class xerces_auto_ptr<XMLPlatformUtils> : boost::noncopyable {
public:

    /** @brief Default and the one constructor. <br>
     * It is controlled by try-catch, so it can catch any XML-exceptions
     * and throw it out of library
     */
    xerces_auto_ptr() try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char *pMsg = XMLString::transcode(toCatch.getMessage());
        std::string s(pMsg);
        XMLString::release(&pMsg);
        throw std::runtime_error(s.c_str());
    }

    /** @brief Terminate context */
    ~xerces_auto_ptr() {
	XMLPlatformUtils::Terminate();
    }
};

//-----------------------------------------
/** @brief A template class implements a pecialization XalanTransformer. <br>
 * XalanTransformer is an analog for XMLPlatformUtils, so it's just an
 * execution context.
 */
XALAN_USING_XALAN(XalanTransformer)
template <>
class xerces_auto_ptr<XalanTransformer> : boost::noncopyable {
public:

    xerces_auto_ptr() {
        XalanTransformer::initialize();
    }

    ~xerces_auto_ptr() {
	XalanTransformer::terminate();
	XalanTransformer::ICUCleanUp();
    }

private:
    // Hide copy constructor and assignment operator
    xerces_auto_ptr(const xerces_auto_ptr&);
    xerces_auto_ptr & operator=(const xerces_auto_ptr&);

};

}


#endif	/* XERCES_AUTO_PTR_H */

