
# Webdriver++

A C++ client library for [Selenium Webdriver](http://www.seleniumhq.org/).
You can use this library in any C++ project.

COPYRIGHTS

 * 2014-2018 Sergey Kogan ([sekogan@gmail.com](mailto:sekogan@gmail.com))
 * 2017 Ilya Durdyev (durdyev)
 
SPECIAL THANKS

 * 2019 xingyun86 ([peipengshuai@hotmail.com](mailto:peipengshuai@hotmail.com))
 * 2020 xloem ([0xloem@gmail.com](mailto:0xloem@gmail.com))
 
## Downloading

```bash
git clone https://github.com/GermanAizek/webdriverxx.git
cd webdriverxx
```

## Installing Requirements

### Debian/Ubuntu

```bash
sudo apt-get install default-jre cmake g++ make curl
```

### Arch Linux

```bash
sudo pacman -Syu jre-openjdk cmake g++ make curl
```
### Gentoo

```bash
sudo emerge -av virtual/jre dev-util/cmake sys-devel/gcc sys-devel/make net-misc/curl
```

## Install on Linux

```bash
mkdir build && cd build && cmake ../src
sudo make && sudo make install
```

## Install on Windows

```bash
mkdir build
cd build
cmake ../src
# For example, open a solution in Visual Studio
```

## A quick example

##### Dependencies
You need to download and run selenium-server-standalone.

Complete list can be seen here: https://selenium-release.storage.googleapis.com/index.html

```bash
wget https://selenium-release.storage.googleapis.com/3.9/selenium-server-standalone-3.9.1.jar
java -jar seleniumjar.jar
```

##### Example codes

Build cmakelists in examples folder and run binary.

```bash
cd examples/example_start_browsers
mkdir build && cd build
cmake ..
make
./example_start_browsers
```

## Features

- Chainable commands.
- Value-like objects compatible with STL containers.
- Header-only.
- Lightweight dependencies:
    - [libcurl](http://curl.haxx.se/libcurl/),
    - [picojson](https://github.com/kazuho/picojson).
- Can be used with any testing framework.
- Linux, Mac and Windows.
- Tested on clang 10.0, GCC 9.3 and Visual Studio 2019.

## More examples

All examples are in the examples folder.

### Use proxy

```cpp
WebDriver ie = Start(InternetExplorer().SetProxy(
	SocksProxy("127.0.0.1:3128")
		.SetUsername("user")
		.SetPassword("12345")
		.SetNoProxyFor("custom.host")
	));
```

```cpp
WebDriver ff = Start(Firefox().SetProxy(DirectConnection()));
```

### Navigate browser

```cpp
driver
	.Navigate("http://facebook.com")
	.Navigate("http://twitter.com")
	.Back()
	.Forward()
	.Refresh();
```

### Find elements

```cpp
// Throws exception if no match is found in the document
Element menu = driver.FindElement(ById("menu"));

// Returns empty vector if no such elements
// The search is performed inside the menu element
std::vector<Element> items = menu.FindElements(ByClass("item"));
```

### Send keyboard input

```cpp
// Sends text input or a shortcut to the element
driver.FindElement(ByTag("input")).SendKeys("Hello, world!");

// Sends text input or a shortcut to the active window
driver.SendKeys(Shortcut() << keys::Control << "t");
```

### Execute Javascript

```cpp
// Simple script, no parameters
driver.Execute("console.log('Hi there!')");

// A script with one parameter
driver.Execute("document.title = arguments[0]", JsArgs() << "Cowabunga!");

// A script with more than one parameter
driver.Execute("document.title = arguments[0] + '-' + arguments[1]",
		JsArgs() << "Beep" << "beep");

// Arrays or containers can also be used as parameters
const char* ss[] = { "Yabba", "dabba", "doo" };
driver.Execute("document.title = arguments[0].join(', ')", JsArgs() << ss);

// Even an Element can be passed to a script
auto element = driver.FindElement(ByTag("input"));
driver.Execute("arguments[0].value = 'That was nuts!'", JsArgs() << element);
```

### Get something from Javascript

```cpp
// Scalar types
auto title = driver.Eval<std::string>("return document.title")
auto number = driver.Eval<int>("return 123");
auto another_number = driver.Eval<double>("return 123.5");
auto flag = driver.Eval<bool>("return true");

// Containers (all std::back_inserter compatible)
std::vector<std::string> v = driver.Eval<std::vector<std::string>>(
		"return [ 'abc', 'def' ]"
		);

// Elements!
Element document_element = driver.Eval<Element>("return document.documentElement");
```

### [Wait implicitly](http://selenium-python.readthedocs.org/en/latest/waits.html) for asynchronous operations

```cpp
driver.SetImplicitTimeoutMs(5000);

// Should poll the DOM for 5 seconds before throwing an exception.
auto element = driver.FindElement(ByName("async_element"));
```

### [Wait explicitly](http://selenium-python.readthedocs.org/en/latest/waits.html) for asynchronous operations

```cpp
#include <webdriverxx/wait.h>

auto find_element = [&]{ return driver.FindElement(ById("async_element")); };
Element element = WaitForValue(find_element);
```

```cpp
#include <webdriverxx/wait.h>

auto element_is_selected = [&]{
	return driver.FindElement(ById("asynchronously_loaded_element")).IsSelected();
	};
WaitUntil(element_is_selected);
```

### Use matchers from [Google Mock](https://code.google.com/p/googlemock/) for waiting

```cpp
#define WEBDRIVERXX_ENABLE_GMOCK_MATCHERS
#include <webdriverxx/wait_match.h>

driver.Navigate("http://initial_url.host.net");
auto url = [&]{ return driver.GetUrl(); };
using namespace ::testing;
auto final_url = WaitForMatch(url, HasSubstr("some_magic"));
```

### Testing with real browsers

Prerequisites:
- [Selenium Server](http://www.seleniumhq.org/download/)

```bash
selenium-server -p 4444 &
./webdriverxx --browser=<firefox|chrome|...>
```

## Advanced topics

### Unicode

The library is designed to be encoding-agnostic. It doesn't make
any assumptions about encodings. All strings are transferred
as is, without modifications.

The WebDriver protocol is based on UTF-8, so all strings passed
to the library/received from the library should be/are encoded
using UTF-8.

### Thread safety

- Webdriver++ objects are not thread safe. It is not safe to use
neither any single object nor different objects obtained from a single WebDriver
concurrently without synchronization. On the other side, Webdriver++ objects
don't use global variables so it is OK to use different instances of WebDriver
in different threads.

- The CURL library should be explicitly initialized if several WebDrivers are used from
multiple threads. Call `curl_global_init(CURL_GLOBAL_ALL);` from `<curl/curl.h>`
once per process before using this library.

### Use common capabilities for all browsers

```cpp
Capabilities common;
common.SetProxy(DirectConnection());
auto ff = Start(Firefox(common));
auto ie = Start(InternetExplorer(common));
auto gc = Start(Chrome(common));
```

### Use required capabilities

```cpp
Capabilities required = /* ... */;
auto ff = Start(Firefox(), required);
```

### Use custom URL for connecting to WebDriver

```cpp
const char* url = "http://localhost:4444/wd/hub/";

auto ff = Start(Firefox(), url);

// or
auto ff = Start(Firefox(), Capabilities() /* required */, url);
```

### Transfer objects between C++ and Javascript

```cpp
namespace custom {

struct Object {
	std::string string;
	int number;
};

// Conversion functions should be in the same namespace as the object
picojson::value CustomToJson(const Object& value) {
	return JsonObject()
		.Set("string", value.string)
		.Set("number", value.number);
}

void CustomFromJson(const picojson::value& value, Object& result) {
	assert(value.is<picojson::object>());
	result.string = FromJson<std::string>(value.get("string"));
	result.number = FromJson<int>(value.get("number"));
}

} // namespace custom

custom::Object o1 = { "abc", 123 };
driver.Execute("var o1 = arguments[0];", JsArgs() << o1);
custom::Object o1_copy = driver.Eval<custom::Object>("return o1");
custom::Object o2 = driver.Eval<custom::Object>("return { string: 'abc', number: 123 }");
```

--------------------

