/* 
 * File:   xpath.cpp
 * Author: ycherkasov
 *
 * Created on 13 Ноябрь 2010 г., 14:53
 */


#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <xalanc/Include/STLHelper.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/PlatformSupport/DirectoryEnumerator.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/DOMSupport/DOMServices.hpp>
#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>

#include "xmlutils/xpath.h"
#include "xmlutils/xmlstring.h"


XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostream)

using namespace xerces;

xpath::xpath(const std::string& filename)
: _xpath_wrapper()
, _filename(filename.c_str())
, _input_source(_filename.c_str()) { }

xpath::~xpath() { }

void xpath::evaluate(const char* expr, const char* context)
{

    // Just hoist everything...
    XALAN_CPP_NAMESPACE_USE
    XALAN_USING_XERCES(XMLException);
    XALAN_USING_XERCES(LocalFileInputSource);
    typedef XPathConstructionContext::GetAndReleaseCachedString GetAndReleaseCachedString;

    // Create XPath execution context
    XalanElement* rootElem = 0;
    helper_t xpath_helper(rootElem);


    // configure the objects needed for XPath to work with the Xerces DOM
    XalanDocument * const doc = xpath_helper._liason_wrapper.parseXMLStream(_input_source);
    assert(doc != 0);

    rootElem = doc->getDocumentElement();
    assert(rootElem != 0);

    // first get the context nodeset
    XalanDOMString xpath_context(context);
    xpath_helper.init(xpath_context);

    XObjectPtr xObj = xpath_helper._xpath_context->execute(rootElem
            , xpath_helper._prefix_resolver
            , xpath_helper._exec_context);

    const NodeRefListBase& contextNodeList = xObj->nodeset();
    const unsigned int theLength = contextNodeList.getLength();

    if (theLength == 0) {
        std::ostringstream err;
        xerces::string x(xpath_context.c_str());
        err << "Emplty nodeset in context " << x.get_string();
        throw std::runtime_error(err.str().c_str());
    }
    if (theLength > 1) {
        // todo : warning multiple nodeset
    }
    else {
        // and now get the result of the primary xpath expression
        XPath * const xpath = xpath_helper._xpath_factory.create();

        xpath_context = expr;
        xpath_helper._xpath_processor.initXPath(*xpath,
                xpath_helper._construction_context,
                xpath_context,
                xpath_helper._prefix_resolver);

        xObj = xpath->execute(contextNodeList.item(0),
                xpath_helper._prefix_resolver,
                xpath_helper._exec_context);
    }

    // now encode the results.  For all types but nodelist, 
    // we'll just convert it to a string, but, for nodelist,
    // we'll convert each node to a string and return a list of them
    XObject::eObjectType type = xObj->getType();
    if (xObj->getType() == XObject::eTypeNodeSet) {
        const NodeRefListBase& nodeset = xObj->nodeset();
        size_t len = nodeset.getLength();

        for (size_t i = 0; i < len; i++) {
            XalanNode * const node = nodeset.item(i);
            XalanDOMString str;

            const int theType = node->getNodeType();

            if (theType == XalanNode::COMMENT_NODE ||
                    theType == XalanNode::PROCESSING_INSTRUCTION_NODE)
                str = node->getNodeValue();
            else if (theType == XalanNode::ELEMENT_NODE)
                str = node->getNodeName();
            else
                DOMServices::getNodeData(*node, str);
            xerces::string res_string(str.c_str());
            _result.push_back(res_string.get_string());
        }
    }
    else {
        xerces::string res_string(xObj->str().c_str());
        _result.push_back(res_string.get_string());
    }
}

#if 0

XObjectPtr xpath::eval(const char* filename, const char* context, const char* expr)
{
    XALAN_USING_XERCES(XMLPlatformUtils)
    XALAN_USING_XERCES(LocalFileInputSource)
    XALAN_USING_XALAN(XalanDocument)
    XALAN_USING_XALAN(XalanDocumentPrefixResolver)
    XALAN_USING_XALAN(XalanDOMString)
    XALAN_USING_XALAN(XalanNode)
    XALAN_USING_XALAN(XalanSourceTreeInit)
    XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
    XALAN_USING_XALAN(XalanSourceTreeParserLiaison)
    XALAN_USING_XALAN(XObjectPtr)

    // Initialize the XalanSourceTree subsystem...
    XalanSourceTreeInit theSourceTreeInit;

    // We'll use these to parse the XML file.
    XalanSourceTreeDOMSupport theDOMSupport;
    XalanSourceTreeParserLiaison theLiaison(theDOMSupport);

    // Hook the two together...
    theDOMSupport.setParserLiaison(&theLiaison);

    const XalanDOMString theFileName(filename); //XMLFilePath

    // Create an input source that represents a local file...
    const LocalFileInputSource theInputSource(theFileName.c_str());

    // Parse the document...
    XalanDocument * const theDocument = theLiaison.parseXMLStream(theInputSource);
    assert(theDocument != 0);

    XalanDocumentPrefixResolver thePrefixResolver(theDocument);

    // OK, let's find the context node...
    XalanNode * const theContextNode =
            theEvaluator.selectSingleNode(
            theDOMSupport,
            theDocument,
            XalanDOMString(context).c_str(), //context
            thePrefixResolver);

    if (theContextNode != 0) {
        // OK, let's evaluate the expression...
        const XObjectPtr theResult(theEvaluator.evaluate(theDOMSupport
                , theContextNode, XalanDOMString(expr).c_str() //XPathExpression
                , thePrefixResolver));

        assert(theResult.null() == false);
        return theResult;
    }
}
#endif

