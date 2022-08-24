#define EXPORT_POKERIMAGE
#include "PokerImage.h"

#include <gd.h>
#include <json/json.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <json_helper/jsoncpp_helper.hpp>

#include "generate_image.h"

namespace {
const std::unordered_map<std::string, ReqHandleFunc> map_res_type = {
    {"generate", GenerateImage}};
}  // namespace

void PokerImageProcess(const char* req_info, OnStartFunc on_start,
                       OnCallbackFunc on_callback) {
  Json::Value on_start_result;
  int start_result = 10000;
  on_start_result["start_result"] = start_result;
  const auto req_info_json = jsoncpp_helper::ReaderHelper(req_info);
  do {
    const auto& operation =
        jsoncpp_helper::GetString(req_info_json["operation"]);
    if (operation.empty()) {
      start_result = -2;
      break;
    }
    auto search = map_res_type.find(operation);
    if (search == map_res_type.end()) {
      start_result = 10;
      break;
    }
    start_result = search->second(req_info, on_callback);
  } while (false);
  on_start_result["start_result"] = start_result;
  const auto& on_start_result_str =
      jsoncpp_helper::WriterHelper(on_start_result);
  on_start(on_start_result_str.c_str());
}
