function b(a){return a?(a^Math.random()*16>>a/4).toString(16):([1e7]+-1e3+-4e3+-8e3+-1e11).replace(/[018]/g,b)}
		
function gen_guid()
{
	return b();
}

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

function get_output_type(xml)
{
	var regex = /<!--.*?-->/;
	
	var regex2 = /.*<output_type>(.*)<\/output_type>.*/;
	
	var ret = xml.replace(/\n/g, "").replace(regex, '')
	
	if(ret.indexOf('<output_type>') == -1)
		return '';
	
	return ret.replace(regex2, '$1');
}

function get_binary_filename(xml)
{
	var regex = /<!--.*?-->/;
	
	var regex2 = /.*<binary_filename>(.*)<\/binary_filename>.*/;
	
	var ret = xml.replace(/\n/g, "").replace(regex, '')
	
	if(ret.indexOf('<binary_filename>') == -1)
		return '';
	
	return ret.replace(regex2, '$1');
}

function list_paths(project_id, project_type)
{
	var ret = [
	"_engine/code/include/common",
	"_engine/code/src/common",
	"_engine/code/include/" + project_type,
	"_engine/code/src/" + project_type,
	"_engine/depedency/*/include",
	"_engine/depedency/*/src",
	"_engine/depedency/googletest",
	project.getProperty('app_unix_name') + "/code/include/common",
	project.getProperty('app_unix_name') + "/code/src/common",
	project.getProperty('app_unix_name') + "/code/include/" + project_id,
	project.getProperty('app_unix_name') + "/code/src/" + project_id,
	];				
	
	return ret;
}

function list_libraries(project_id, project_type)
{
	var ret = [
	"_engine/depedency/*/lib/windows_x86",
	];				
	
	return ret;
}

function list_ignored_depedencies(project_type)
{
	var ret = [
	'apache_ant',
	'nsis',
	'wxwidgets\\include',
	'jdk1.8.0_131',
	'googletest\\src\\gtest.cc',
	'googletest\\src\\gtest-death-test.cc',
	'googletest\\src\\gtest-filepath.cc',
	'googletest\\src\\gtest-port.cc',
	'googletest\\src\\gtest-printers.cc',
	'googletest\\src\\gtest-test-part.cc',
	'googletest\\src\\gtest-typed-test.cc',
	];
	
	if(project_type != 'test')
		ret.push('googletest');
	
	return ret;
}

function list_additional_includes(project_id, project_type)
{
	var src_path = list_paths(project_id, project_type);
	
	var ret = [];
	
	dir = project.getBaseDir() + "/../" + project.getProperty("app_unix_name") + "/project/visual_studio_2015/";
	
	for (var i=0; i<src_path.length; i++)
	{
		ds = project.createDataType("dirset");
		ds.setDir( new java.io.File( dir + "../../../" ) );
		ds.setIncludes( src_path[i] );
		ds.setErrorOnMissingDir(false);
		
		files = ds.getDirectoryScanner( project ).getIncludedDirectories( );
		for ( f = 0; f < files.length; f++ )
		{	
			var vs_path = ("../../../" + files[f]).replace(/\//g, '\\').replace('\\..\\' + project.getProperty('app_unix_name'), '');
				
			if(ret.indexOf(vs_path) == -1)
				ret.push(vs_path);
		}
	}
	return ret;
}

function list_sources(project_id, project_type, extensions, func)
{
	if(!func)
		func = list_paths;
		
	dir = project.getBaseDir() + "/../" + project.getProperty("app_unix_name") + "/project/visual_studio_2015/";
	
	var src_path = func(project_id, project_type);
				
	var ignored = list_ignored_depedencies(project_type);
	
	var ret = [];
	
	for(var ext = 0; ext < extensions.length; ext++)
	{
		for (var i=0; i<src_path.length; i++)
		{
			ds = project.createDataType("fileset");
			ds.setDir( new java.io.File( dir + "../../../" ) );
			ds.setIncludes( src_path[i] + "/" + "**/*" + extensions[ext] );
			ds.setErrorOnMissingDir(false);
			
			files = ds.getDirectoryScanner( project ).getIncludedFiles( );
			for ( f = 0; f < files.length; f++ )
			{	
				var vs_path = ("../../../" + files[f]).replace(/\//g, '\\').replace('\\..\\' + project.getProperty('app_unix_name'), '');
					
				if(extensions[ext] == '' && vs_path.lastIndexOf('.') > vs_path.lastIndexOf('\\'))
					continue;
					
					
				var is_ignored = false;
				
				for(var k = 0 ; k < ignored.length; k++)
				{
					if(vs_path.indexOf('..\\..\\..\\_engine\\depedency\\' + ignored[k]) != -1)
						is_ignored = true;
				}
				
				if(!is_ignored)
				{		
					if(ret.indexOf(vs_path) == -1)
						ret.push(vs_path);
					/*
					echo = project.createTask("echo");
					echo.setMessage( vs_path );
					echo.perform( );
					*/
				}
			}
		}
	}
	
	return ret;
}

function src_to_filter(src)
{
	src = src.substr(0, src.lastIndexOf('\\'));
	if(src.indexOf('..\\..\\..\\_engine\\depedency\\') == 0)
	{
		return 'depedency\\' + src.substr('..\\..\\..\\_engine\\depedency\\'.length);
	}
	else if(src.indexOf('..\\..\\..\\_engine\\code\\') == 0)
	{
		return 'engine\\' + src.substr('..\\..\\..\\_engine\\code\\'.length);
	}
	else if(src.indexOf('..\\..\\code\\') == 0)
	{
		return 'code\\' + src.substr('..\\..\\code\\'.length);
	}
	return src;
}

ds = project.createDataType("dirset");
dir = project.getBaseDir() + "/" + project.getProperty("build_manifest_path");
ds.setDir( new java.io.File( dir ) );
ds.setIncludes( "*" );
ds.setExcludes( "_common" );

// iterate over files found.
projectsToGen = ds.getDirectoryScanner( project ).getIncludedDirectories( );

var solutionProjectSection = '';
var solutionProjectPostBuildOrder = '';
var solutionProjectConfigurationOnly = '';
var solutionProjectConfiguration = '';

for ( i = 0; i < projectsToGen.length; i++ )
{
	var projectToGen = projectsToGen[i];
	var manifestFilename = project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectToGen + "/manifest.xml";
					
	var projectType = get_output_type(load_file(manifestFilename));
	var binaryFilenameBase = get_binary_filename(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectToGen + '/windows/manifest.xml'));
	var binaryFilenameDebug = get_binary_filename(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectToGen + '/windows/manifest_debug.xml'));
	var binaryFilenameRelease = get_binary_filename(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectToGen + '/windows/manifest_release.xml'));
	
	if(binaryFilenameDebug == '' || binaryFilenameDebug == null)
		binaryFilenameDebug = binaryFilenameBase;	
	if(binaryFilenameRelease == '' || binaryFilenameRelease == null)
		binaryFilenameRelease = binaryFilenameBase;	
		
		
	var projectGuid = gen_guid();
	solutionProjectSection += '\nProject("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "' + projectToGen + '", "' + projectToGen + '.vcxproj", "{' + projectGuid + '}"\n	ProjectSection(ProjectDependencies) = postProject\n		{47F0434A-32FC-4640-A38D-7531A1CDB5EE} = {47F0434A-32FC-4640-A38D-7531A1CDB5EE}\n	EndProjectSection\nEndProject';
	solutionProjectPostBuildOrder += '\n{' + projectGuid + '} = {' + projectGuid + '}\n';
				
	for ( p = 0; p < projectsToGen.length; p++ )
	{								
		var projectType = get_output_type(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectsToGen[p] + "/manifest.xml"));

		if(projectType == 'executable')
		{
			if(projectToGen == projectsToGen[p])
				solutionProjectConfiguration += '{' + projectGuid + '}.debug_only_' + projectsToGen[p] + '|Win32.ActiveCfg = debug|Win32\n' +
											'{' + projectGuid + '}.debug_only_' + projectsToGen[p] + '|Win32.Build.0 = debug|Win32\n';
			else
				solutionProjectConfiguration += '{' + projectGuid + '}.debug_only_' + projectsToGen[p] + '|Win32.ActiveCfg = void|Win32\n' +
											'{' + projectGuid + '}.debug_only_' + projectsToGen[p] + '|Win32.Build.0 = void|Win32\n';
		}
	}	
	
	var projectType = get_output_type(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectsToGen[i] + "/manifest.xml"));

	if(projectType == 'test')
	{			
		solutionProjectConfiguration += '\n' +		
'{' + projectGuid + '}.debug_portable|Win32.ActiveCfg = void|Win\n' +
'{' + projectGuid + '}.debug_portable|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.debug|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.debug|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.docs|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.docs|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.final_portable|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.final_portable|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.final_installer|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.final_installer|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.hybrid_portable|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.hybrid_portable|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.hybrid_installer|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.hybrid_installer|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.test|Win32.ActiveCfg = test|Win32\n' +
'{' + projectGuid + '}.test|Win32.Build.0 = test|Win32\n';
	}
	else
	{
		solutionProjectConfiguration += '\n' +
'{' + projectGuid + '}.debug_portable|Win32.ActiveCfg = debug_portable|Win32\n' +
'{' + projectGuid + '}.debug_portable|Win32.Build.0 = debug_portable|Win32\n' +
'{' + projectGuid + '}.debug|Win32.ActiveCfg = debug|Win32\n' +
'{' + projectGuid + '}.debug|Win32.Build.0 = debug|Win32\n' +
'{' + projectGuid + '}.docs|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.docs|Win32.Build.0 = void|Win32\n' +
'{' + projectGuid + '}.final_portable|Win32.ActiveCfg = final_portable|Win32\n' +
'{' + projectGuid + '}.final_portable|Win32.Build.0 = final_portable|Win32\n' +
'{' + projectGuid + '}.final_installer|Win32.ActiveCfg = final_installer|Win32\n' +
'{' + projectGuid + '}.final_installer|Win32.Build.0 = final_installer|Win32\n' +
'{' + projectGuid + '}.hybrid_portable|Win32.ActiveCfg = hybrid_portable|Win32\n' +
'{' + projectGuid + '}.hybrid_portable|Win32.Build.0 = hybrid_portable|Win32\n' +
'{' + projectGuid + '}.hybrid_installer|Win32.ActiveCfg = hybrid_installer|Win32\n' +
'{' + projectGuid + '}.hybrid_installer|Win32.Build.0 = hybrid_installer|Win32\n' +
'{' + projectGuid + '}.test|Win32.ActiveCfg = void|Win32\n' +
'{' + projectGuid + '}.test|Win32.Build.0 = void|Win32\n';

		
	}
	
	if(projectType == 'executable')
		solutionProjectConfigurationOnly += 'debug_only_' + projectToGen + '|Win32 = debug_only_' + projectToGen + '|Win32\n';

		
	antcall = project.createTask("antcall");
	antcall.setTarget('copy_vs_project');
	var param1 = antcall.createParam();	param1.setName("copy_vs_project.type");	param1.setValue(projectType);
	var param2 = antcall.createParam();	param2.setName("copy_vs_project.unix_name"); param2.setValue(projectToGen);
	antcall.perform();
	
	var vcxproj_path = project.getBaseDir() + "/" + project.getProperty('build_vs_path') + "/" + projectToGen + ".vcxproj";
	var vcxproj = load_file(vcxproj_path);
	var vcxproj_user = load_file(vcxproj_path + '.user');
	var vcxproj_filters = load_file(vcxproj_path + '.filters');
	
	vcxproj_user = vcxproj_user.replace(/\$BINARY_FILENAME_DEBUG\$/g, binaryFilenameDebug);
	vcxproj_user = vcxproj_user.replace(/\$BINARY_FILENAME_RELEASE\$/g, binaryFilenameRelease);
	
	vcxproj = vcxproj.replace(/\$APP_UNIX_NAME\$/g, project.getProperty('app_unix_name'));
	
	var cpp = list_sources(projectToGen, projectType, [".cpp", ".c", ".cc"]);
	var hpp = list_sources(projectToGen, projectType, [".hpp", ".h", ""]);
	var rc = list_sources(projectToGen, projectType, [".rc"]);
	var libs = list_sources(projectToGen, projectType, [".lib"], list_libraries);
	
	var cl_additional_includes = '';
	var already_included = [];
	var includes_paths = list_additional_includes(projectToGen, projectType);
	
	for(var c  = 0; c < includes_paths.length; c++)
	{
		var dir = includes_paths[c];
		if(already_included.indexOf(dir) == -1)
		{
			cl_additional_includes += ('$(SolutionDir)' + dir + ';');
			already_included.push(dir);
		}
	}
	vcxproj = vcxproj.replace(/\$CL_ADDITIONAL_INCLUDES\$/g, cl_additional_includes);
	
	var cl_additional_libraries = '';
	var cl_additional_libraries_filenames = '';
	var already_included1 = [];
	var already_included2 = [];
	
	for(var c  = 0; c < libs.length; c++)
	{
		var dir = libs[c].substr(0, libs[c].lastIndexOf('\\'));
		var lib = libs[c].substr(libs[c].lastIndexOf('\\') + 1);
		if(already_included1.indexOf(dir) == -1)
		{
			cl_additional_libraries += ('$(SolutionDir)' + dir + ';');
			already_included1.push(dir);
		}
		if(already_included2.indexOf(lib) == -1)
		{
			cl_additional_libraries_filenames += (lib + ";");
			already_included2.push(lib);
		}
	}
	vcxproj = vcxproj.replace(/\$CL_ADDITIONAL_LIBRARIES\$/g, cl_additional_libraries);
	vcxproj = vcxproj.replace(/\$CL_ADDITIONAL_LIBRARIES_NAMES\$/g, cl_additional_libraries_filenames);
	
	var filters = [];
	
	var cl_section = '';
	var cl_section2 = '';
	for(var c = 0; c < cpp.length; c++)
	{
		cl_section += '\n	<ClCompile Include="' + cpp[c] + '" />';
		
		var filter = src_to_filter(cpp[c]);
		if(filters.indexOf(filter) == -1)
			filters.push(filter);
			
		cl_section2 += '\n	<ClCompile Include="' + cpp[c] + '">\n      <Filter>' + filter + '</Filter>\n    </ClCompile>';
	}
	vcxproj = vcxproj.replace('$CL_COMPILE_SECTION$', cl_section);
	vcxproj_filters = vcxproj_filters.replace('$CL_COMPILE_SECTION$', cl_section2);
	
	cl_section = '';
	cl_section2 = '';
	for(var c = 0; c < hpp.length; c++)
	{
		cl_section += '\n	<ClInclude Include="' + hpp[c] + '" />';
		
		var filter = src_to_filter(hpp[c]);
		if(filters.indexOf(filter) == -1)
			filters.push(filter);
			
		cl_section2 += '\n	<ClInclude Include="' + hpp[c] + '">\n      <Filter>' + filter + '</Filter>\n    </ClInclude>';
	}
	vcxproj = vcxproj.replace('$CL_INCLUDE_SECTION$', cl_section);
	vcxproj_filters = vcxproj_filters.replace('$CL_INCLUDE_SECTION$', cl_section2);
	
	cl_section = '';
	cl_section2 = '';
	for(var c = 0; c < rc.length; c++)
	{
		cl_section += '\n	<ResourceCompile Include="' + rc[c] + '" />';
		
		var filter = src_to_filter(rc[c]);
		if(filters.indexOf(filter) == -1)
			filters.push(filter);
			
		cl_section2 += '\n	<ResourceCompile Include="' + rc[c] + '">\n      <Filter>' + filter + '</Filter>\n    </ResourceCompile>';
	}
	vcxproj = vcxproj.replace('$CL_RESOURCE_SECTION$', cl_section);
	vcxproj_filters = vcxproj_filters.replace('$CL_RESOURCE_SECTION$', cl_section2);
	
	var filters_base = [];
	
	for(var c = 0; c < filters.length; c++)
	{
		var path_part = filters[c].split("\\");
		
		var path_item = '';
		
		for(var p = 0; p < path_part.length - 1; p++)
		{
			if(p > 0) path_item += "\\";
			
			path_item += path_part[p];
			
			if(filters_base.indexOf(path_item) == -1)
				filters_base.push(path_item);
		}
	}
	
	cl_section = '';
	for(var c = 0; c < filters_base.length; c++)
	{
		cl_section += '\n	<Filter Include="' + filters_base[c] + '">\n      <UniqueIdentifier>{' + gen_guid() + '}</UniqueIdentifier>\n    </Filter>';
	}
	for(var c = 0; c < filters.length; c++)
	{
		cl_section += '\n	<Filter Include="' + filters[c] + '">\n      <UniqueIdentifier>{' + gen_guid() + '}</UniqueIdentifier>\n    </Filter>';
	}
	vcxproj_filters = vcxproj_filters.replace('$CL_FILTERS_SECTION$', cl_section);
	
	
	save_file(vcxproj_path, vcxproj);
	save_file(vcxproj_path + '.user', vcxproj_user);
	save_file(vcxproj_path + '.filters', vcxproj_filters);
	/*
	
	echo = project.createTask("echo");
	echo.setMessage( project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectToGen + '/windows/manifest_debug.xml' );
	echo.perform( );
	
	*/
}

for ( p = 0; p < projectsToGen.length; p++ )
{								
	var projectType = get_output_type(load_file(project.getBaseDir() + "/" + project.getProperty("build_manifest_path") + "/" + projectsToGen[p] + "/manifest.xml"));

	if(projectType != 'test')
		solutionProjectConfiguration += '{2E20FD25-ABA9-4471-B0A4-F3F358D9DC2B}.debug_only_' + projectsToGen[p] + '|Win32.ActiveCfg = debug|Win32\n' +
										'{2E20FD25-ABA9-4471-B0A4-F3F358D9DC2B}.debug_only_' + projectsToGen[p] + '|Win32.Build.0 = debug|Win32\n' +
										'{47F0434A-32FC-4640-A38D-7531A1CDB5EE}.debug_only_' + projectsToGen[p] + '|Win32.ActiveCfg = debug|Win32\n' +
										'{47F0434A-32FC-4640-A38D-7531A1CDB5EE}.debug_only_' + projectsToGen[p] + '|Win32.Build.0 = debug|Win32\n';
	
}	

var sln_path = project.getBaseDir() + "/" + project.getProperty('build_vs_path') + "/" + project.getProperty('app_unix_name') + ".sln";
var sln = load_file(sln_path);
sln = sln.replace('$SLN_PROJECT_SECTION$', solutionProjectSection);
sln = sln.replace('$SLN_POSTBUILD_SECTION$', solutionProjectPostBuildOrder);
sln = sln.replace('$SLN_CONFIGURATION_ONLY_SECTION$', solutionProjectConfigurationOnly);
sln = sln.replace('$SLN_CONFIGURATION_SECTION$', solutionProjectConfiguration);
save_file(sln_path, sln);