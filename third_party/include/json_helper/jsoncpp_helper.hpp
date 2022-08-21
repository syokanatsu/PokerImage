#include <cinttypes>
#include <memory>
#include <string>

///
#include <json/json.h>  // NOLINT
namespace jsoncpp_helper {
// get string
inline std::string GetString(const Json::Value &value) {
  return value.isString() ? value.asString() : std::string("");
}
inline std::string GetString(const Json::Value &value, const std::string &key) {
  return value.get(key, std::string("")).asString();
}

// get bool
inline bool GetBool(const Json::Value &value) {
  return value.isBool() ? value.asBool() : false;
}
inline bool GetBool(const Json::Value &value, const std::string &key) {
  return value.get(key, false).asBool();
}

// get int
inline int GetInt(const Json::Value &value) {
  return value.isInt() ? value.asInt() : 0;
}
inline int GetInt(const Json::Value &value, const std::string &key) {
  return value.get(key, 0).asInt();
}

// get int64
inline int64_t GetInt64(const Json::Value &value) {
  return value.isInt64() ? value.asInt64() : 0;
}
inline int64_t GetInt64(const Json::Value &value, const std::string &key) {
  return value.get(key, 0).asInt64();
}

// get uint64
inline uint64_t GetUint64(const Json::Value &value) {
  return value.isUInt64() ? value.asUInt64() : 0;
}
inline uint64_t GetUint64(const Json::Value &value, const std::string &key) {
  return value.get(key, 0).asUInt64();
}

// get uint
inline uint32_t GetUint(const Json::Value &value) {
  return value.isUInt() ? value.asUInt() : 0;
}
inline uint32_t GetUint(const Json::Value &value, const std::string &key) {
  return value.get(key, 0).asUInt();
}

// get double
inline double GetDouble(const Json::Value &value) {
  return value.isDouble() ? value.asDouble() : 0;
}
inline double GetDouble(const Json::Value &value, const std::string &key) {
  return value.get(key, 0).asDouble();
}

// 规范读取，使用严格模式且原地解析
inline bool ReaderHelper(const std::string &json_info,
                         Json::Value &json_value) {  // NOLINT
  bool flag = false;
  do {
    if (json_info.empty()) {
      break;
    }
    Json::CharReaderBuilder reader_builder;
    Json::CharReaderBuilder::strictMode(&reader_builder.settings_);
    std::unique_ptr<Json::CharReader> const reader_ptr(
        reader_builder.newCharReader());
    if (nullptr == reader_ptr) {
      break;
    }
    if (!reader_ptr->parse(json_info.c_str(),
                           json_info.c_str() + json_info.size(), &json_value,
                           nullptr)) {
      break;
    }
    /// 2020.5.11 增加对Json::Value的空和非法情况的判定
    if (json_value.isNull() || !json_value.isObject()) {
      break;
    }
    flag = true;
  } while (false);
  return flag;
}
inline Json::Value ReaderHelper(const std::string &json_info) {
  Json::Value result;
  ReaderHelper(json_info, result);
  if (!result.isObject()) {
    decltype(result) temp_swap;
    result.swap(temp_swap);
  }
  return result;
}
// 规范写入，无格式
inline std::string WriterHelper(const Json::Value &json_value) {
  std::string json_info;
  Json::StreamWriterBuilder wbuilder;
  wbuilder.settings_["indentation"] = "";
  json_info = Json::writeString(wbuilder, json_value);
  return json_info;
}

inline void WriterHelper(const Json::Value &json_value,
                         std::string &json_info) {  // NOLINT
  json_info = WriterHelper(json_value);
}

}  // namespace jsoncpp_helper
