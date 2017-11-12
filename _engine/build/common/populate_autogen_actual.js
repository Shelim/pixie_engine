/*******************************************************************
 * This script populates values to autogen manifest. It can handle
 * arbitrary amount of manifest variables. If you add a new manifest
 * variable, be sure to update std/manifest_std.hpp as well!
 *
 * Manifests for all platforms will be processed by this script
 * and embeded into application.
 * 
 * REQUIRES:
 *     - Javascript engine (yeah, it's Javascript file...)
 *     - Base path '\_engine'
 *     - Manifests source inside '\${project_to_build}\manifest\${platform}\manifest.xml'
 * INPUT:
 *     (None)
 * AUTHOR:
 *     Piotr Kosek <piotr@kosek.com>
 * LICENSE:
 *     MIT/X11 License
 * CHANGELOG:
 *     0.1 [October, 18th 2017]: 
 *         - Initial version
 *******************************************************************
 * CONSTRAINS:
 *     - Configurable at the top of file
 *     - All config variables tamperable
 *     - No redudancy
 *******************************************************************
 * Copyright (c) 2017 Piotr Kosek
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 * A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *******************************************************************/
  
// START OF CONFIGURATION SECTION

// Setup manifest file information
var manifest_base_path = "${project_to_build}/manifest";

var manifest_file = "manifest.xml";	

// Setup autogen file information
var autogen_path = "${build_path_autogen_file}";

// Setup source code for manifest generation
var manifest_pattern = "engine::ustring_t engine::manifest_$TYPE$_t::get_$NAME$() const { return \"$VALUE$\"_u; } \n";
							
// END OF CONFIGURATION SECTION
// START OF HELPER METHOD SECTION

// Log something
function log(str)
{
	var echo = project.createTask("echo");
    echo.setMessage(str);
    echo.perform();
}

// Load file into string and return it
function load_file(path)
{
	var file = new java.io.File( project.replaceProperties(path) );
	var reader = new java.io.FileReader(file);
	var buffered_reader = new java.io.BufferedReader(reader);
	
	var line = buffered_reader.readLine();
	var sb = new java.lang.StringBuilder();
	
	while(line != null)
	{
	  sb.append(line).append("\n");
	  line = buffered_reader.readLine();
	}
	
	return sb.toString();
}

// Save content to file
function save_file(path, content)
{
	var writer = new java.io.PrintWriter(project.replaceProperties(path));
	writer.print(content);
	writer.close();
}

// Returns array
function search_all_directories_at_given_path(path)
{	
	var ret = [];
	
	dir = project.getBaseDir() + "/../"; // Because we start at '\_engine'
	
	ds = project.createDataType("dirset");
	ds.setDir(new java.io.File(dir + project.replaceProperties(path)));

	ds.setIncludes("*");
	ds.setErrorOnMissingDir(true);
	
	directories = ds.getDirectoryScanner(project).getIncludedDirectories();
	
	for(var d = 0; d < directories.length; d++)
	{
		ret.push(directories[d]);
	}
		
	return ret;
}

// Ultra simple XML parser (in single line!)
var fromXML;!function(r){function t(r,t){return a(n(r),t)}function n(r){function t(r){var t=r.length,f=r[0];if("/"===f)for(var u=r.replace(/^\/|[\s\/].*$/g,"").toLowerCase();c.length;){var a=l.n&&l.n.toLowerCase();if(l=c.pop(),a===u)break}else if("?"===f)n({n:"?",r:r.substr(1,t-2)});else if("!"===f)"[CDATA["===r.substr(1,7)&&"]]"===r.substr(-2)?e(r.substr(8,t-10)):n({n:"!",r:r.substr(1)});else{var o=s(r);n(o),"/"===r[t-1]?o.c=1:(c.push(l),l=o)}}function n(r){l.f.push(r)}function e(r){r=f(r),r&&n(u(r))}for(var a=String.prototype.split.call(r,/<([^!<>?](?:'[\S\s]*?'|"[\S\s]*?"|[^'"<>])*|!(?:--[\S\s]*?--|\[[^\[\]'"<>]+\[[\S\s]*?]]|DOCTYPE[^\[<>]*?\[[\S\s]*?]|(?:ENTITY[^"<>]*?"[\S\s]*?")?[\S\s]*?)|\?[\S\s]*?\?)>/),o=a.length,i={f:[]},l=i,c=[],p=0;p<o;){var v=a[p++];v&&e(v);var g=a[p++];g&&t(g)}return i}function s(r){var t={f:[]};r=r.replace(/\s*\/?$/,"");var n=r.search(/[\s='"\/]/);return n<0?t.n=r:(t.n=r.substr(0,n),t.t=r.substr(n)),t}function e(r,t){if(r.t){for(var n,s,e=r.t.split(/([^\s='"]+(?:\s*=\s*(?:'[\S\s]*?'|"[\S\s]*?"|[^\s'"]*))?)/),a=e.length,i=0;i<a;i++){var c=f(e[i]);if(c){n||(n={});var p=c.indexOf("=");if(p<0)c=l+c,s=null;else{s=c.substr(p+1).replace(/^\s+/,""),c=l+c.substr(0,p).replace(/\s+$/,"");var v=s[0],g=s[s.length-1];v!==g||"'"!==v&&'"'!==v||(s=s.substr(1,s.length-2)),s=u(s)}t&&(s=t(c,s)),o(n,c,s)}}return n}}function f(r){return r&&r.replace(/^\s+|\s+$/g,"")}function u(r){return r.replace(/(&(?:lt|gt|amp|apos|quot|#(?:\d{1,6}|x[0-9a-fA-F]{1,5}));)/g,function(r){if("#"===r[1]){var t="x"===r[2]?parseInt(r.substr(3),16):parseInt(r.substr(2),10);if(t>-1)return String.fromCharCode(t)}return i[r]||r})}function a(r,t){if("string"==typeof r)return r;var n=r.r;if(n)return n;var s,f=e(r,t),u=r.f,i=u.length;if(f||i>1)s=f||{},u.forEach(function(r){"string"==typeof r?o(s,c,r):o(s,r.n,a(r,t))});else if(i){var l=u[0];if(s=a(l,t),l.n){var p={};p[l.n]=s,s=p}}else s=r.c?null:"";return t&&(s=t(r.n||"",s)),s}function o(r,t,n){if("undefined"!=typeof n){var s=r[t];s instanceof Array?s.push(n):t in r?r[t]=[s,n]:r[t]=n}}var i={"&amp;":"&","&lt;":"<","&gt;":">","&apos;":"'","&quot;":'"'},l="@",c="#";r.fromXML=fromXML=t}("object"==typeof exports&&exports||{});

// Returns object
function parse_manifest(manifest)
{
	return fromXML(manifest);
};

// END OF HELPER METHOD SECTION

var platforms = search_all_directories_at_given_path(manifest_base_path)

var fields_content = '';

for(var p = 0; p < platforms.length; p++)
{
	var platform = platforms[p];
	var manifest_path = manifest_base_path + "/" + platform + "/" + manifest_file;
	
	var manifest_content = parse_manifest(load_file(project.replaceProperties(manifest_path)));
	
	for(var prop in manifest_content)
	{
		if(prop.startsWith("manifest_"))
		{
			var type = prop.substring("manifest_".length());
			for(var prop2 in manifest_content[prop])
			{
				if(!prop2.startsWith("!"))
				{
					var name = prop2;
					var value = manifest_content[prop][prop2];
	
					fields_content += manifest_pattern.replace("$TYPE$", type).replace("$NAME$", name).replace("$VALUE$", value);
				}
			}
		}
	}
}

autogen_path = project.getBaseDir() + "\\" + project.replaceProperties(autogen_path);

var autogen_content = load_file(autogen_path);
autogen_content = autogen_content.replace('$MANIFEST_FIELDS$', fields_content); // ToDo!
save_file(autogen_path, autogen_content);