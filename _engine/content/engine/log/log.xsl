<?xml version="1.0"?>

<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:variable name="smallcase" select="'abcdefghijklmnopqrstuvwxyz'" />
<xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

<xsl:template name="break">
  <xsl:param name="text" select="."/>
  <xsl:choose>
    <xsl:when test="contains($text, '&#xa;')">
      <xsl:value-of select="substring-before($text, '&#xa;')"/>
      <br />
      <xsl:call-template name="break">
        <xsl:with-param 
          name="text" 
          select="substring-after($text, '&#xa;')"
        />
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$text"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="/">
  <html>
  <head>
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <script type="text/javascript">
	<![CDATA[
	Array.prototype.contains = function(obj)
	{
		var i = this.length;
		while (i--)
		{
			if (this[i] === obj)
			{
				return true;
			}
		}
		return false;
	}
	function has_class(element, cls)
	{
		return (' ' + element.className + ' ').indexOf(' ' + cls + ' ') > -1;
	}
	function update_rows()
	{
		var content = document.getElementById('content');
		var rows = content.getElementsByTagName("tr");
		
		var checked_message = document.getElementById('switch_show_message').checked;
		var checked_warning = document.getElementById('switch_show_warning').checked;
		var checked_error = document.getElementById('switch_show_error').checked;
		var checked_taskinterrupted = document.getElementById('switch_show_taskinterrupted').checked;
		var checked_taskdone = document.getElementById('switch_show_taskdone').checked;
		var checked_taskfailed = document.getElementById('switch_show_taskfailed').checked;
		
		var threads_to_ignore = [];
		var threads = document.getElementsByClassName("thread-info");
		for (var i=0; i < threads.length; i++)
		{
			var thread_id = threads[i].getAttribute('data-id');
			if(!threads[i].checked)
			{
				threads_to_ignore.push(thread_id);
			}
		}
		var functions_to_ignore = [];
		var functions = document.getElementsByClassName("function-info");
		for (var i=0; i < functions.length; i++)
		{
			var function_id = functions[i].getAttribute('data-id');
			if(!functions[i].checked)
			{
				functions_to_ignore.push(function_id);
			}
		}
		var search_box = document.getElementById('toolbox_search');
		var searching = search_box.value.trim().toLowerCase().split(' ');
		
		for (var i=0; i < rows.length; i++)
		{
		    var show = true;
			if(has_class(rows[i], 'tr_message') || has_class(rows[i], 'tr_message_alt'))
			{
				if(!checked_message) show = false;
			}
			if(has_class(rows[i], 'tr_warning') || has_class(rows[i], 'tr_warning_alt'))
			{
				if(!checked_warning) show = false;
			}
			if(has_class(rows[i], 'tr_error') || has_class(rows[i], 'tr_error_alt'))
			{
				if(!checked_error) show = false;
			}
			if(has_class(rows[i], 'tr_taskinterrupted') || has_class(rows[i], 'tr_taskinterrupted_alt'))
			{
				if(!checked_taskinterrupted) show = false;
			}
			if(has_class(rows[i], 'tr_taskdone') || has_class(rows[i], 'tr_taskdone_alt'))
			{
				if(!checked_taskdone) show = false;
			}
			if(has_class(rows[i], 'tr_taskfailed') || has_class(rows[i], 'tr_taskfailed_alt'))
			{
				if(!checked_taskfailed) show = false;
			}
			
			if(threads_to_ignore.contains(rows[i].getAttribute('data-thread')))
				show = false;
			
			if(functions_to_ignore.contains(rows[i].getAttribute('data-function')))
				show = false;
				
			if(show && searching.length > 0)
			{
				var not_found = true;
				var srch = rows[i].getAttribute('data-search').toLowerCase();
				for (var k=0; k < searching.length; k++)
				{
					if(srch.indexOf(searching[k]) != -1)
					{
						not_found = false;
						break;
					}
				}
				if(not_found)
					show = false;
			}
			
			var cols = rows[i].getElementsByTagName("td");
			for (var k=0; k < cols.length; k++)
			{
				cols[k].childNodes[0].style.opacity = show ? 1 : 0;
				cols[k].childNodes[0].style.maxHeight = show ? '100px' : '0px';
			}
		}
	}
	function on_load()
	{
		var is_firefox = typeof InstallTrigger !== 'undefined';
		
		var year = new Date().getFullYear();
		if(year > 2016)
			document.getElementById('js_year').innerHTML = '- ' + year + ' ';
		
		if(is_firefox)
		{
			document.getElementById('hide_if_not_firefox').style.display = 'block';
			document.getElementById('hide_if_firefox').style.display = 'none';
			
			var content = document.getElementById('content');
			var rows = content.getElementsByTagName("tr");
			var threads = [];
			for (var i=0; i < rows.length; i++)
			{
				var thread_id = rows[i].getAttribute('data-thread');
				if(!threads.contains(thread_id))
					threads.push(thread_id);
			}
			var functions = [];
			for (var i=0; i < rows.length; i++)
			{
				var function_id = rows[i].getAttribute('data-function');
				if(!functions.contains(function_id))
					functions.push(function_id);
			}
			
			var container = document.getElementById('toolbox_container');
			
			for (var i=0; i < threads.length; i++)
			{
				container.innerHTML = container.innerHTML + '<div class="toolbox_content">' +
					'<label class="switch switch-light">' +
						'<input class="switch-input thread-info" data-id="' + threads[i] + '" type="checkbox" checked="checked" onclick="update_rows();" />' +
						'<span class="switch-label" data-name="Thread ' + threads[i] + '"></span>' +
						'<span class="switch-handle"></span>' +
					'</label>' +
				'</div>';
			}
			for (var i=0; i < functions.length; i++)
			{
				container.innerHTML = container.innerHTML + '<div class="toolbox_content">' +
					'<label class="switch switch-light">' +
						'<input class="switch-input function-info" data-id="' + functions[i] + '" type="checkbox" checked="checked" onclick="update_rows();" />' +
						'<span class="switch-label switch-functional" data-name="' + functions[i] + '()"></span>' +
						'<span class="switch-handle"></span>' +
					'</label>' +
				'</div>';
			}
			
			update_rows();
		}
	}
	]]>
  </script>
  <title>
  <xsl:value-of select="pixie/logger/execution_info/game_name/value" /> log (<xsl:value-of select="pixie/logger/execution_info/app_type/value" />)
  </title>
  <style type="text/css">
	#hide_if_not_firefox
	{
		display:none;
	}
	body
	{
		background-color:#1e226f;
		color:white;
		font-family: Verdana, Arial, Helvetica, sans-serif;
		margin:0px;
		padding:0px;
	}
	h1
	{
		text-align:center;
		margin-top:15px
	}
	#info
	{
		border-style:double;
		border-width:3px;
		border-color:#fcffa2;
		border-collapse:collapse;
		width:50%;
		margin-left:25%;
		margin-bottom:50px;
		margin-top:25px;
		
		border-radius: 10px;
		-moz-border-radius: 10px;
		-webkit-border-radius: 10px;
		
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
	}
	.info_th_image
	{
		width:40%;
		text-align:center;
		padding:5px;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
		font-size:20px;
	}
	#info_th
	{
		width:60%;
		text-align:left;
		padding:15px;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
		font-size:20px;
	}
	.tr_info
	{
		background-color:#323797;
	}
	.tr_info_alt
	{
		background-color:#1e226f;
	}
	.td_info_first
	{
		width:40%;
		padding:5px;
		text-align:right;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
		font-weight:bold;
	}
	.td_info_second
	{
		width:60%;
		padding:5px;
		text-align:left;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
	}
	#content
	{
		border-style:double;
		border-width:3px;
		border-color:#fcffa2;
		border-collapse:collapse;
		width:90%;
		margin-left:5%;
		margin-bottom:50px;
		margin-top:25px;
		
		border-radius: 10px;
		-moz-border-radius: 10px;
		-webkit-border-radius: 10px;
		
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
	}
	td > div
	{
		transition: All 1.3s ease;
		-webkit-transition: All 1.3s ease;
		-moz-transition: All 1.3s ease;
		-o-transition: All 1.3s ease;
		max-height:100px;
	}
	.tr_taskinterrupted
	{
		background-color:#eb9b00;
	}
	.tr_taskinterrupted_alt
	{
		background-color:#cb8600;
	}
	.tr_taskfailed
	{
		background-color:#aa0000;
	}
	.tr_taskdone
	{
		background-color:#329737;
	}
	.tr_taskdone_alt
	{
		background-color:#1e6f22;
	}
	.tr_error
	{
		background-color:#ff0000;
	}
	.tr_warning
	{
		background-color:#eb9b00;
	}
	.tr_warning_alt
	{
		background-color:#cb8600;
	}
	.tr_message
	{
		background-color:#323797;
	}
	.tr_message_alt
	{
		background-color:#1e226f;
	}
	.td_first
	{
		width:64px;
		border-style:solid none solid solid;
		border-color:#fcffa2;
		border-width:1px;
	}
	.td_second
	{
		width:40%;
		padding-left:10px;
		text-align:left;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
	}
	.td_third
	{
		border-style:solid solid solid none;
		border-color:#fcffa2;
		border-width:1px;
	}
	#toolbox
	{
		border-radius: 24px 24px 0px 0px;
		-moz-border-radius: 24px 24px 0px 0px;
		-webkit-border-radius: 24px 24px 0px 0px;
		border: 2px groove #626670;
		border-style:solid solid none solid;
		border-width:2px;
		background-color: #212327;
		color: #626670;
		margin-bottom:0px;
		margin-left:0px;
		margin-right:0px;
		padding:25px;
		
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
	}
	#toolbox_box
	{
		border-radius: 20px 20px 20px 20px;
		-moz-border-radius: 20px 20px 20px 20px;
		-webkit-border-radius: 20px 20px 20px 20px;
		border: none;
		background-color: #626670;
		width:90%;
		height:384px;
		padding:16px;
		margin-bottom:32px;
		margin-left:5%;
	}
	#toolbox_container
	{
		height:100%;
		overflow:auto;
	}
	#toolbox_search
	{
		border-radius: 5px 5px 5px 5px;
		-moz-border-radius: 5px 5px 5px 5px;
		-webkit-border-radius: 5px 5px 5px 5px;
		padding:5px;
		height:16px;
		margin-top:16px;
		margin-right:16px;
		width:256px;
		background-color: #aaa;
		border:none;
		font-size:16px;
	}
	.toolbox_content
	{
		float:left;
	}
	.toolbox_icon
	{
		width:64px;
		margin-right:15px;
	}
	hr
	{
		background-color: #626670;
		color: #626670;
	}
	#footer
	{
		text-align:center;
		margin-top:25px;
		font-style:italic;
	}
	#footer_logo
	{
	
	}
	.switch
	{
		position: relative;
		display: block;
		width: 64px;
		height: 64px;
		cursor: pointer;
	}
	.switch-input
	{
		position: absolute;
		top: 0;
		left: 0;
		opacity: 0;
	}
	.switch-label
	{
		position: relative;
		display: block;
		height: inherit;
	}
	.switch-label:before, .switch-label:after
	{
		position: absolute;
		-webkit-transition: inherit;
		-moz-transition: inherit;
		-o-transition: inherit;
		transition: inherit;
	}
	.switch-label:before
	{
		content: '';
	}
	.switch-label:after
	{
		content: '';
		opacity: 0;
	}
	.switch-input:checked ~ .switch-label:before
	{
		opacity: 0;
	}
	.switch-input:checked ~ .switch-label:after
	{
		opacity: 1;
	}
	 
	.switch-label, .switch-handle
	{
		transition: All 0.3s ease;
		-webkit-transition: All 0.3s ease;
		-moz-transition: All 0.3s ease;
		-o-transition: All 0.3s ease;
	}
	
	.switch-yes-no
	{
		background: #ccc;
	}
	.switch-yes-no :hover
	{
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
	}
	.switch-yes-no .switch-label:after, .switch-yes-no .switch-label:before
	{
		width: 100%;
		height: 100%;
		text-align: center;
	}
	.switch-yes-no .switch-label:after
	{
		background: #626670;
		backface-visibility: hidden;
		transform: rotateY(180deg);
	}
	.switch-yes-no .switch-label:before
	{
		background: #626670;
		backface-visibility: hidden;
	}
	.switch-yes-no .switch-input:checked ~ .switch-label:before
	{
		transform: rotateY(180deg)
	}
	.switch-yes-no .switch-input:checked ~ .switch-label:after
	{
		transform: rotateY(0)
	}
	.switch-message .switch-label:after
	{
		background-image:url('./engine/log/item_message.png');
	}
	.switch-message .switch-label:before
	{
		background-image:url('./engine/log/item_message_disabled.png');
	}
	.switch-warning .switch-label:after
	{
		background-image:url('./engine/log/item_warning.png');
	}
	.switch-warning .switch-label:before
	{
		background-image:url('./engine/log/item_warning_disabled.png');
	}
	.switch-error .switch-label:after
	{
		background-image:url('./engine/log/item_error.png');
	}
	.switch-error .switch-label:before
	{
		background-image:url('./engine/log/item_error_disabled.png');
	}
	.switch-taskinterrupted .switch-label:after
	{
		background-image:url('./engine/log/item_task_interrupted.png');
	}
	.switch-taskinterrupted .switch-label:before
	{
		background-image:url('./engine/log/item_task_interrupted_disabled.png');
	}
	.switch-taskdone .switch-label:after
	{
		background-image:url('./engine/log/item_task_done.png');
	}
	.switch-taskdone .switch-label:before
	{
		background-image:url('./engine/log/item_task_done_disabled.png');
	}
	.switch-taskfailed .switch-label:after
	{
		background-image:url('./engine/log/item_task_failed.png');
	}
	.switch-taskfailed .switch-label:before
	{
		background-image:url('./engine/log/item_task_failed_disabled.png');
	}
	.no-overflow
	{
		overflow:hidden;
	}
	/* TODO */
	.switch-light
	{
		padding: 0;
		background: #aaa;
		background-image: none;
		height:48px;
		width:196px;
		margin-right:16px;
		margin-top:8px;
		border-radius: 10px 10px 10px 10px;
		-moz-border-radius: 10px 10px 10px 10px;
		-webkit-border-radius: 10px 10px 10px 10px;
		border: 2px solid #212327;
		text-align:center;
	}
	.switch-light :hover
	{
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
	}
	.switch-light .switch-label
	{
		height:41px;
		width:186px;
		padding:5px 5px 2px 5px;
		word-break: break-all;
		vertical-align:middle;
		display:block;
	}
	.switch-light .switch-label:after
	{
		margin-left:-93px;
		color: #aaa;
		content: attr(data-name);
		width:186px;
		text-align:center;
		height:auto;
	}
	.switch-light .switch-label:before
	{
		margin-left:-93px;
		color: #212327;
		content: attr(data-name);
		width:186px;
		text-align:center;
		height:auto;
	}
	.switch-light .switch-functional:after
	{
		font-size:11px;
	}
	.switch-light .switch-functional:before
	{
		font-size:11px;
	}
	.switch-light .switch-input:checked ~ .switch-label
	{
		background: #212327;
		border-color: #9370DB;
	}
	a:link,a:visited
	{
		color: white;
		text-decoration: none;
		font-style: normal;
		target-new: tab;
	}
	a:hover
	{
		color: yellow;
		text-decoration: none;
		font-style: normal;
		target-new: tab;
	}
	a:active
	{
		color: red;
		text-decoration: none;
		font-style: normal;
		target-new: tab;
	}
  </style>
  </head>
  <body onload="on_load()">
    <h1><xsl:value-of select="pixie/logger/execution_info/game_name/value" /> log (<xsl:value-of select="pixie/logger/execution_info/app_type/value" />)</h1>
	<table id="info">
		<tr>
			<th class="info_th_image">
				<img src="./engine/log/info.png" />
			</th>
			<th id="info_th" colspan="2">Platform information</th>
		</tr>
		<xsl:for-each select="pixie/logger/execution_info/*">
			<xsl:if test="position() mod 2 = 0">
				<xsl:if test="status = 'normal'">
					<tr class="tr_info_alt" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/platform_{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
				<xsl:if test="status = 'good'">
					<tr class="tr_info_alt" style="background-color:#007700" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
				<xsl:if test="status = 'bad'">
					<tr class="tr_info_alt" style="background-color:#ff0000" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
			</xsl:if>
			<xsl:if test="position() mod 2 = 1">
				<xsl:if test="status = 'normal'">
					<tr class="tr_info" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
				<xsl:if test="status = 'good'">
					<tr class="tr_info" style="background-color:#007700" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
				<xsl:if test="status = 'bad'">
					<tr class="tr_info" style="background-color:#ff0000" data-key="{name(.)}" data-value="{value}">
						<td class="td_info_first">
							<xsl:value-of select="desc" />:
						</td>
						<td class="td_info_second">
							<xsl:if test="name(.) = 'os_name'">
								<img src="./engine/log/{translate(value, $uppercase, $smallcase)}.png" alt="" style="margin-right:5px;" />
							</xsl:if>
							<xsl:value-of select="value" />
							<xsl:if test="name(.) = 'cpu_cache'">kb</xsl:if>
						</td>
					</tr>
				</xsl:if>
			</xsl:if>
		</xsl:for-each>
	</table>
    <table id="content">
    <xsl:for-each select="pixie/logger/items/value">
	<xsl:if test="level = 'error'">
		<tr class="tr_error" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
	</xsl:if>
	<xsl:if test="level = 'warning'">
		<xsl:if test="position() mod 2 = 0">
		<tr class="tr_warning" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
		<xsl:if test="position() mod 2 = 1">
		<tr class="tr_warning_alt" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
	</xsl:if>
	<xsl:if test="level = 'message'">
		<xsl:if test="position() mod 2 = 0">
		<tr class="tr_message" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
		<xsl:if test="position() mod 2 = 1">
		<tr class="tr_message_alt" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
	</xsl:if>
	<xsl:if test="level = 'task_failed'">
		<tr class="tr_taskfailed" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>... FAILED!
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
	</xsl:if>
	<xsl:if test="level = 'task_done'">
		<xsl:if test="position() mod 2 = 0">
		<tr class="tr_taskdone" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>... Done!
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
		<xsl:if test="position() mod 2 = 1">
		<tr class="tr_taskdone_alt" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>... Done!
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
	</xsl:if>
	<xsl:if test="level = 'task_interrupted'">
		<xsl:if test="position() mod 2 = 0">
		<tr class="tr_taskinterrupted" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>...
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
		<xsl:if test="position() mod 2 = 1">
		<tr class="tr_taskinterrupted_alt" data-thread="{thread}" data-function="{function}" data-search="{level} {message} {function} {file} {line} {frame} {time} {thread}">
			<td class="td_first"><div class="no-overflow"><xsl:if test="level = 'task_interrupted'"><img src="./engine/log/item_task_interrupted.png" alt="(Task - Interrupted)" /></xsl:if><xsl:if test="level = 'task_done'"><img src="./engine/log/item_task_done.png" alt="(Task - Done)" /></xsl:if><xsl:if test="level = 'task_failed'"><img src="./engine/log/item_task_failed.png" alt="(Task - Failed)" /></xsl:if><xsl:if test="level = 'message'"><img src="./engine/log/item_message.png" alt="(message)" /></xsl:if><xsl:if test="level = 'warning'"><img src="./engine/log/item_warning.png" alt="(warning)" /></xsl:if><xsl:if test="level = 'error'"><img src="./engine/log/item_error.png" alt="(error)" /></xsl:if></div></td>
			<td class="td_second">
				<div class="no-overflow">
					<xsl:call-template name="break">
						<xsl:with-param name="text" select="message" />
					</xsl:call-template>...
				</div>
			</td>
			<td class="td_third"><div class="no-overflow">In function <b><xsl:value-of select="function"/>()</b><br />In file <b><xsl:value-of select="file"/></b> on line <b><xsl:value-of select="line"/></b><br />Frame <b><xsl:value-of select="frame"/></b>, Time <b><xsl:value-of select="time"/></b>, ThreadID: <b><xsl:value-of select="thread"/></b></div></td>
		 </tr>
		</xsl:if>
	</xsl:if>
    </xsl:for-each>
    </table>
	<div id="toolbox">
		<div id="hide_if_not_firefox">
			<h1>Toolbox</h1>
			<div id="toolbox_box">
				<div id="toolbox_container">
					<div class="toolbox_content">
						<input type="text" id="toolbox_search" placeholder="Search..." oninput="update_rows();" />
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-message">
							<input id="switch_show_message" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-warning">
							<input id="switch_show_warning" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-error">
							<input id="switch_show_error" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-taskinterrupted">
							<input id="switch_show_taskinterrupted" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-taskdone">
							<input id="switch_show_taskdone" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
					<div class="toolbox_content toolbox_icon">
						<label class="switch switch-yes-no switch-taskfailed">
							<input id="switch_show_taskfailed" class="switch-input" type="checkbox" checked="checked" onclick="update_rows();" />
							<span class="switch-label"></span> 
						</label>
					</div>
				</div>
			</div>
		</div>
		<div id="hide_if_firefox">
			<h1>We <b>strongly</b> recomend to view this file in Mozilla Firefox, for maximum control!</h1>
		</div>
		<hr />
		<div id="footer">Powered by <img src="./engine/logo.png" alt="Pixie Engine" id="footer_logo" /><br />Copyright &#169; 2016 <span id="js_year"></span> by <b><a href="http://games.kosek.com" target="_blank">Kosek.com</a></b></div>
	</div>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>