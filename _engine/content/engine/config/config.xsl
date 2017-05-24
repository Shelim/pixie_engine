<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <head>
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <title>
  The current <xsl:value-of select="pixie/game_name" /> configuration
  </title>
  <style type="text/css">
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
		margin-top:5px
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
	.tr_settings
	{
		background-color:#323797;
	}
	.tr_settings_alt
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
		width:300px;
		border-style:solid none solid none;
		border-color:#fcffa2;
		border-width:1px;
		text-align:right;
	}
	.td_third
	{
		padding-left:10px;
		text-align:left;
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
		margin-top:50px;
		padding:25px;
		
		box-shadow: 0px 0px 10px 5px #777777;
		-moz-box-shadow: 0px 0px 10px 5px #777777;
		-webkit-box-shadow: 0px 0px 10px 5px #777777;
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
  <script type="text/javascript">
	<![CDATA[
	function on_load()
	{
		var year = new Date().getFullYear();
		if(year > 2016)
			document.getElementById('js_year').innerHTML = '- ' + year + ' ';
	}
	]]>
  </script>
  </head>
  <body onload="on_load()">
    <h1>The current <xsl:value-of select="pixie/game_name" /> configuration</h1>
    <table id="content">
    <xsl:for-each select="pixie/config/*">
	<xsl:if test="position() mod 2 = 0">
	<tr class="tr_settings">
		<td class="td_first"><img src="./engine/config/config_item.png" alt="(item)" /></td>
		<td class="td_second"><b><xsl:value-of select="name()" /> = </b></td>
		<td class="td_third"><xsl:value-of select="." /></td>
	</tr>
	</xsl:if>
	<xsl:if test="position() mod 2 = 1">
	<tr class="tr_settings_alt">
		<td class="td_first"><img src="./engine/config/config_item.png" alt="(item)" /></td>
		<td class="td_second"><b><xsl:value-of select="name()" /> = </b></td>
		<td class="td_third"><xsl:value-of select="." /></td>
    </tr>
	</xsl:if>
    </xsl:for-each>
    </table>
	<div id="toolbox">
		<div id="footer">Powered by <img src="./engine/logo.png" alt="Pixie Engine" id="footer_logo" /><br />Copyright &#169; 2016 <span id="js_year"></span> by <b><a href="http://games.kosek.com" target="_blank">Kosek.com</a></b></div>
	</div>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>