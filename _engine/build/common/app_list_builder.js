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


for(var proj in projectsManifests)
{
	output_hpp += '\nAPP_STD(' + proj + ')';
}

var content = load_file(project.getBaseDir() + '/' + project.getProperty('autogen_app_list_path'), content);

content = content.replace(/\$APP_LIST\$/g, output_hpp);

save_file(project.getBaseDir() + '/' + project.getProperty('autogen_app_list_path'), content);

