#include <vector>
struct Document;

//struct IMachine
//{
//  virtual void print(Document& doc) = 0;
//  virtual void fax(Document& doc) = 0;
//  virtual void scan(Document& doc) = 0;
//};
//
//struct MFP : IMachine
//{
//  void print(Document& doc) override;
//  void fax(Document& doc) override;
//  void scan(Document& doc) override;
//};

// 1. 재검파일
// 2. 클라이언트는 이것을 필요로 하지 않는다
// 3. 구현자에게 불필요하게 많은 것을 구현하게 강제한다

struct IPrinter
{
  virtual void print(Document& doc) = 0;
};

struct IScanner
{
  virtual void scan(Document& doc) = 0;
};

struct Printer : IPrinter
{
  void print(Document& doc) override;
};

struct Scanner : IScanner
{
  void scan(Document& doc) override;
};

struct IMachine: IPrinter, IScanner
{
};

struct Machine : IMachine
{
  IPrinter& printer;
  IScanner& scanner;

  Machine(IPrinter& printer, IScanner& scanner)
    : printer{printer},
      scanner{scanner}
  {
  }

  void print(Document& doc) override {
    printer.print(doc);
  }
  void scan(Document& doc) override;
};

// IPrinter --> Printer
// everything --> Machine