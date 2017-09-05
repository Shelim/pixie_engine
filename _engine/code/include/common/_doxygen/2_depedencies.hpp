/**
* @page depedencies Depedencies
* @tableofcontents
* @section depedency_dashboard (All depedencies)
* <center><table>
* <caption id="depedency_angelcode_dashboard_table">List of all depedencies</caption>
* <tr><th>Name<th>Role<th>License<th>Author
* <tr><td>@ref depedency_angelcode "AngelCode Scripting Library"<td>Scripting language provider<td>zlib<td>Andreas J&ouml;nsson
* <tr><td>@ref depedency_anttweakbar "AntTweakBar"<td>Tweaker UI provider<td>zlib/libpng<td>Philippe Decaudin
* <tr><td>@ref depedency_apacheant "Apache Ant"<td>Crossplatform build system<td>Apache License, Version 2.0<td>The Apache Software Foundation
* <tr><td>@ref depedency_blowfish "Blowfish"<td>Fast encryption module<td>The Microsoft Public License (Ms-PL)<td>George Anescu
* <tr><td>@ref depedency_di "Boost Dependency Injection"<td>Depedency Injection framework<td>Boost Software License, Version 1.0<td>Kris Jusiak
* <tr><td>@ref depedency_cereal "cereal"<td>Serialization framework<td>BSD license<td>Randolph Voorhies, Shane Grant
* <tr><td>@ref depedency_cg "cg"<td>Shader language<td>License For Customer Use of NVIDIA Software<td>NVIDIA
* <tr><td>@ref depedency_debugbreak "debugbreak"<td>Crossplatform software breakpoints<td>MIT/X11<td>Scott Tsai
* <tr><td>@ref depedency_directx "directx"<td>Renderer backend<td>Propertiary<td>Microsoft
* <tr><td>@ref depedency_doxygen "doxygen"<td>Docs generator<td>GNU GPL (No source code included)<td>Dimitri van Heesch
* <tr><td>@ref depedency_git "git"<td>Source control system and changelog support<td>GNU LGPL v2.1<td>Linus Torvalds, Junio Hamano and others
* <tr><td>@ref depedency_glew "glew"<td>Extensions for OpenGL<td>Khronos License (MIT)<td>Lev Povalahev and others
* <tr><td>@ref depedency_googletest "Google Test"<td>Autmated tests for engine<td>BSD 3-clauses<td>Google Inc.
* <tr><td>@ref depedency_html_help_workshop "HTML Help Workshop"<td>Tool for generating chm documentation for @ref depedency_doxygen "doxygen"<td>HTML Help End-User License Agreement<td>Microsoft
* <tr><td>@ref depedency_jdk "JDK 1.8"<td>Required by @ref depedency_apacheant "Apache Ant"<td>Sun License<td>Oracle Corporation
* <tr><td>@ref depedency_librocket "libRocket"<td>HTML/CSS GUI-in-renderer<td>MIT/X11<td>CodePoint Ltd, Shift Technology Ltd, and contributors
* <tr><td>@ref depedency_vlc "libVLC"<td>Movie playback<td>GNU LGPL 2<td>VideoLAN
* <tr><td>@ref depedency_minizip "miniZip"<td>Archive compression/deompression<td>BSD Variant<td>Nathan Moinvaziri
* <tr><td>@ref depedency_nsis "NSIS"<td>Setup on Windows platform<td>zlib/libpng<td>Open Source Contributors
* <tr><td>@ref depedency_openal_soft "OpenAL (Software)"<td>Sound support module<td>GNU LPGL<td>kcat
* <tr><td>@ref depedency_pugixml "pugixml"<td>Fast XML loader and parser<td>MIT/X11<td>Arseny Kapoulkine
* <tr><td>@ref depedency_sdl "SDL 2.0"<td>Cross-platform application base<td>zlib<td>Open Source Contributors
* <tr><td>@ref depedency_sdl_net "SDL NET 2.0"<td>Cross-platform network support<td>GNU LGPL<td>Open Source Contributors
* <tr><td>@ref depedency_stackwalker "Stackwalker"<td>Cross-platform callstack dumper<td>New BSD License<td>Jochen Kalmbach
* <tr><td>@ref depedency_stb "stb"<td>Simple single-header only tools<td>Public Domain<td>Sean Barrett
* <tr><td>@ref depedency_thttpd "thttpd"<td>Simple HTTP server for update testing<td>BSD licenses variant<td>Jef Poskanzer
* <tr><td>@ref depedency_upx "UPX"<td>Universal Executable Packer, reducing size of binary files<td>BSD licenses variant<td>Markus F.X.J. Oberhumer, L&aacute;szl&oacute; Moln&aacute;r & John F. Reiser.
* <tr><td>@ref depedency_wxwidgets "wxWidgets"<td>GUI for editor/lanucher/etc.<td>wxWindows Licence<td>Open Source Contributors
* </table></center>
*
* @section depedency_angelcode AngelCode Scripting Library
* The AngelCode Scripting Library, or AngelScript as it is also known, is an extremely flexible cross-platform scripting library designed to allow applications to extend their functionality through external scripts. It has been designed from the beginning to be an easy to use component, both for the application programmer and the script writer.
*
* Efforts have been made to let it call standard C functions and C++ methods with little to no need for proxy functions. The application simply registers the functions, objects, and methods that the scripts should be able to work with and nothing more has to be done with your code. The same functions used by the application internally can also be used by the scripting engine, which eliminates the need to duplicate functionality.
*
* For the script writer the scripting language follows the widely known syntax of C/C++, but without the need to worry about pointers and memory leaks. Contrary to most scripting languages, AngelScript uses the common C/C++ datatypes for more efficient communication with the host application.
*
* AngelScript is completely free, released under the zlib license. I only ask for your recognition, nothing else. However, if you would like to make a donation I would be very grateful. Your donation gives me more inspiration and will allow me to spend more time working with AngelScript.
*
* Give the library a try, and let me know how you like it.
*
* Regards,
* Andreas J&ouml;nsson
*
* @section depedency_anttweakbar AntTweakBar
* AntTweakBar is a small and easy-to-use C/C++ library that allows programmers to quickly add a light and intuitive graphical user interface into graphic applications based on OpenGL (compatibility and core profiles), DirectX 9, DirectX 10 or DirectX 11 to interactively tweak parameters on-screen. 
*
* C/C++ variables can be bound to graphical controls that allow users to modify them. Thus, variables exposed by programmers can be easily modified. They are displayed into the graphical application through one or more embedded windows called tweak bars.
*
* The AntTweakBar library mainly targets graphical applications that need a quick way to tune parameters (even in fullscreen mode) and see the result in real-time like 3D demos, games, prototypes, inline editors, debug facilities of weightier graphical applications, etc.
*
* @section depedency_apacheant Apache Ant
* Apache Ant is a Java library and command-line tool whose mission is to drive processes described in build files as targets and extension points dependent upon each other. The main known usage of Ant is the build of Java applications. Ant supplies a number of built-in tasks allowing to compile, assemble, test and run Java applications. Ant can also be used effectively to build non Java applications, for instance C or C++ applications. More generally, Ant can be used to pilot any type of process which can be described in terms of targets and tasks.
*
* Ant is written in Java. Users of Ant can develop their own "antlibs" containing Ant tasks and types, and are offered a large number of ready-made commercial or open-source "antlibs".
*
* Ant is extremely flexible and does not impose coding conventions or directory layouts to the Java projects which adopt it as a build tool.
*
* @section depedency_blowfish Blowfish
* Encryption and Decryption of Byte Strings using the Blowfish Encryption Algorithm. Blowfish is a block cipher that encrypts data in 8-byte blocks. The algorithm consists of two parts: a key-expansion part and a data-ancryption part. Key expansion converts a variable key of at least 1 and at most 56 bytes into several subkey arrays totaling 4168 bytes. Blowfish has 16 rounds. Each round consists of a key-dependent permutation, and a key and data-dependent substitution. All operations are XORs and additions on 32-bit words. The only additional operations are four indexed array data lookups per round. Blowfish uses a large number of subkeys. These keys must be precomputed before any data encryption or decryption. The P-array consists of 18 32-bit subkeys: P0, P1,...,P17. There are also four 32-bit S-boxes with 256 entries each: S0,0, S0,1,...,S0,255; S1,0, S1,1,...,S1,255; S2,0, S2,1,...,S2,255; S3,0, S3,1,...,S3,255;
*
* @section depedency_di Boost Dependency Injection
* Dependency Injection (DI) involves passing (injecting) one or more dependencies (or services) to a dependent object (or client) which become part of the client’s state. It is like the Strategy Pattern, except the strategy is set once, at construction. DI enables loosely coupled designs, which are easier to maintain and test.
*
* @section depedency_cereal cereal
* cereal is a header-only C++11 serialization library. cereal takes arbitrary data types and reversibly turns them into different representations, such as compact binary encodings, XML, or JSON. cereal was designed to be fast, light-weight, and easy to extend - it has no external dependencies and can be easily bundled with other code or used standalone.
*
* Serialization support for pretty much every type in the standard library comes out of the box with cereal. cereal also fully supports inheritance and polymorphism. Since cereal was written to be a minimal, fast library, it does not perform the same level of object tracking as other serialization libraries such as Boost. As a consequence of this raw pointers and references are not supported, however smart pointers (things like std::shared_ptr and std::unique_ptr) are no problem.
*
* cereal uses features new to C++11 and requires a fairly compliant C++ compiler to work properly. cereal officially supports g++ 4.7.3, clang++ 3.3, and MSVC 2013 (or newer). It may work on older versions or other compilers such as ICC, but there is no emphasis on supporting them. cereal works under both libstdc++ and libc++ when compiling with g++ or clang++.
*
* @section depedency_cg cg
* Cg (short for C for Graphics[1]) is a high-level shading language developed by Nvidia in close collaboration with Microsoft for programming vertex and pixel shaders. Cg is based on the C programming language and although they share the same syntax, some features of C were modified and new data types were added to make Cg more suitable for programming graphics processing units. This language is only suitable for GPU programming and is not a general programming language. The Cg compiler outputs DirectX or OpenGL shader programs.
*
* @section depedency_debugbreak debugbreak
* Simple header-only for crossplatform breakpoint trapping
*
* @section depedency_directx DirectX
* Microsoft DirectX is a collection of application programming interfaces (APIs) for handling tasks related to multimedia, especially game programming and video, on Microsoft platforms. Originally, the names of these APIs all began with Direct, such as Direct3D, DirectDraw, DirectMusic, DirectPlay, DirectSound, and so forth. The name DirectX was coined as a shorthand term for all of these APIs (the X standing in for the particular API names) and soon became the name of the collection. When Microsoft later set out to develop a gaming console, the X was used as the basis of the name Xbox to indicate that the console was based on DirectX technology. The X initial has been carried forward in the naming of APIs designed for the Xbox such as XInput and the Cross-platform Audio Creation Tool (XACT), while the DirectX pattern has been continued for Windows APIs such as Direct2D and DirectWrite.
*
* Direct3D (the 3D graphics API within DirectX) is widely used in the development of video games for Microsoft Windows and the Xbox line of consoles. Direct3D is also used by other software applications for visualization and graphics tasks such as CAD/CAM engineering. As Direct3D is the most widely publicized component of DirectX, it is common to see the names "DirectX" and "Direct3D" used interchangeably.
*
* The DirectX software development kit (SDK) consists of runtime libraries in redistributable binary form, along with accompanying documentation and headers for use in coding. Originally, the runtimes were only installed by games or explicitly by the user. Windows 95 did not launch with DirectX, but DirectX was included with Windows 95 OEM Service Release 2. Windows 98 and Windows NT 4.0 both shipped with DirectX, as has every version of Windows released since. The SDK is available as a free download. While the runtimes are proprietary, closed-source software, source code is provided for most of the SDK samples. Starting with the release of Windows 8 Developer Preview, DirectX SDK has been integrated into Windows SDK.
*
* @section depedency_doxygen doxygen
* Doxygen is the de facto standard tool for generating documentation from annotated C++ sources, but it also supports other popular programming languages such as C, Objective-C, C#, PHP, Java, Python, IDL (Corba, Microsoft, and UNO/OpenOffice flavors), Fortran, VHDL, Tcl, and to some extent D.
*
* @section depedency_git git
* Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency.
*
* Git is easy to learn and has a tiny footprint with lightning fast performance. It outclasses SCM tools like Subversion, CVS, Perforce, and ClearCase with features like cheap local branching, convenient staging areas, and multiple workflows.
*
* @section depedency_glew glew
* The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.
*
* @section depedency_googletest Google Test
* Google Test is a unit testing library for the C++ programming language, based on the xUnit architecture. The library is released under the BSD 3-clause license. It can be compiled for a variety of POSIX and Windows platforms, allowing unit-testing of C sources as well as C++ with minimal source modification. The tests themselves could be run one at a time, or even be called to run all at once. This makes the debugging process very specific and caters to the need of many programmers and coders alike.
*
* @section depedency_html_help_workshop HTML Help Workshop
* Microsoft Compiled HTML Help is a Microsoft proprietary online help format, consisting of a collection of HTML pages, an index and other navigation tools. The files are compressed and deployed in a binary format with the extension .CHM, for Compiled HTML. The format is often used for software documentation.
*
* It was introduced as the successor to Microsoft WinHelp with the release of Windows 98 and is still supported in Windows 7. Although the format was designed by Microsoft, it has been successfully reverse-engineered and is now supported in many document viewer applications.
*
* @section depedency_jdk JDK 1.8
* Thank you for downloading this release of the Java Platform, Standard Edition Development Kit (JDK). The JDK is a development environment for building applications, applets, and components using the Java programming language.
*
* The JDK includes tools useful for developing and testing programs written in the Java programming language and running on the Java platform.
*
* @section depedency_librocket libRocket
* libRocket is the C++ user interface package based on the HTML and CSS standards. It is designed as a complete solution for any project's interface needs.
*
* libRocket uses the time-tested open standards XHTML1.0 and CSS2.0 (while borrowing features from HTML5 and CSS3), and extends them with features suited towards real-time applications. Because of this, you don't have to learn a whole new proprietary technology like other libraries in this space.
*
* @section depedency_vlc libVLC
* The libVLC (VLC SDK) media framework can be embedded into an application to get multimedia capabilities.
*
* Since VLC is based on libVLC, one should be able to have the same features that VLC media player has.
*
* The libVLC media framework is already used by several applications; see who uses libVLC?
*
* @section depedency_minizip miniZip
* Contains the latest bug fixes that having been found all over the internet including the old minizip forum and zlib developer's mailing list along with some additional features. Based on the original work of Gilles Vollant and contributed to by many people over the years.
*
* @section depedency_nsis NSIS
* NSIS (Nullsoft Scriptable Install System) is a professional open source system to create Windows installers. It is designed to be as small and flexible as possible and is therefore very suitable for internet distribution.
* 
* Being a user's first experience with your product, a stable and reliable installer is an important component of successful software. With NSIS you can create such installers that are capable of doing everything that is needed to setup your software.
* 
* NSIS is script-based and allows you to create the logic to handle even the most complex installation tasks. Many plug-ins and scripts are already available: you can create web installers, communicate with Windows and other software components, install or update shared components and more.
*
* @section depedency_openal_soft OpenAL (Software)
* OpenAL Soft is an LGPL-licensed, cross-platform, software implementation of the OpenAL 3D audio API. It's forked from the open-sourced Windows version available originally from openal.org's SVN repository (now defunct).
* 
* OpenAL provides capabilities for playing audio in a virtual 3D environment. Distance attenuation, doppler shift, and directional sound emitters are among the features handled by the API. More advanced effects, including air absorption, occlusion, and environmental reverb, are available through the EFX extension. It also facilitates streaming audio, multi-channel buffers, and audio capture.
*
* @section depedency_pugixml pugixml
* pugixml is a light-weight C++ XML processing library. It features:
* - DOM-like interface with rich traversal/modification capabilities
* - Extremely fast non-validating XML parser which constructs the DOM tree from an XML file/buffer
* - XPath 1.0 implementation for complex data-driven tree queries
* 0 Full Unicode support with Unicode interface variants and automatic encoding conversions
* 
* The library is extremely portable and easy to integrate and use. You can download it in an archive (Windows/Unix line endings), get it from Git/Subversion repository, install it as a package in one of the major Linux/BSD distributions (Ubuntu, Debian, Fedora, Gentoo, Arch Linux, FreeBSD and more), install it as a package in one of the OSX package managers (Homebrew, MacPorts), install a NuGet package or use one of the alternative package managers (Conda).
*
* pugixml is developed and maintained since 2006 and has many users. All code is distributed under the MIT license, making it completely free to use in both open-source and proprietary applications.
*
* @section depedency_sdl SDL 2.0
* Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.
* 
* SDL officially supports Windows, Mac OS X, Linux, iOS, and Android. Support for other platforms may be found in the source code.
*
* SDL is written in C, works natively with C++, and there are bindings available for several other languages, including C# and Python.
*
* SDL 2.0 is distributed under the zlib license. This license allows you to use SDL freely in any software.
*
* @section depedency_sdl_net SDL NET 2.0
* SDL_net is a network library that is used with the SDL library, and almost as portable. It allows a programmer to use network functionality without having to code different things for different platforms. It also simplyfies the handling of network connections and data transfer.
*
* @section depedency_stackwalker Stackwalker
* This article describes the (documented) way to walk a callstack for any thread (own, other and remote). It has an abstraction layer, so the calling app does not need to know the internals.
*
* This project was initially published on Codeproject (http://www.codeproject.com/KB/threads/StackWalker.aspx). But it is hard to maintain the article and the source on codeproject, so I was pushed to publish the source code on an "easier to modify" platform. Therefor I have choosen "codeplex" ;(
*
* But time goes by, and codeplex went away ;)
*
* @section depedency_stb stb
* Single-file public domain (or MIT licensed) libraries for C/C++
*
* Most libraries by stb, except: stb_dxt by Fabian "ryg" Giesen, stb_image_resize by Jorge L. "VinoBS" Rodriguez, and stb_sprintf by Jeff Roberts.
*
* @section depedency_thttpd thttpd
* thttpd is a simple, small, portable, fast, and secure HTTP server.
*
* - Simple: It handles only the minimum necessary to implement HTTP/1.1. Well, maybe a little more than the minimum.
* - Small: See the comparison chart. It also has a very small run-time size, since it does not fork and is very careful about memory allocation.
* - Portable: It compiles cleanly on most any Unix-like OS, specifically including FreeBSD, SunOS 4, Solaris 2, BSD/OS, Linux, OSF.
* - Fast: In typical use it's about as fast as the best full-featured servers (Apache, NCSA, Netscape). Under extreme load it's much faster.
* - Secure: It goes to great lengths to protect the web server machine against attacks and breakins from other sites.
*
* @section depedency_upx UPX
* UPX is a free, portable, extendable, high-performance executable packer for several executable formats.
*
* @section depedency_wxwidgets wxWidgets
* wxWidgets is a C++ library that lets developers create applications for Windows, Mac OS X, Linux and other platforms with a single code base. It has popular language bindings for Python, Perl, Ruby and many other languages, and unlike other cross-platform toolkits, wxWidgets gives applications a truly native look and feel because it uses the platform's native API rather than emulating the GUI. It's also extensive, free, open-source and mature.
**/