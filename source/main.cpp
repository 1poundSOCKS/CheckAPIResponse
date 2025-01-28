#include "pch.h"

using namespace xercesc;

#pragma warning (disable:4459)

template<typename inserter_type>
auto Extract(DOMDocument* doc, DOMElement* rootElement, DOMXPathNSResolver *nsResolver, std::string expression, inserter_type inserter) -> void
{
  auto expressionToEvaluate = XMLString::transcode(expression.c_str());
  DOMXPathResult* result=doc->evaluate(expressionToEvaluate, rootElement, nsResolver, DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE, NULL);

  DOMNode* node = result->getNodeValue();
  DOMNode* child = node->getFirstChild();
  DOMNodeList* children = node->getChildNodes();
  auto childCount = children->getLength();
  
  for( int childIndex = 0; childIndex < childCount; ++childIndex )
  {
    DOMNode* childNode = children->item(childIndex);
    DOMNode* childText = childNode->getFirstChild();

    auto nodeName = childNode->getNodeName();
    auto childValue = childText->getNodeValue();

    std::string nameStr = XMLString::transcode(nodeName);
    std::string valueStr = XMLString::transcode(childValue);

    inserter = { nameStr, valueStr };
  }
}

auto main(int argc, char* argv[]) -> int
{
  for( int arg = 0; arg < argc; ++arg )
  {
    std::cout << argv[arg] << ' ';
  }

  std::cout << '\n';

  if( argc != 3 )
  {
    return 0;
  }

  std::cout << "CheckAPIResponse started" << '\n';
  const char* xmlFile = argv[1];
  const char* responseTag = argv[2];

  auto config = LoadDefaultConfig();

  try
  {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException&)
  {
    return 1;
  }

  auto tempStr = XMLString::transcode("LS");
  DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
  XMLString::release(&tempStr);
  DOMLSParser* parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMValidate, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMValidate, true);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMNamespaces, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, true);

  if (parser->getDomConfig()->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true))
      parser->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);

  DOMDocument *doc = 0;

  try
  {
    doc = parser->parseURI(xmlFile);
    auto rootElement = doc->getDocumentElement();
    auto tagName = rootElement->getTagName();
    char* temp2 = XMLString::transcode(tagName);
    std::cout << temp2 << '\n';

    DOMXPathNSResolver* nsResolver = doc->createNSResolver(rootElement);
    nsResolver->addNamespaceBinding(XMLString::transcode("SOAP-ENV"), XMLString::transcode("http://www.w3.org/2003/05/soap-envelope"));
    nsResolver->addNamespaceBinding(XMLString::transcode("sp2"), XMLString::transcode("http://www.servicepower.com/sp.xsd1"));
    
    std::map<std::string, std::string> values;
    std::string tag1 = std::string("/SOAP-ENV:Envelope/SOAP-ENV:Body/sp2:") + std::string(responseTag) + std::string("/result");
    // std::string tag2 = std::string("/SOAP-ENV:Envelope/SOAP-ENV:Body/sp2:") + std::string(responseTag) + std::string("/jobDetails/additionalInformation/forceOptions");
    Extract(doc, rootElement, nsResolver, tag1, std::inserter(values, std::begin(values)));
    // Extract(doc, rootElement, nsResolver, tag2, std::inserter(values, std::begin(values)));

    for( auto&& entry : values )
    {
      std::cout << entry.first << std::string_view(": ") << entry.second << '\n';
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
