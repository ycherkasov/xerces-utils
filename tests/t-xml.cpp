#include <iostream>
#include <fstream>
#include <cassert>
#include <boost/shared_ptr.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <exception>
#include <xercesc/framework/LocalFileFormatTarget.hpp>


#include <gtest/gtest.h>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "xmlutils/dom_document.h"
#include "xmlutils/xpath.h"

XERCES_CPP_NAMESPACE_USE
        using namespace std;

// test plan:
//

class xerces_wrapper_test : public ::testing::Test
{
protected:
    virtual void SetUp() {
        // create XML file working copy
        XMLPlatformUtils::Initialize();
    }

    virtual void TearDown() {
        XMLPlatformUtils::Terminate();
    }

};

class xpath_wrapper_test : public ::testing::Test
{
protected:
    virtual void SetUp() {
        // create XML file working copy
    }

    virtual void TearDown() {
        // delete working copy
    }

};

// 1. Xerces wrappers
// 1.1 Create some DOM document (root and one node)

TEST_F(xerces_wrapper_test, create_dom_document)
{

    
    std::string sdoc("t-settings1.xml");
    std::string sset("settings1");
    std::string sval("value1");
    std::string sroot("root");
    // --- create DOM document with root
    {
        xerces::dom_document domDocument;
        // Use wrapper for types that must be released
        DOMElement* n1 = domDocument.create_node(sset.c_str(), sset.c_str());
        
        ASSERT_TRUE( n1 );
        domDocument.save_document_as(sdoc.c_str());
    }
    return;

    // --- check it with native Xerces API ---
    {
        //  Parse the XML file, catching any XML exceptions that might propogate
        //  out of it.
        try {
            boost::shared_ptr<XercesDOMParser> parser(new XercesDOMParser());
            parser->setValidationScheme(XercesDOMParser::Val_Auto);
            parser->setDoNamespaces(false);
            parser->setDoSchema(false);
            parser->setValidationSchemaFullChecking(false);
            parser->setCreateEntityReferenceNodes(false);
            parser->parse(sdoc.c_str());
            DOMDocument* doc = parser->getDocument();
            
            DOMElement* root = doc->getDocumentElement();
            

            // check root created
            const XMLCh* root_name = root->getTagName();
            xerces::string xroot(sroot.c_str());
            xerces::string xset1(sset.c_str());
            xerces::string xval1(sval.c_str());
            ASSERT_FALSE( XMLString::compareIString(root_name, xroot.get_wchar()) );

            // check child node created ond the one
            DOMNodeList * lnodes = root->getChildNodes();
            ASSERT_TRUE( lnodes );
            ASSERT_TRUE( lnodes->getLength() == 1 );
            

            // check node type
            DOMNode *node = lnodes->item(0);
            ASSERT_TRUE( node->getNodeType() == DOMNode::ELEMENT_NODE );
            DOMElement* el = static_cast<DOMElement*>(node);

            // check node name
            ASSERT_FALSE( XMLString::compareIString(el->getTagName(), xset1.get_wchar()) );

            // check node value
            ASSERT_FALSE( XMLString::compareIString(el->getTextContent(), xval1.get_wchar()) );
            

        }
        catch (const OutOfMemoryException& e) {
            xerces::string x(e.getMessage());
            FAIL() << x.get_string().c_str();
        }
        catch (const XMLException& e) {
            xerces::string x(e.getMessage());
            FAIL() << x.get_string().c_str();
        }
        catch (const DOMException& e) {
            xerces::string x(e.getMessage());
            FAIL() << x.get_string().c_str();
        }
        catch (const std::exception& e){
            FAIL() << e.what();
        }
        catch (...) {
            FAIL() << "Generic error occur";
        }

    }

}

// 1.2 Create and save DOM document

TEST_F(xerces_wrapper_test, create_save_dom_document) {
 }

// 1.3 Create DOM document (root, two nodes with two attributes both)

TEST_F(xerces_wrapper_test, create_dom_document_with_content) {
 }

// 1.4 Create DOM document from existing XML file, change attributes and save

TEST_F(xerces_wrapper_test, open_edit_dom_document1) {
 }

// 1.5 Create DOM document from existing XML file, delete node and save

TEST_F(xerces_wrapper_test, open_edit_dom_document2) {
 }

// Post-test. Delete test XML file
//
// 2. XPath wrappers
// 2.1 Evaluate xpath as string vector

TEST_F(xpath_wrapper_test, eval_as_string_vector) {
 }

// 2.2 Evaluate xpath as string

TEST_F(xpath_wrapper_test, eval_as_string) {
 }

// 2.3 Evaluate xpath as integer

TEST_F(xpath_wrapper_test, eval_as_int) {
 }

// 2.4 Evaluate xpath as double

TEST_F(xpath_wrapper_test, eval_as_double) {
 }

// 2.5 Evaluate xpath as boolean

TEST_F(xpath_wrapper_test, eval_as_boolean) {
 }

void test_wrapper1()
{

    // --- create DOM document with root
    xerces::dom_document domDocument;

    // Use wrapper for types that must be released
    domDocument.create_node("MyGreatNode");
    DOMElement* n1 = domDocument.create_node("MyGreatestNode", "ЧСВ9000");
    DOMElement* n2 = domDocument.create_node("MyGreatAttributeNode");
    domDocument.set_attribute_value(n2, "CHSV", "9000");
    domDocument.set_attribute_value(n2, "CHSV", "10000");
    domDocument.save_document_as("great_chsv.xml");
}

void test_wrapper2()
{

    // --- create DOM document with root
    xerces::dom_document domDocument("great_chsv.xml");

    // Use wrapper for types that must be released
    domDocument.create_node("MyGreatNode2");
    DOMElement* n1 = domDocument.create_node("MyGreatestNode", "ЧСВ9000");
    DOMElement* n2 = domDocument.create_node("MyGreatAttributeNode2");
    domDocument.set_attribute_value(n2, "CHSV", "90000");
    domDocument.save_document();
}

#if 0
void test_xpath1(const char* filename, const char* context, const char* expr)
{

    XALAN_USING_XERCES(XMLPlatformUtils);
    XALAN_USING_XERCES(XMLException);
    XALAN_USING_XALAN(XalanTransformer);

    using namespace xerces;
    ///////////////////////////////////////////..
    // create your XPath helper object
    xpath evaluator(filename);

    try {
        // call evaluate, passing in the XML string, the context string and the xpath string
        evaluator.evaluate(expr, context);


        const std::vector<std::string>& res = evaluator.result();
        // take the resulting string vector	and do whatever you want with it:

        cout << "the result set has " << res.size() << " strings\n";
        for (size_t i = 0; i < res.size(); i++)
            cout << "item " << (i + 1) << "= \"" << res[i] << "\"" << endl;
    }
    catch (const XERCES_CPP_NAMESPACE_QUALIFIER XMLException&) {
        cerr << "Exception caught!  Exiting..." << endl;
    }


}


    const NodeRefListBase& nodeSet = theResult->nodeset();
    for (unsigned i = 0; i < nodeSet.getLength(); i++) {
        XalanNode* node = nodeSet.item(i);

        // ensure its an element
        if (node->getNodeType() == XalanNode::ELEMENT_NODE) {
            XalanElement* element = static_cast<XalanElement*> (node);

            // Get element name
            const XalanDOMString& str = element->getTagName();

            // Get element name
            const XalanDOMChar* chstr = str.c_str();
            xerces::string x(chstr);
            cout << "String value: " << x.get_string().c_str() << endl;

            // Get element attributes map
            const XalanNamedNodeMap* nodeMap = element->getAttributes();
            for (unsigned i = 0; i < nodeMap->getLength(); i++) {
                XalanNode* attrNode = nodeMap->item(i);
                if (attrNode->getNodeType() == XalanNode::ATTRIBUTE_NODE) {
                    XalanAttr* attr = static_cast<XalanAttr*> (attrNode);
                    xerces::string attr_name(attr->getName().c_str());
                    xerces::string attr_val(attr->getValue().c_str());
                    cout << "Attr name: " << attr_name.get_string().c_str()
                            << " Attr value: " << attr_val.get_string().c_str()
                            << endl;
                }
            }
        }

    }
#endif




