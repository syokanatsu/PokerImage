#include "generate_image.h"

#include <future>
#include <json_helper/jsoncpp_helper.hpp>

int GenerateImage(const std::string& req, OnCallbackFunc on_callback) {
  int result = -1;
  do {
    Json::Value root;
    if (!on_callback || !jsoncpp_helper::ReaderHelper(req, root)) {
      break;
    }
    const auto& output = jsoncpp_helper::GetString(root, "output");
    // 输出路径不能为空
    if (output.empty() || output.back() != '/') {
      break;
    }
    Json::Value& cards = root["cards"];
    if (cards.type() != Json::arrayValue) {
      break;
    }
    const auto& size = cards.size();
    std::async(std::launch::async, [cards, size, output, on_callback](){
      
    });
    result = 0;
  } while (false);
  return result;
}
