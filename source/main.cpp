#include "pch.h"

using namespace xercesc;

#pragma warning (disable:4459)

auto main(int argc, char* argv[]) -> int
{
  for( int arg = 0; arg < argc; ++arg )
  {
    std::cout << argv[arg] << ' ';
  }

  std::cout << '\n';

  if( argc != 1 )
  {
    return 0;
  }

  std::cout << "CheckAPIResponse started" << '\n';

  auto config = LoadDefaultConfig();

  try
  {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException&)
  {
    return 1;
  }

  XMLCh tempStr[100];
  XMLString::transcode("LS", tempStr, 99);
  DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
  DOMLSParser* parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMValidate, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMValidate, true);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMNamespaces, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, true);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);

  const char* xmlFile = "test/data/GetJob_response.xml";
  DOMDocument *doc = 0;

  try
  {
    doc = parser->parseURI(xmlFile);
    auto rootElement = doc->getDocumentElement();
    auto tagName = rootElement->getTagName();
    char* temp2 = XMLString::transcode(tagName);
    std::cout << temp2 << '\n';

    auto nsResolver = doc->createNSResolver(rootElement);
    nsResolver->addNamespaceBinding(XMLString::transcode("SOAP-ENV"), XMLString::transcode("http://www.w3.org/2003/05/soap-envelope"));
    nsResolver->addNamespaceBinding(XMLString::transcode("sp2"), XMLString::transcode("http://www.servicepower.com/sp.xsd1"));

    DOMXPathResult* result=doc->evaluate(
        XMLString::transcode("/SOAP-ENV:Envelope/SOAP-ENV:Body/sp2:GetJobResponse/result/type"),
        rootElement,
        nsResolver,
        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
        NULL);

    DOMNode* node = result->getNodeValue();
    DOMNode* child = node->getFirstChild();
    DOMNode::NodeType type = child->getNodeType();

    if( type == DOMNode::TEXT_NODE )
    {
      auto childValue = child->getNodeValue();
      std::cout << XMLString::transcode(childValue) << '\n';
    }

    if (result->getNodeValue() == NULL)
    {
      std::cout << "There is no result for the provided XPath " << '\n';
    }
    else
    {
      // auto nodeValue = result->getNodeValue()->getFirstChild()->getNodeValue();
      // auto value = XMLString::transcode(result->getNodeValue());
      // std::cout << value << '\n';
    }
  }
  catch (const XMLException& toCatch)
  {
      char* message = XMLString::transcode(toCatch.getMessage());
      std::cout << "Exception message is: \n" << message << "\n";
      XMLString::release(&message);
      return -1;
  }
  catch (const DOMException& toCatch)
  {
      char* message = XMLString::transcode(toCatch.msg);
      std::cout << "Exception message is: \n" << message << "\n";
      XMLString::release(&message);
      return -1;
  }
  catch (...)
  {
      std::cout << "Unexpected Exception \n" ;
      return -1;
  }

  parser->release();

  XMLPlatformUtils::Terminate();

  std::cout << "CheckAPIResponse closing" << '\n';

  return 0;
}
