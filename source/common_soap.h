#pragma once

#include "soapspProxy.h"
#include "sp.nsmap"

struct session_info
{
  session_info(soap* soap, const char* url) : soap { soap }, url { url }
  {
  }

  ~session_info()
  {
    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
  }

  soap* soap;
  std::string url;
};

struct auth_info
{
  auth_info(const char* databaseUser, const char* password, const char* spUser) :
    databaseUser { databaseUser }, password { password }, spUser { spUser }
  {
  }

  std::string databaseUser;
  std::string password;
  std::string spUser;
};
