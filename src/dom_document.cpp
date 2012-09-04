#include <stdexcept>
#include <errno.h>
#include "xmlutils/dom_document.h"

using namespace xerces;

// definitions group to wrap xerces exceptions to standard std exceptions
// and throw ot over the library
#define RETHROW_MESSAGE(e)     xerces::string x(e.getMessage());    \
    throw std::runtime_error(x.get_string().c_str());		    \

#define TRY_XERCES_EXCEPTIONS try {

#define RETHROW_XERCES_EXCEPTIONS }			\
    catch (const OutOfMemoryException& ex) {		\
    RETHROW_MESSAGE(ex)					\
    } catch (const XMLException& ex) {			\
    RETHROW_MESSAGE(ex)					\
    } catch (const DOMException& ex) {			\
    RETHROW_MESSAGE(ex)					\
    } catch (const std::exception& ex) {		\
    throw;       					\
    } catch (...) {					\
    std::cout << "Generic error occur" << std::endl;	\
    }							\

//---------------------------------------------------------------
void dom_document::open_document(const char* docname) {

    static XercesDOMParser::ValSchemes gValScheme = XercesDOMParser::Val_Auto;
    static bool gDoNamespaces = false;
    static bool gDoSchema = false;
    static bool gSchemaFullChecking = false;
    static bool gDoCreate = false;

    //  Parse the XML file, catching any XML exceptions that might propogate
    //  out of it.
    TRY_XERCES_EXCEPTIONS
    XercesDOMParser* parser = new XercesDOMParser;
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);
#if 0
    DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
    parser->setErrorHandler(errReporter);
#endif
    parser->parse(docname);
    _doc.assign(parser->getDocument());
    RETHROW_XERCES_EXCEPTIONS

}

//---------------------------------------------------------------
void dom_document::save_document() {
    if(!_filename.empty())
	save_document_as(_filename.c_str());
}

//---------------------------------------------------------------
void dom_document::save_document_as(const char* xml_filename) {
    TRY_XERCES_EXCEPTIONS
    // --- Create DOM model
    xerces::string x("LS 3.0");
    DOMImplementation* dom_impl =
	    DOMImplementationRegistry::getDOMImplementation(x.get_wchar());
    if(dom_impl == 0)
	throw std::runtime_error("DOM model does not supported");

    // --- DOM builder/writer factory class
    DOMImplementationLS* dom_ls = static_cast<DOMImplementationLS*> (dom_impl);

    // create DOM writer from factory
    DOMWriter *dom_writer = dom_ls->createDOMWriter();

    // --- set readable line ending
    if (dom_writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	dom_writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

    XMLFormatTarget *local_file_format_target =
	    (LocalFileFormatTarget*)new LocalFileFormatTarget(xml_filename);

    if (!dom_writer->writeNode(local_file_format_target, *_doc.get()))
	return;

    dom_writer->release();
    RETHROW_XERCES_EXCEPTIONS
}

//---------------------------------------------------------------
DOMElement* dom_document::create_node(const char* const element_name
	, const char* const node_value/* = 0*/
	, DOMElement* parent_element/* = 0*/) {

    DOMElement* childElement = 0;
    TRY_XERCES_EXCEPTIONS
    xerces::string x(element_name);
    childElement = _doc->createElement(x.get_wchar());

    // if no parent presented, create in root
    if (parent_element) {
	parent_element->appendChild(childElement);
    } else {
	_doc->getDocumentElement()->appendChild(childElement);
    }

    // if value presented, set it
    if (node_value) {
	DOMText* nodeValue = _doc->createTextNode(x.get_wchar());
	childElement->appendChild(nodeValue);
    }
    RETHROW_XERCES_EXCEPTIONS
    return childElement;
}

//---------------------------------------------------------------
void dom_document::create_attribute(DOMElement* node,
	const char* const attr_name,
	const char* const attr_value) {
    TRY_XERCES_EXCEPTIONS
    xerces::string x_attr_name(attr_name);
    xerces::string x_attr_value(attr_value);
    node->setAttribute(x_attr_name.get_wchar(), x_attr_value.get_wchar());
    RETHROW_XERCES_EXCEPTIONS
}

//---------------------------------------------------------------
void dom_document::set_attribute_value(DOMElement* node,
	const char* const attr_name,
	const char* const attr_value) {
    create_attribute(node, attr_name, attr_value);
}

//---------------------------------------------------------------
void dom_document::delete_node(DOMElement* delete_node) {

    TRY_XERCES_EXCEPTIONS
    _doc->removeChild(delete_node);
    RETHROW_XERCES_EXCEPTIONS
}

//---------------------------------------------------------------
DOMDocument* dom_document::create_dom_document(const char* root) {
    // --- Create DOM model
    DOMDocument* doc = 0;
    TRY_XERCES_EXCEPTIONS
    xerces::string str_root(root);
    xerces::string str1("Core");

    DOMImplementation* dom_impl =
	    DOMImplementationRegistry::getDOMImplementation(str1.get_wchar());

    // --- create DOM document with root
    doc = dom_impl->createDocument(0, str_root.get_wchar(), 0);

    RETHROW_XERCES_EXCEPTIONS
    return doc;
}

//---------------------------------------------------------------
DOMDocument* dom_document::create_ls_dom_document(const char* filename) {
    // --- Create DOM model
    DOMDocument* doc = 0;
    TRY_XERCES_EXCEPTIONS
    xerces::string str1("LS");

    DOMImplementation* dom_impl =
	    DOMImplementationRegistry::getDOMImplementation(str1.get_wchar());

    DOMImplementationLS* dom_impl_ls = DOMImplementationRegistry::getDOMImplementation(str1.get_wchar());

    // --- create DOM document with root
    DOMBuilder *dom_builder = dom_impl_ls->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
    doc = dom_builder->parseURI(filename);

    RETHROW_XERCES_EXCEPTIONS
    return doc;
}