Why I wrote TinyConfig
======================

Almost every program needs a configuration function to define the system
parameters and read them from file, sometimes also from command-line. During
my development of a simulator, I wanted a simply and easy-to-use configuration
parser without dependencies on non-standard C++ libraries. Hence I began to work 
on it and finally I got TinyConfig. 

The first version is written without separating the ConfigReader (which reads 
and parses file) from the Config (which defines the parameters). It is super 
simple, clean, and it just works. This version is tagged as `v0.1`.

Later on I wanted to try different config file format, espacially using a lua as 
the file parser. Hence the ConfigReader is separated from Config. With this 
separation, it is possible to support more format in the future by deriving new 
ConfigReaders. 

TinyConfig has the following functions:

1. It reads parameters from a simple file format. Each line defines a parameter 
with the format of "key = value", where "key" is the name of the parameter 
and "value" specifies the assigned value. No hierarchy is supported.

2. With every defined parameter, invoke Config::add() to register it to the 
parser. Then the Config will automatically set the value if it is specified in 
the configuration file.

3. Three types of parameters are supported: bool, int and double.

In the current test defined in test/config_test.cc, a sub-class is derived from
class Config to define parameters. However, I really prefer to just copy the 
Config class, add parameters to it and invoke Config::add() in its constructor.


