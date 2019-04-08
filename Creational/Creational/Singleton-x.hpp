


class Database
{
public:
  virtual int get_population(const std::string& name) = 0;
};

 class SingletonDatabase : public Database
 {
   SingletonDatabase() { /* 데이터베이스에서 데이터 읽어 들이기 */ }
   std::map<std::string, int> capitals;
 public:
   SingletonDatabase(SingletonDatabase const&) = delete;
   void operator=(SingletonDatabase const&) = delete;

   static SingletonDatabase& get()
   {
     static SingletonDatabase db;
     return db;
   }

   int get_population(const std::string& name) override
   {
     return capitals[name];
   }
 };

 struct SingletonRecordFinder
 {
   int total_population(std::vector<std::string> names)
   {
     int result = 0;
     for (auto& name : names)
       result += SingletonDatabase::get().get_population(name);
     return result;
   }
 };

 struct ConfigurableRecordFinder
 {
   explicit ConfigurableRecordFinder(Database& db)
     : db{db} {}

   int total_population(std::vector<std::string> names)
   {
     int result = 0;
     for (auto& name : names)
       result += db.get_population(name);
     return result;
   }

   Database& db;
 };


 class DummyDatabase : public Database
 {
   std::map<std::string, int> capitals;
 public:
   DummyDatabase()
   {
     capitals["alpha"] = 1;
     capitals["beta"] = 2;
     capitals["gamma"] = 3;
   }

   int get_population(const std::string& name) override {
     return capitals[name];
   }
 };