#include "generate_image.h"

#include <gd.h>
#include <thread>  // NOLINT
#include <vector>
#include <queue>
#include <json_helper/jsoncpp_helper.hpp>
#include <tools/scoop_guard.hpp>
namespace {
const auto CardWidth = 67.46153846153846;
const auto CardHeight = 103.2;
const auto ShowWidth = 13;
const auto suits = {"spade", "heart", "club", "diamond"};
enum POKER_SUIT { SPADE = 0, HEART, CLUB, DIAMOND };
}  // namespace

int GenerateImage(const std::string& req, OnCallbackFunc on_callback) {
  int result = -1;
  do {
    Json::Value root;
    if (!on_callback || !jsoncpp_helper::ReaderHelper(req, root)) {
      result = 2;
      break;
    }
    const auto& output = jsoncpp_helper::GetString(root, "output");
    // 输出路径不能为空
    if (output.empty() || output.back() != '/') {
      result = 3;
      break;
    }
    Json::Value& cards = root["cards"];
    if (cards.type() != Json::arrayValue) {
      result = 4;
      break;
    }
    const auto& size = cards.size();
    if (size <= 0) {
      result = 5;
      break;
    }
    std::thread([cards, size, output, on_callback]() {
      int error = -1;
      const auto width = (size - 1) * ShowWidth + CardWidth;
      const auto height = CardHeight;
      FILE* png_out = nullptr;
      FILE* png_in = nullptr;
      struct stat stat_buf;
      gdImagePtr im;
      do {
        png_in = fopen("POKER.png", "rb");
        if (!png_in) {
          error = 100;
          break;
        }
        // 保护文件png_in关闭
        scoop_guard guard_in([png_in]() { fclose(png_in); });
        if (fstat(fileno(png_in), &stat_buf) != 0) {
          error = 101;
          break;
        }
        /* Read the entire thing into a buffer
          that we allocate */
        char* buffer = (char*)malloc(stat_buf.st_size);  // NOLINT
        if (!buffer) {
          error = 102;
          break;
        }
        // 保护内存buffer释放
        scoop_guard guard_buffer([buffer]() { free(buffer); });
        if (fread(buffer, 1, stat_buf.st_size, png_in) != stat_buf.st_size) {
          /* Error */
          error = 103;
          break;
        }
        im = gdImageCreateFromPngPtr(stat_buf.st_size, buffer);
        // 保护im
        scoop_guard guard_im([im]() { gdImageDestroy(im); });
        // 创建输出图像
        gdImagePtr im_out = nullptr;
        im_out = gdImageCreate(200, 200);
        scoop_guard guard_im_out([im_out]() { gdImageDestroy(im_out); });
        auto black = gdImageColorAllocate(im, 0, 0, 0);
        // 循环遍历cards，向im_out中绘制图像
        bool succeed = true;
        struct Card {
          int number;
          bool hide;
          POKER_SUIT suit;
        };
        // card排序算法
        struct CompCard {
          bool operator()(const Card& c1, const Card& c2) {
            return (c1.number == c2.number) ? c1.suit < c2.suit
                                            : c1.number < c2.number;
          }
        };
        std::priority_queue<Card, std::vector<Card>, CompCard> card_queue;
        for (const auto& card : cards) {
          const auto& number = jsoncpp_helper::GetInt(card, "number");
          const auto& show_back = jsoncpp_helper::GetBool(card, "hide");
          const auto& suit = jsoncpp_helper::GetString(card, "suit");
          if (number <= 0 || number > 15) {
            succeed = false;
            break;
          }
          // 未知花色
          auto card_suit_iter = std::find(suits.begin(), suits.end(), suit);
          if (card_suit_iter == suits.end()) {
            break;
          }
          card_queue.push(
              {number, show_back,
               (POKER_SUIT)std::distance(suits.begin(), card_suit_iter)});
        }
        // 遍历出现问题
        if (!succeed) {
          break;
        }
        int current_idx = 0;
        while (!card_queue.empty()) {
          auto card = card_queue.top();
          card_queue.pop();
          // 获取牌图片
          gdRect rt_in = {0, 0, CardWidth, CardHeight};
          rt_in.x = card.number > 13 ? (card.number - 14) * CardWidth
                                     : (card.number - 1) * CardWidth;
          rt_in.y = card.number > 13 ? 4 * CardHeight : card.suit * CardHeight;
          auto card_im_ptr = gdImageCrop(im, &rt_in);
          scoop_guard guard_card_im(
              [card_im_ptr]() { gdImageDestroy(card_im_ptr); });
          // 绘制区域
          gdRect rt = {0, 0, CardWidth, CardHeight};
          rt.x = current_idx++ * ShowWidth;
          gdImageCopy(im_out, card_im_ptr, rt.x, rt.y, 0, 0, CardWidth,
                      CardHeight);
        }
        // 保存到文件
        png_out = fopen("test.jpg", "wb");
        if (!png_out) {
          break;
        }
        scoop_guard guard_png_out([png_out]() { fclose(png_out); });
        gdImageJpeg(im_out, png_out, -1);
        error = 0;
      } while (false);

      Json::Value root;
      root["error_code"] = error;
      on_callback(jsoncpp_helper::WriterHelper(root).c_str());
    }).detach();
    result = 0;
  } while (false);
  return result;
}
