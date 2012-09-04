/* 
 * File:   dom_document.h
 * Author: ycherkasov
 *
 * Created on 8 Ноябрь 2010 г., 13:54
 */

#ifndef DOM_DOCUMENT_H
#define	DOM_DOCUMENT_H

#include <iostream>
#include <boost/noncopyable.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "xmlutils/xmlstring.h"

namespace xerces {

 /**  @brief This class implements the Document Object Model (DOM) wrapper.<br>
  * It should be used by applications which uses Xerces-c and Xalan
  * Apache libraries to implement easy DOM-document processing.
  * The RAII-wrappers allows not to mean about resource initialisation and
  * release. Facade between Xerces-c and Xalan interfaces simplifies
  * DOM-document creartion, editing and making queries.
  *
  * For example, instead of using native Xerces methods to create DOM-document
  * @code
  * DOMDocument* doc = 0;
  * xerces::string str1("LS");
  * DOMImplementation* dom_impl =
  * DOMImplementationRegistry::getDOMImplementation(str1.get_wchar());
  * DOMImplementationLS* dom_impl_ls
  *	= DOMImplementationRegistry::getDOMImplementation(str1.get_wchar());
  * // --- create DOM document with root
  * DOMBuilder *dom_builder
  *	= dom_impl_ls->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  * doc = dom_builder->parseURI(filename);
  * return doc;
  * @endcode
  *
  * you can just use a wrapper creation
  * @code
  * xerces::dom_document domDocument;
  * @endcode
  * 
  */
class dom_document : boost::noncopyable {

public:

    
    /** @brief Default constructor
    //@{

     * Construct a new DOM document.
     *
     * Default constructor creates a new DOM document, with LS
     * (load and save support) and DOM 3.0 level support.
     * You have to call <code>save_document_as()</code> method
     * to save in as a file.
     * @code
     * xerces::dom_document domDocument;
     * DOMElement* settings_node = domDocument.create_node("settings");
     * DOMElement* srv_settings = domDocument.create_node("server_settings", "127.0.0.1", settings_node);
     * DOMElement* color_settings = domDocument.create_node("color_settings", settings_node);
     * domDocument.create_attribute(color_settings, "line", "0xFFCC00FF");
     * domDocument.create_attribute(color_settings, "background", "0xFFFFCCFF");
     * domDocument.save_document_as("settings.xml");
     * @endcode
     */
    dom_document()
    : _doc(create_dom_document("root")) {
    }

    
    /** @brief Existing XML-document constructor
    //@{

     * Construct a DOM-document from existing XML file.
     *
     * Default constructor creates a DOM document from existing XML-file
     * , with LS (load and save support) and DOM 3.0 level support.
     *
     * @param filename XML file name
     */
    dom_document(const char* filename)
    : _filename(filename) {
	open_document(filename);
    }

protected:

    
    /** @brief This static method returns a new DOMDocument object.<br>
     * It must be released with <code>DOMDocument::release() method</code>, so
     * don't use it without a special RAII wrapper:<br>
     * <code>xerces_auto_ptr<DOMDocument> wrapper(create_dom_document());</code>
     *  */
    static DOMDocument* create_dom_document(const char* root);

    
    /** @brief This static method returns a new DOMDocument object with LS support.<br>
     * It must be released with <code>DOMDocument::release() method</code>, so
     * don't use it without a special RAII wrapper:<br>
     * <code>xerces_auto_ptr<DOMDocument> wrapper(create_ls_dom_document());</code>
     *  */
    static DOMDocument* create_ls_dom_document(const char* filename) ;

public:

    
    /** @brief This method loads a new DOMDocument into object.<br> 
     * Existing object will be released without save, so,
     * you should save it before with method <code>save_document()</code> or
     * <code>save_document_as()</code>
     * @param xml_filename XML file name
     *  */
    void open_document(const char* xml_filename);

    
    /** @brief This method saves current DOMDocument as an XML file<br>
     * It should be saved before with <code>save_document_as()</code> method
     * or opened as existing document, or it doesn't teake any effect.
     *  */
    void save_document();

    
    /** @brief This method saves current DOMDocument as an XML file with
     * the name produced.
     *
     * @param xml_filename XML file name
     *  */
    void save_document_as(const char* xml_filename);

    
    /** @brief This method creates new XML-node elsewhere in document hierarchy.*/
    /** If a node with the same name has already been created,
     * it diplicates. If node with the same name and existing value has been
     * created, result is undefined. Once more - don"t try to rewrite
     * node value, remove it and try to create new instead.
     * @code
     * DOMElement* n1 = domDocument.create_node("Node1", "Value1");
     * DOMElement* n2 = domDocument.create_node("Node1", "Value2");
     * // Value hasn't been rewritten!
     * @endcode
     * @param element_name string name of node
     * @param node_value string value of node - optional
     * @param parent_element pointer to parent element, document root by default
     * @return created node pointer
     *  */
    DOMElement* create_node(const char* const element_name
	    , const char* const node_value = 0
	    , DOMElement* parent_element = 0);

    
    /** @brief This method creates new node attribute with new value*/
    /** If an attribute with the same name has already been created,
     * its value rewrites.
     * @param node pointer to node containing the attribute
     * @param attr_name string attribute name
     * @param attr_value string attribute value
     *  */
    void create_attribute(DOMElement* node,
	    const char* const attr_name,
	    const char* const attr_value);

    
    /** @brief This method sets the new attribute value */
    /** It completely duplucates functionality of
     * <code>create_attribute()</code>, but it is more
     * conviniently for library users.
     * @param node pointer to node containing the attribute
     * @param attr_name string attribute name
     * @param attr_value string attribute value
     *  */
    void set_attribute_value(DOMElement* node,
	    const char* const attr_name,
	    const char* const attr_value);

    
    /** @brief Delete node by the pointer provided */
    /**
     * @param delete_node node pointer to delete
     *  */
    void delete_node(DOMElement* delete_node);

private:
    /** @brief RAII-wrapper under the DOMDocument object */
    xerces_auto_ptr<DOMDocument> _doc;

    /** @brief Related XML-file name */
    std::string _filename;
};

}


#endif	/* DOM_DOCUMENT_H */

