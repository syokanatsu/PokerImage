#include <functional>

class scoop_guard {
 public:
  using Destructor = std::function<void()>;
  explicit scoop_guard(Destructor des) : _des(des) {}
  ~scoop_guard() {
    if (_des) {
      _des();
    }
    sizeof(Destructor);
  }

 private:
  Destructor _des;
};
