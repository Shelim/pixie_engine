<?xml version="1.0"?>

<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <head>
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <title>
  The current <xsl:value-of select="keybinding/@game" /> keybinding
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
		margin-top:15px
	}
	h2
	{
		text-align:left;
		font-size:14px;
	}
	#content
	{
		border-style:double;
		border-color:#fcffa2;
		border-collapse:collapse;
		width:90%;
		margin-left:5%;
		margin-bottom:50px
	}
	.tr_settings
	{
		background-color:#323797;
	}
	.tr_settings_alt
	{
		background-color:#1e226f;
	}
	td
	{
		padding:16px;
	}
	.div_bind
	{
		font-size:14px;
	}
	.div_item
	{
		background-color:#7279eb;
		padding:8px;
		display:inline-block;
		vertical-align:bottom;
		-moz-border-radius: 15px;
		border-radius: 15px;
	}
	.div_button
	{
		color:#FF0000;
		width:32px;
		height:48px;
		padding-left:16px;
		padding-right:16px;
		padding-top:16px;
		background-image:url('./engine/keybinding/key_base.png');
		display:inline-block;
		font-size:11px;
		word-wrap:break-word;
	}
	.div_mod
	{
		color:#FF0000;
		width:64px;
		height:48px;
		padding-left:16px;
		padding-right:16px;
		padding-top:16px;
		background-image:url('./engine/keybinding/key_mod.png');
		display:inline-block;
		font-size:11px;
		word-wrap:break-word;
	}
	.div_other
	{
		vertical-align:bottom;
	}
	.div_sep
	{
		display:inline-block;
		padding-bottom:32px;
	}
  </style>
  </head>
  <body>
    <h1>The current <xsl:value-of select="keybinding/@game" /> keybinding</h1>
    <table id="content">
    <xsl:for-each select="keybinding/action">
	<xsl:if test="position() mod 2 = 0">
	<tr class="tr_settings">
		<td><h2>Bind for '<xsl:value-of select="@name" />':</h2>
		<div class="div_bind">
		<xsl:for-each select="bind">
			<div class="div_item">
			<xsl:if test="@mod != ''">
				<xsl:if test="not(contains(@mod, '|'))">
					<div class="div_mod">
						<xsl:value-of select="@mod" />
					</div>
					+
				</xsl:if>
				<xsl:if test="contains(@mod, '|')">
					<xsl:apply-templates select="."/>
				</xsl:if>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="@key = 'LMB'"><img class="div_other" src="./engine/keybinding/mouse_{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'MMB'"><img class="div_other" src="./engine/keybinding/mouse_{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'RMB'"><img class="div_other" src="./engine/keybinding/mouse_{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'X1MB'"><img class="div_other" src="./engine/keybinding/mouse_{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'X2MB'"><img class="div_other" src="./engine/keybinding/mouse_{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = ''"><img class="div_other" src="./engine/keybinding/unknown.png" alt="unknown?" /></xsl:when>
				<xsl:otherwise>
					<div class="div_button">
						<xsl:value-of select="@key" />
					</div>
				</xsl:otherwise>
			</xsl:choose>
			</div>
			<xsl:if test="position()!=last()">
				<div class="div_sep">OR</div>
			</xsl:if>
		</xsl:for-each>
		</div>
		</td>
	</tr>
	</xsl:if>
	<xsl:if test="position() mod 2 = 1">
	<tr class="tr_settings_alt">
		<td><h2>Bind for '<xsl:value-of select="@name" />':</h2>
		<div class="div_bind">
		<xsl:for-each select="bind">
			<div class="div_item">
			<xsl:if test="@mod != ''">
				<xsl:if test="not(contains(@mod, '|'))">
					<div class="div_mod">
						<xsl:value-of select="@mod" />
					</div>
					+
				</xsl:if>
				<xsl:if test="contains(@mod, '|')">
					<xsl:apply-templates select="."/>
				</xsl:if>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="@key = 'LMB'"><img class="div_other" src="./engine/keybinding/{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'MMB'"><img class="div_other" src="./engine/keybinding/{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'RMB'"><img class="div_other" src="./engine/keybinding/{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'X1MB'"><img class="div_other" src="./engine/keybinding/{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = 'X2MB'"><img class="div_other" src="./engine/keybinding/{@key}.png" alt="{@key}" /></xsl:when>
				<xsl:when test="@key = ''"><img class="div_other" src="./engine/keybinding/unknown.png" alt="unknown?" /></xsl:when>
				<xsl:otherwise>
					<div class="div_button">
						<xsl:value-of select="@key" />
					</div>
				</xsl:otherwise>
			</xsl:choose>
			</div>
			<xsl:if test="position()!=last()">
				<div class="div_sep">OR</div>
			</xsl:if>
		</xsl:for-each>
		</div>
		</td>
    </tr>
	</xsl:if>
    </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>


<xsl:template match="bind">
 <xsl:call-template name="tokenize"> 
   <xsl:with-param name="item" select="@mod" /> 
 </xsl:call-template>    
</xsl:template>

<xsl:template name="tokenize">
	<xsl:param name="item" />
	<xsl:variable name="first-item" select="normalize-space(substring-before( concat( $item, '|'), '|'))" /> 
	<xsl:if test="$first-item">
		<div class="div_mod">
			<xsl:value-of select="$first-item" />
		</div>
		+
		<xsl:call-template name="tokenize"> 
			<xsl:with-param name="item" select="substring-after($item,'|')" /> 
		</xsl:call-template>    
	</xsl:if>  
</xsl:template>

</xsl:stylesheet>