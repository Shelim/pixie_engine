/***************************************
 *                                     *
 * REBUILDS MANIFEST APP FOR GIVEN APP *
 *                                     *
 ***************************************/

// File IO

function load_file(filename)
{
	var file = new java.io.File( filename );
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

function save_file(filename, content)
{
	var writer = new java.io.PrintWriter(filename);
	writer.print(content);
	writer.close();
}

// Property parser

function parse_xml(xml)
{
	var dbf = javax.xml.parsers.DocumentBuilderFactory.newInstance();
	var db = dbf.newDocumentBuilder();
	var is = new org.xml.sax.InputSource(new java.io.StringReader(xml));
	return db.parse(is);
}

function parse_xml_into_manifest(xml)
{
	var manifest = {};
	obj = parse_xml(xml);
	
	var root = obj.getElementsByTagName('manifest').item(0);
	for(var node = root.getFirstChild(); node != null; node = node.getNextSibling())
	{
		if (node.getNodeType() == org.w3c.dom.Node.ELEMENT_NODE)
			manifest[node.getNodeName()] = node.getTextContent();
	}
	
	return manifest;
}

function merge_manifests(obj1,obj2)
{
    var obj3 = {};
    for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
    for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
    return obj3;
}

// Search for projects

ds = project.createDataType("dirset");
dir = project.getBaseDir() + '/' + project.getProperty('build_app_path') + "/manifest";
ds.setDir( new java.io.File( dir ) );
ds.setIncludes( "*" );
ds.setExcludes( "_common" );

// iterate over files found.
projectsToGen = ds.getDirectoryScanner( project ).getIncludedDirectories( );

var manifestCommonBase = parse_xml_into_manifest(load_file(project.getBaseDir() + '/' + project.getProperty('build_app_path') + '/manifest/_common/common/manifest.xml'));
var manifestCommonBuild = parse_xml_into_manifest(load_file(project.getBaseDir() + '/' + project.getProperty('build_app_path') + '/manifest/_common/common/manifest_' + project.getProperty('build_configuration') + '.xml'));
	
var manifestCommonFinal = merge_manifests(manifestCommonBase, manifestCommonBuild);
var projectsManifests = {};	

for ( i = 0; i < projectsToGen.length; i++ )
{
	var projectToGen = projectsToGen[i];
	
	var manifestRoot = parse_xml_into_manifest(load_file(project.getBaseDir() + '/' + project.getProperty('build_app_path') + '/manifest/' + projectToGen + '/manifest.xml'));
	var manifestBase = parse_xml_into_manifest(load_file(project.getBaseDir() + '/' + project.getProperty('build_app_path') + '/manifest/' + projectToGen + '/common/manifest.xml'));
	var manifestBuild = parse_xml_into_manifest(load_file(project.getBaseDir() + '/'  + project.getProperty('build_app_path') + '/manifest/' + projectToGen + '/common/manifest_' + project.getProperty('build_configuration') + '.xml'));

	projectsManifests[projectToGen] = merge_manifests(merge_manifests(manifestBase, manifestBuild), manifestRoot);
	
}

var output_hpp = '';

for(var item in manifestCommonFinal)
{
	output_hpp += '\t\tstatic ustring_t get_manifest_' + item + '() { return "' + manifestCommonFinal[item] + '"_u; }\n';
}

output_hpp += '\n\n\t\tstatic ustring_t get_app_name(app_t app)\n\t\t{\n\t\t\tswitch(app)\n\t\t\t{';
for(var proj in projectsManifests)
{
	output_hpp += '\n\t\t\t\tcase app_t::' + proj + ': return "' + proj + '"_u;';
}
output_hpp += '\n\t\t\t}\n\t\t\treturn ""_u;\n\t\t}\n\n';

var possibleItems = [];

for ( i = 0; i < projectsToGen.length; i++ )
{
	var projectToGen = projectsToGen[i];
	for(var item in projectsManifests[projectToGen])
	{
		possibleItems.push(item);
		output_hpp += '\t\tstatic ustring_t get_app_' + projectToGen + '_manifest_' + item + '() { return "' + projectsManifests[projectToGen][item] + '"_u; }\n';
	}
}

var existingItems = [];

for(var i in possibleItems)
{
	var item = possibleItems[i];
	
	if(existingItems.indexOf(item) != -1)
		continue;
	
	existingItems.push(item);
	output_hpp += '\t\tstatic ustring_t get_common_manifest_' + item + '(app_t app) \n\t\t{';
	for ( i = 0; i < projectsToGen.length; i++ )
	{
		var projectToGen = projectsToGen[i];
		for(var item2 in projectsManifests[projectToGen])
		{
			if(item2 == item)
				output_hpp += '\n\t\t\tif(app == app_t::' + projectToGen + ') return "' + projectsManifests[projectToGen][item] + '"_u;';
		}
	}
	output_hpp += '\n\t\t\treturn ""_u;\n\t\t}\n';

}

var local_app = project.getProperty('output_unix_name');
for(var item in projectsManifests[local_app])
{
	output_hpp += '\t\tstatic ustring_t get_local_manifest_' + item + '() { return "' + projectsManifests[local_app][item] + '"_u; }\n';
}

output_hpp += '\n\t\tstatic app_t get_local_app() { return app_t::' + local_app + '; }\n';

output_hpp += '\n\n\t\tstatic ustring_t get_local_app_name() { return "' + local_app + '"_u; }';

var content = load_file(project.getBaseDir() + '/' + project.getProperty('autogen_manifest_app_hpp_path'), content);

content = content.replace(/\$MANIFEST_APP\$/g, output_hpp);

save_file(project.getBaseDir() + '/' + project.getProperty('autogen_manifest_app_hpp_path'), content);

