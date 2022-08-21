#pragma once
#include <iostream>
#include "defines.h"

extern "C" {

/**
 * @description: pokerimage交互接口，接受通过json字符串进行通讯，可以支持多语言跨平台使用
 * @param {char*} req_info 请求参数的json字符串
 * @param {OnStartFunc} on_start 请求初步检测结果信息回调*同步回调
 * @param {OnCallbackFunc} on_callback 请求数据处理结果回调*异步回调
 * @return {*}
 */
POKER_API void PokerImageProcess(const char* req_info, OnStartFunc on_start,
                             OnCallbackFunc on_callback);
}
