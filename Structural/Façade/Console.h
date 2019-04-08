#include <iostream>
#include <string>
#include <vector>
#include <memory>
//#include "Window.h"
//#include "TextBuffer.h"
using namespace std;


class Point 
{
public:
    Point() {}
    Point(int x, int y) {}
};


class TextBuffer
{
public:
    TextBuffer(int w, int h) {}
    void add_string(string& str, int idx){}
    int get_size() { return 0; }
};


class Viewport
{
public:
    Viewport(shared_ptr<TextBuffer> buffer) {}
    Viewport(shared_ptr<TextBuffer> buffer, int size, Point& start_p, Point& end_p) {}
};


class Window
{
public:
    Window(const string& title, int w, int h) {}
    vector<shared_ptr<TextBuffer>> buffers;
    vector<shared_ptr<Viewport>> viewports;

    void Show(){};
};


class Console
{
public:
  static Console& instance()
  {
    static Console console;
    return console;
  }

  vector<shared_ptr<Window>> windows;

  // static으로 만들 수 없다.
  shared_ptr<Window> single(const string& title, uint8_t charsWide, uint8_t charsHigh)
  {
    auto w = make_shared<Window>(title, charsWide * charWidth, charsHigh * charHeight);
    auto buffer = make_shared<TextBuffer>(charsWide, charsHigh);
    w->buffers.push_back(buffer);
    auto viewport = make_shared<Viewport>(buffer);
    w->viewports.push_back(viewport);
    windows.push_back(w);
    return w;
  }

  shared_ptr<Window> multicolumn(const string& title, uint8_t columnCount, uint8_t columnWidth, uint8_t charsHigh)
  {
    auto w = make_shared<Window>(title, columnCount * columnWidth * charWidth, charsHigh * charHeight);
    for (uint8_t c = 0; c < columnCount; ++c)
    {
      auto buffer = make_shared<TextBuffer>(columnWidth, charsHigh);
      w->buffers.push_back(buffer);
      auto viewport = make_shared<Viewport>(buffer, buffer->get_size(), Point{}, Point{ columnWidth*c, 0 });
      w->viewports.push_back(viewport);
      windows.push_back(w);
    }
    return w;
  }

private:
  const int charWidth = 10, charHeight = 14;
  Console(){}
  Console(Console const&) = delete;
  void operator=(Console const&) = delete;
};
