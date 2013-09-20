libconfig
=============

A library to read configuration files.

Dependecies
=============
* libcollections


Format
=============
Configuration files use curly braces to specify hierarchical grouping and relationships. 
A configuration statement is a key-value pair separated with a equal sign character.

Here is an example:
`
/*
 *  Application Settings
 *
 *  All of the application settings go in here. 
 */
settings {
	locale = "en_US" // the language and locale 
	default_table_size = 3019 /* default */
	alpha = 1.2
	beta  = 0.00121312

	network {
		host = "127.0.0.1"
		port = 65436
	}
}

debug {
	enable = true
}

message {
	english = "Hello!"
	spanish = "¡Hola"
	german  = "Hallo!"
	chinese = "你好"
	french  = "Bonjour!"
	russian = "привет"
}

novelties {
	text = "Blah Blah Blah"
}
`


License
=============
> Copyright (C) 2012 Joseph A. Marrero.  http://www.manvscode.com/
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in
> all copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
> THE SOFTWARE.

