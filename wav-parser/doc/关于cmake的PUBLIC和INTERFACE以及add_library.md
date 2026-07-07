# 1、project(my-own-...

**问题详情：**

project(my-own-sample)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE my-own-thread-pool)

target_link_libraries(${PROJECT_NAME} PRIVATE my-own-memory-pool)

target_link_libraries(${PROJECT_NAME} PRIVATE my-own-endian-test)

target_link_libraries(${PROJECT_NAME} PRIVATE my-own-wav-parser)

target_link_libraries(${PROJECT_NAME} PRIVATE Logger)

----------------------------------

project(my-own-wav-parser)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/3rd)

target_link_libraries(${PROJECT_NAME} INTERFACE Logger)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.hpp")

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.h")

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${srcs})

为什么sample中能引用到RtAudio下的头文件，而wav-parser中无法引用呢

---

关键点在于：**`sample` 能引用，不代表 `wav-parser` 自己能引用。**

你的 CMake 写法里面其实隐藏了几个问题，我们一个一个分析。

---

## 先看你的目录

```
wav-parser/
├── 3rd/
│   └── RtAudio/
│       └── RtAudio.h
├── include/
│   └── wav-parser/
│       └── wavparser.hpp
└── CMakeLists.txt
```

你的 CMake

```cmake
target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd
)
```

这意味着

凡是

```cmake
target_link_libraries(xxx PRIVATE my-own-wav-parser)
```

的目标都会得到

```
-I wav-parser/include
-I wav-parser/3rd
```

因此

sample 可以

```c++
#include <RtAudio/RtAudio.h>
```

因为

```
3rd/
    RtAudio/
        RtAudio.h
```

正好满足

```
-I wav-parser/3rd
```

以后

```
RtAudio/RtAudio.h
```

能够找到。

---

## 那为什么 wav-parser 自己不能？

因为你的库是

```cmake
add_library(${PROJECT_NAME} INTERFACE)
```

**INTERFACE Library 本身不会编译任何 cpp。**

它只有

```
INTERFACE_INCLUDE_DIRECTORIES
```

没有

```
PRIVATE_INCLUDE_DIRECTORIES
```

也就是说

```
wav-parser
```

自己实际上没有一个真正的编译过程。

如果你的

```
wavparser.cpp
```

或者

```
wavparser.hpp
```

里面

```c++
#include <RtAudio/RtAudio.h>
```

出现报错，通常不是 CMake 编译报错，而是 **IDE(IntelliSense)** 不知道 INTERFACE include。

---

## 更大的问题

你的下面这几句其实没有意义：

```cmake
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${srcs})
```

因为

```
INTERFACE Library
```

不能拥有

```
PRIVATE sources
```

正确应该是

```
target_sources(... INTERFACE ...)
```

或者根本不要写。

如果有 cpp，INTERFACE library 本来就是错误选择。

---

## 如果 wav-parser 有 cpp

例如

```
wav-parser
    wavparser.cpp
```

那么应该写

```cmake
add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/wavparser.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        3rd
)
```

如果 RtAudio 也需要暴露出去，再改成

```cmake
PUBLIC 3rd
```

---

## 如果 wav-parser 是纯 Header Only

那应该写

```cmake
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        3rd
)
```

然后不要

```cmake
target_sources(... PRIVATE ...)
```

---

## 还有一种最容易忽略的情况

你的 `wavparser.hpp`

如果写的是

```c++
#include <RtAudio.h>
```

那么是不行的。

因为你的 include path 是

```
-I wav-parser/3rd
```

实际文件是

```
3rd/
    RtAudio/
        RtAudio.h
```

应该写

```c++
#include <RtAudio/RtAudio.h>
```

除非你把 include 路径改成

```cmake
target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd/RtAudio
)
```

这样才能

```c++
#include <RtAudio.h>
```

---

### 我想确认一下真正的问题

你说的「**wav-parser 中无法引用**」具体是哪一种？

1. **VSCode/CLion 编辑器红线（IntelliSense 找不到）**
2. **CMake 编译时报找不到 `RtAudio.h`**
3. **`wavparser.hpp` 中找不到**
4. **`wavparser.cpp` 中找不到**

如果能把 **`wav-parser/CMakeLists.txt` 完整内容** 和 **报错信息**（完整错误）贴出来，我基本可以直接定位是哪一处配置导致的。

---

# 2、为什么这么说呢？

---

这是因为 **`INTERFACE Library` 在 CMake 的语义就是"没有自己的编译产物"**。

也就是说，它只是一个**使用要求（Usage Requirements）**的集合，包括：

- include directories
- compile definitions
- compile options
- link libraries

它**不会编译任何源文件**。

---

举个最简单的例子。

假设

```cmake
add_library(foo INTERFACE)

target_sources(foo PRIVATE a.cpp)
```

你觉得 `a.cpp` 会什么时候编译？

答案是：**永远不会。**

因为整个工程里根本不存在

```
编译 foo
```

这个动作。

真正发生的是

```
sample
 ├── 编译 sample.cpp
 └── 链接
```

而不是

```
foo
 ├── 编译 a.cpp
 └── 生成 foo.lib
```

由于 `foo` 是 INTERFACE，所以没有 `.obj`、没有 `.a`、没有 `.lib`。

既然没有自己的编译阶段，那么

```cmake
PRIVATE
```

给谁看？

没有人。

---

## PRIVATE 的意思是什么？

例如

```cmake
add_library(foo)

target_sources(foo PRIVATE
    a.cpp
)
```

这里的意思是

> **编译 foo 的时候，把 a.cpp 编译进去。**

所以这里必须存在

```
foo
 ├── a.cpp
 ├── b.cpp
 └── foo.lib
```

但是 INTERFACE 根本不存在这个步骤。

---

## CMake 官方也是这么规定的

`target_sources()` 有三种作用域：

```
PRIVATE
PUBLIC
INTERFACE
```

对于普通库：

```
PRIVATE
```

表示

> 给自己编译。

```
PUBLIC
```

表示

> 给自己编译，同时传播给别人。

```
INTERFACE
```

表示

> 自己不用，只告诉别人。

---

而对于

```cmake
add_library(foo INTERFACE)
```

官方明确规定：

> An INTERFACE library may only have INTERFACE sources.

也就是说

```cmake
target_sources(foo PRIVATE ...)
```

本身就是不符合 INTERFACE 的设计理念。

---

## 那为什么有时候还能配置成功？

这是很多人第一次接触 CMake 时容易疑惑的地方。

例如

```cmake
add_library(foo INTERFACE)

target_sources(foo PRIVATE a.cpp)
```

有些版本的 CMake：

- 不报错；
- 但是 **忽略** 这些 source。

你可以生成工程看看。

例如 VS 解决方案里面：

```
foo
```

下面根本没有

```
a.cpp
```

也没有编译动作。

换句话说，

```
target_sources(...)
```

没有真正发挥作用。

---

## Header-only 库应该怎么写？

真正的 Header-only 库通常就是

```cmake
add_library(foo INTERFACE)

target_include_directories(foo
    INTERFACE
        include
)
```

结束。

甚至连

```cmake
target_sources()
```

都不用。

如果只是为了让 IDE 显示头文件，可以这样：

```cmake
target_sources(foo
    INTERFACE
        include/foo/foo.hpp
        include/foo/bar.hpp
)
```

这里用的是 **INTERFACE**，作用是把这些头文件附加到目标上，方便 IDE 展示，但不会参与编译。

---

## 如果有 `.cpp` 呢？

一旦你的库里出现

```
foo.cpp
```

那它就已经不是 Header-only 了。

应该改成

```cmake
add_library(foo)

target_sources(foo PRIVATE
    foo.cpp
)

target_include_directories(foo
    PUBLIC
        include
)
```

这样 CMake 才会生成

```
foo.obj
↓
foo.lib
↓
sample.exe
```

这也是绝大多数 C++ 库（如 fmt、spdlog 的非 header-only 版本、OpenCV 等）的组织方式。

**一句话总结：**

- `INTERFACE` = **没有自己的编译过程**，只能传播"使用要求"，因此 `PRIVATE` 源文件没有实际意义。
- 有 `.cpp` 就应该使用普通库（`STATIC`、`SHARED` 或默认库），而不是 `INTERFACE`。