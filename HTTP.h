/*
* Copyright (c) 2013-2016, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#ifndef HTTP_H__
#define HTTP_H__

#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace i2p {
namespace http {
  const char CRLF[] = "\r\n";         /**< HTTP line terminator */
  const char HTTP_EOH[] = "\r\n\r\n"; /**< HTTP end-of-headers mark */
  extern const std::vector<std::string> HTTP_METHODS;  /**< list of valid HTTP methods */
  extern const std::vector<std::string> HTTP_VERSIONS; /**< list of valid HTTP versions */

  struct URL {
    std::string schema;
    std::string user;
    std::string pass;
    std::string host;
    unsigned short int port;
    std::string path;
    std::string query;
    std::string frag;

    URL(): schema(""), user(""), pass(""), host(""), port(0), path(""), query(""), frag("") {};

    /**
     * @brief Tries to parse url from string
     * @return true on success, false on invalid url
     */
    bool parse (const char *str, size_t len = 0);
    bool parse (const std::string& url);

    /**
     * @brief Parse query part of url to key/value map
     * @note Honestly, this should be implemented with std::multimap
     */
    bool parse_query(std::map<std::string, std::string> & params);

    /**
     * @brief Serialize URL structure to url
     * @note Returns relative url if schema if empty, absolute url otherwise
     */
    std::string to_string ();
  };

  struct HTTPMsg {
    std::map<std::string, std::string> headers;
  };

  struct HTTPReq : HTTPMsg {
    std::string version;
    std::string method;
    std::string uri;
    std::string host;

    HTTPReq (): version("HTTP/1.0"), method("GET"), uri("/") {};

    /**
     * @brief Tries to parse HTTP request from string
     * @return -1 on error, 0 on incomplete query, >0 on success
     * @note Positive return value is a size of header
     */
    int parse(const char *buf, size_t len);
    int parse(const std::string& buf);

    /** @brief Serialize HTTP request to string */
    std::string to_string();
  };

  struct HTTPRes : HTTPMsg {
    std::string version;
    std::string status;
    unsigned short int code;

    HTTPRes (): version("HTTP/1.1"), status("OK"), code(200) {}

    /**
     * @brief Tries to parse HTTP response from string
     * @return -1 on error, 0 on incomplete query, >0 on success
     * @note Positive return value is a size of header
     */
    int parse(const char *buf, size_t len);
    int parse(const std::string& buf);

    /** @brief Serialize HTTP response to string */
    std::string to_string();

    /** @brief Checks that response declared as chunked data */
    bool is_chunked();

    /** @brief Returns declared response length or -1 if unknown */
    long int length();
  };

  /**
   * @brief returns HTTP status string by integer code
   * @param code HTTP code [100, 599]
   * @return Immutable string with status
   */
  const char * HTTPCodeToStatus(int code);

  /**
   * @brief Replaces %-encoded characters in string with their values
   * @param data Source string
   * @param null If set to true - decode also %00 sequence, otherwise - skip
   * @return Decoded string
   */
  std::string UrlDecode(const std::string& data, bool null = false);
} // http
} // i2p

#endif /* HTTP_H__ */
