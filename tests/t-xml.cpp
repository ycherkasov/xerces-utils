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

