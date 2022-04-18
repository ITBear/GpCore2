GPCore - C++ library of low level components (reflection, multithreading, fibers, safe math etc.)
===================================

GPCore is a C++20 library of low level components written for wide use cases. There are libraries built on top
of GPCore like [GpJson](https://github.com/ITBear/GpJson) (classes mapper from/to json), [GpNetwork](https://github.com/ITBear/GpNetwork) (tcp, udp, http asynchronous server),
[GpDbConnector](https://github.com/ITBear/GpDbConnector) (lightweight database ORM) and [other](#libraries-are-built-on-top-of-gpcore).

Library can be [configured and build](#build-notes) for Linux, Windows, MacOS, iOS, Android, Browser(wasm) and bare metal (cortex-m).

# Key features

- **Reflection**. Provides reflection tools. With it it is possible to bind class fields to a data-model descriptor and store it into a manager.
Data-model manager gives R/W access to class fields through field names and access to data-model description. Reflection tools also support
inheritance (not multiple), fully dynamic "on-fly" data-model builder. See [examples](#using-reflection) or [documentation](https://github.com/ITBear/GpCore2/Doc/Reflection.md).

- **Fibers**. Provides tools for work with fibers: task manager, tasks that suspend and resume execution at certain locations (manual or by events).
See [examples](#using-fibers) or [documentation](https://github.com/ITBear/GpCore2/Doc/Fibers.md).

- **Safe math**.  Provides tools for work safe with arithmetic types (std::is_arithmetic). For all operations that can cause
overflow (addition, multiplication) or undefined behavior (overflows) library provides safe wrappers.If possible, checks will be
done at a compile time, if not then at a runtime. If a runtime operation cause an error, then an exception will be thrown.
[examples](#using-safe-math) or [documentation](https://github.com/ITBear/GpCore2/Doc/NumericOps.md).

- **Service lifetime** (start, read config, start internal tasks, OS signals processing, stop)

* UUIDs (include [version 7](https://www.ietf.org/archive/id/draft-peabody-dispatch-new-uuid-format-03.html))
* Encoders (Base64, Base58)
* String conversion
* Enums
* Date/time
* Measure unit types

### Using reflection

[Documentation](https://github.com/ITBear/GpCore2/Doc/Reflection.md)

Let's assume you have class:
```cpp
class MyDataStruct
{
public:
   string         str;
   int32_t        i32 = 0;
   vector<string> vec_str;
}
```

Make it reflectable:
- Inherits from GpTypeStructBase
- Macros CLASS_DECLARE_DEFAULTS declares containers with MyDataStruct like shares pointers, vectors etc. (can be used: MyDataStruct::C::Vec::Val is equal to std::vector<MyDataStruct>)
- Macros TYPE_STRUCT_DECLARE declares all the reflection 'magic'. As in argument it takes UUID for data-model structure. By this UUID data-model can be accessed from the manager.
- Declares default, copy and mode constructors
- Macros TYPE_STRUCT_IMPLEMENT implements all the reflection 'magic'. As in arguments it takes name of the class (MyDataStruct) and group UUID for data-model structure
 (GP_MODULE_UUID - is just the global definition of "07d0903e-1195-4dcf-92d7-4f0db3f7e4f9"_sv). '_sv' is string literal for std::string_view
- In method _SCollectStructProps binds class fields to data-model
```cpp
class MyDataStruct final: public GpTypeStructBase
{
public:
   CLASS_DECLARE_DEFAULTS(MyDataStruct)
   TYPE_STRUCT_DECLARE("2bfadcf2-fe2a-4e10-a86c-78580fa1b6bb"_sv)

public:           
                  MyDataStruct  (void) noexcept {}
                  MyDataStruct  (const MyDataStruct& aDesc):
                   str(aDesc.str), i32(aDesc.i32), vec_str(aDesc.vec_str) {}
                  MyDataStruct  (MyDataStruct&& aDesc) noexcept:
                   str(std::move(aDesc.str)), i32(aDesc.i32), vec_str(std::move(aDesc.vec_str)) {}
   virtual        ~MyDataStruct (void) noexcept override final {}

public:
   string         str;
   int32_t        i32 = 0;
   vector<string> vec_str;
};

TYPE_STRUCT_IMPLEMENT(MyDataStruct, GP_MODULE_UUID)

void    MyDataStruct::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
   PROP(str);
   PROP(i32);
   PROP(vec_str);
}
```

After this you can get R/W access to fields throughout manager

```cpp
...
void    Foo (GpTypeStructBase& aStruct)
{
   //Get data-model info
   const GpTypeStructInfo& typeStructInfo = aStruct.TypeInfo();

   //Get references to class fields
   auto& str     = typeStructInfo.Prop("str"_sv).Value_String(aStruct.DataPtr());
   auto& i32     = typeStructInfo.Prop("i32"_sv).Value_UInt32(aStruct.DataPtr());
   auto& vec_str = typeStructInfo.Prop("vec_str"_sv).Vec_String(aStruct.DataPtr());

   //Read values from fields
   std::cout
       << str << "\n"
       << i32 << "\n"
       << vec_str.at(0) << "\n" << std::endl;

   //Write values to fields
   str = "456";
   i32 = -456;
   vec_str = {"ABC", "DEF"};
}

int main (int aArgc, char** aArgv)
{
   MyDataStruct s;
   s.str = "123";
   s.i32 = 123;
   s.vec_str = {"First", "2", "3"};

   std::cout << "Call Foo...\n";
   Foo(s);
   std::cout << "Return...\n"
       << s.str << "\n"
       << s.i32 << "\n"
       << s.vec_str.at(0) << "\n" << std::endl;

   return 0;
}
```

Will be printed

```text
Call Foo...
123
123
First

Return...
456
-456
ABC
```

### Using fibers

[Documentation](https://github.com/ITBear/GpCore2/Doc/Fibers.md)

```cpp
//Simple fiber task
class MyTask final: public GpTaskFiberBase
{
public:
   CLASS_DECLARE_DEFAULTS(MyTask)

public:
                         MyTask  (void) noexcept:GpTaskFiberBase("test task"_sv) {}
                         ~MyTask (void) noexcept override final {}

   virtual void          OnStart (void) override final {std::cout << "Start task..." << std::endl;}
   virtual GpTask::ResT  OnStep  (EventOptRefT /*aEvent*/) override final;
   virtual void          OnStop  (void) noexcept override final {std::cout << "Stop task..." << std::endl;}
};

GpTask::ResT    MyTask::OnStep (EventOptRefT /*aEvent*/)
{
   int k = 0;
   std::cout << "Task step " << k++ << std::endl;
   YELD_READY();
   std::cout << "Task step " << k++ << std::endl;
   YELD_READY();
   std::cout << "Task step " << k++ << std::endl;

   return GpTask::ResT::DONE;
}

int main (int aArgc, char** aArgv)
{
   //Initialize fiber manager (1024 fibers max, stack size 512kB per fiber)
   GpTaskFiberManager::S().Init(1024_cnt, 512_kB);

   //Initialize task scheduler
   GpTaskScheduler::SP taskScheduler = MakeSP<GpTaskScheduler>();
   taskScheduler->Start(taskScheduler, 8_cnt);

   //Create and add task to manager
   taskScheduler->AddTaskToReady(MakeSP<MyTask>());

   std::this_thread::sleep_for(std::chrono::seconds(5));

   //Stop task scheduler
   taskScheduler->RequestStop();
   taskScheduler->Join();

   return 0;
}
```

Will be printed

```text
Start task...
Task step 0
Task step 1
Task step 2
Stop task...
```

### Using safe-math

[Documentation](https://github.com/ITBear/GpCore2/Doc/NumericOps.md)

Base class for safe math operations is GpNumericOps. Provides functions for:
- SAdd - addition of two arithmetic values of the same type
- SSub - subtraction of two arithmetic values of the same type
- SMul - multiplication of two arithmetic values of the same type
- SDiv - division of two arithmetic values of the same type
- SConvertSafe - conversion between two arithmetic values of any types
```cpp
...
constexpr std::int8_t res = NumOps::SAdd(std::int8_t(100), std::int8_t(100));
...
```

Will cause an error at a compile time because of int8_t have range -128...127 and result of addition will be 200

```cpp
...
std::int8_t res = NumOps::SAdd(std::int8_t(100), std::int8_t(100));
...
```

Will cause an error at a runtime time because of int8_t have range -128...127 the result will be out of range   

# Build notes

 [QMake](https://doc.qt.io/qt-6/qmake-manual.html) is used as building tool. Supported compillers are g++ (v. 10+) and clang (v 11+)

### Dependencies
 - [boost context](https://www.boost.org/doc/libs/1_77_0/libs/context/doc/html/index.html) - for fibers context switching 
 (only if GP_USE_MULTITHREADING_FIBERS flag is set)
 - [gmp and gmpxx](https://github.com/alisw/GMP/) - for Base58 encoder (only if GP_USE_BASE58 flag is set)
 
 [Documentation](https://github.com/ITBear/GpCore2/Doc/Building.md)

# Libraries are built on top of GPCore

 - [GpJson](https://github.com/ITBear/GpJson) - 
 - [GpXml](https://github.com/ITBear/GpXml) - 
 - [GpNetwork](https://github.com/ITBear/GpNetwork) - 
 - [GpDbConnector](https://github.com/ITBear/GpDbConnector) - 
 - [GpLog](https://github.com/ITBear/GpLog) - 
 - [GpApi](https://github.com/ITBear/GpApi) - 
 
