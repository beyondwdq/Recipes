TinyConfig
==========

Almost every program needs a configuration function to define the system
parameters and read them from file, sometimes also from command-line. During
my development of a simulator, I wanted a simply and easy-to-use configuration
parser without dependencies on non-standard C++ libraries. Hence I began to work 
on it and finally I got TinyConfig. 

The first version is written without separating the `ConfigReader` (which reads 
and parses file) from the `Config` (which defines the parameters). It is super 
simple, clean, and it just works. This version is tagged as `v0.1`.

Later on I wanted to try different config file format, espacially using a lua as 
the file parser. Hence the ConfigReader is separated from Config. With this 
separation, it is possible to support more format in the future by deriving new 
ConfigReaders. 

TinyConfig has the following functions:

1. It can read parameters from either a lua script (`LuaConfigReader`) or a plain 
file format (`PlainConfigReader`). In the plain file format, each line defines a 
parameter with the format of `key = value`, where "`key`" is the name of the 
parameter and `value` specifies the assigned value. No hierarchy is supported.

2. With every defined parameter, invoke `Config::add()` to register it to the 
parser. Then `ConfigReader` will automatically set the value if it is specified in 
the configuration file.

3. Three types of parameters are supported: `bool`, `int` and `double`.

In the current test defined in `test/config_test.cc`., a sub-class is derived from
class `Config` to define parameters. However, I really prefer to just copy the 
`Config` class, add parameters to it and invoke `Config::add()` in its constructor.

Although `LuaConfigReader` is more flexible by providing variables and expressions,
plain text is still considered as the best configuration file format. Here is a
good point quoted from a discussion on [Stackoverflow][best_format]:

>That is, plain old data has advantages over code. It's more human-readable. It's more machine-readable. You can look at it and see the actual values (rather than an algorithm for computing them). It encourages everyone to keep it simple. Loading an INI file can't put you in an infinite loop. It can't delete all your files. If loading an INI file works the first time, it'll work the next 3000 times too. (Scripts can work now and break later for any number of reasons.) Plain old data formats tend to have fewer versioning issues. And ironically, data is better for scripting than code. – Jason Orendorff

[best_format]: http://stackoverflow.com/questions/1925305/best-config-file-format
