/* 
 * File:   xpath.h
 * Author: ycherkasov
 *
 * Created on 13 Ноябрь 2010 г., 14:53
 */

#ifndef XPATH_H
#define	XPATH_H

#include <vector>
#include <string>
#include <boost/noncopyable.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include "xmlutils/xmlstring.h"

namespace xerces {

/** @brief It is a simple XPath wrapper under the Xalan-C++ library.<br>
 * XPath, the XML Path Language, is a query language for selecting nodes
 * from any XML document.
 * In addition, XPath may be used to compute values (e.g., strings,
 * numbers, or boolean values) from the content of an XML document.
 * See also http://en.wikipedia.org/wiki/XPath <br>
 * Result set can be given by <code>result()</code> function as a vector
 * of strings.
 * @code
 *  xpath evaluator(filename);
 *  // call evaluate, passing in the XML string, the context string and the xpath string
 *  evaluator.evaluate("/root/color_settings[@color=\"0xFFCCFFFF\"]", "\");
 *  const std::vector<std::string>& res = evaluator.result();
 * @endcode
 */
class xpath : boost::noncopyable {

    /** @brief This structure implements wrapper under Xalan RAII-objects
     * and wrappers.<br>
     * You should create and initialize it before any XPath evaluations.
     * You mustn't change members initialization order due to ugly
     * Xalan design. It may cause an exception raise or even application crash.
     *  */
    struct helper_t {

        // do not change initialization order!
        XalanSourceTreeDOMSupport _dom_wrapper;
        XalanSourceTreeParserLiaison _liason_wrapper;
        XPathEnvSupportDefault _environment_wrapper;
        XObjectFactoryDefault _xobject_factory;
        const ElementPrefixResolverProxy _prefix_resolver;
        XPathExecutionContextDefault _exec_context;
        XPathConstructionContextDefault _construction_context;
        XPathFactoryDefault _xpath_factory;
        XPathProcessorImpl _xpath_processor;
        XPath* const _xpath_context;
        XPath* _xpath_exec_context;


        /** @brief XPath helper constructor<br>
         * @param root_elem root element of related DOM document. Can be NULL
         *  */
        helper_t(XalanElement * root_elem) : _dom_wrapper()
        , _liason_wrapper(_dom_wrapper)
        , _prefix_resolver(root_elem, _environment_wrapper, _dom_wrapper)
        , _exec_context(_environment_wrapper, _dom_wrapper, _xobject_factory)
        , _xpath_context(_xpath_factory.create()) {
            _dom_wrapper.setParserLiaison(&_liason_wrapper);
        }

        

        /** @brief XPath helper initialization<br>
         * @param context context to perform XPath queries. Normally is root("/")
         *  */
        void init(const XalanDOMString & context) {
            _xpath_processor.initXPath(*_xpath_context, _construction_context
                    , context, _prefix_resolver);
        }

    };
public:
    
    /** @brief XPath evaluator constructor<br>
     * @param filename XML file name
     *  */
    xpath(const std::string& filename);

    ~xpath();

    
    /** @brief XPath evaluator constructor<br>
     * Given an xml document and an xpath context and expression in the form
     * of (ascii) string objects, this function parses the XML document,
     * evaluates the xpath and places the result as a class member,
     * as a vector of string objects. It can be fetched with
     * <code>result()</code> method.
     * @param expr XPath expression
     * @param context XML document context
     *  */
    void evaluate(const char* expr, const char* context);

    
    /** @brief This method returns XPath query result as a vector of strings<br>
     * If XPath result should return the only string, it is a first element
     * of the result vector.
     * @return XPath query result
     *  */
    const std::vector<std::string>& result() {
        return _result;
    }

private:

    // do not change initialization order!
    
    /** @brief Xalan internal RAII-initializer. <br>
     * Must be existed while XPath evaluation performs
     * and result nodeset under processing */
    XPathEvaluator _evaluator;

    /** @brief XPath result set */
    std::vector<std::string> _result;

    /** @brief XPath result set */
    XPathInit _xpath_wrapper;

    /** @brief XML file name */
    XalanDOMString _filename;

    /** @brief XPath context */
    XalanDOMString theContext;

    /** @brief XML input source */
    const LocalFileInputSource _input_source;
};


#if 0

template <typename T>
T evaluate_xpath(const char* xml_file,
        const char* path) {

    throw std::runtime_error("unable to convert");

}

template <>
int evaluate_xpath<int>(const char* xml_file,
        const char* path) {

    xpath x(xml_file);
    x.evaluate(path, "/");
    std::string s = x.result().at(0);
    return atoi(s.c_str());
}

template <>
std::string evaluate_xpath<std::string>(const char* xml_file,
        const char* path) {

    xpath x(xml_file);
    x.evaluate(path, "/");
    return x.result().at(0);

}

#endif

}


#endif	/* XPATH_H */

